%{
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "attr.h"
#include "instrutil.h"
int yylex();
void yyerror(char * s);
#include "symtab.h"
#include "deptest.h"
#include <string.h>

FILE *outfile;
FILE *depstats;
char *CommentBuffer;

%}

%union {
  tokentype token;
  regInfo targetReg;
  loopcontrol loopctrl;
}

%token PROG PERIOD VAR
%token INT BOOL PRINT THEN IF FI ENDWHILE DO ENDFOR
%token ARRAY OF
%token BEG END ASG
%token EQ NEQ LT LEQ AND OR TRUE FALSE
%token ELSE
%token WHILE FOR
%token <token> ID ICONST

%type <targetReg> exp
%type <targetReg> lhs
%type <token> idlist
%type <token> type
%type <token> stype
%type <targetReg> ctrlexp
%type <targetReg> condexp
%type <targetReg> ifhead
%type <targetReg> ifstmt
%type <targetReg> stmt
%type <targetReg> astmt
%type <loopctrl> WHILE

%start program

%nonassoc EQ NEQ LT LEQ GT GEQ
%left '+' '-' AND
%left '*' OR

%nonassoc THEN
%nonassoc ELSE

%{
/* Helper functions */
void dereference_arr(regInfo *res, tokentype *id, regInfo *index) {
  int indexReg;
  int newReg;
  int tmpReg;
  int tmpReg2;
  SymTabEntry *var;

  tmpReg = NextRegister();
  tmpReg2 = NextRegister();
  newReg = NextRegister();

  indexReg = index->targetRegister;
  res->targetRegister = newReg;

  if ( (var = lookup(id->str)) == NULL) {
    printf(ERR_VARIABLE_NOT_DECLARED, id->str);
    return;
  }

  res->type = var->type;

  if ( index->type != TYPE_INT ) {
    printf(ERR_INDEX_NOT_INTEGER);
    return;
  }

  emit(NOLABEL, LOADI, 4, tmpReg, EMPTY);
  emit(NOLABEL, MULT, tmpReg, indexReg, tmpReg2);
  emit(NOLABEL, ADDI, tmpReg2, var->offset, newReg);
}

void format_comment(const char *format, ...) {
  va_list args;
  char *comment_str;

  va_start(args, format);
  comment_str = malloc(sizeof(char) * 512);
  vsprintf(comment_str, format, args);
  emitComment(comment_str);
  free(comment_str);
  va_end(args);
}
%}

%%
program : {
            emitComment("Assign STATIC_AREA_ADDRESS to register \"r0\"");
            emit(NOLABEL, LOADI, STATIC_AREA_ADDRESS, 0, EMPTY);
          }
          PROG ID ';' block PERIOD { }
        ;

block : variables cmpdstmt { }
      ;

variables: /* empty */
         | VAR vardcls { }
  ;

vardcls : vardcls vardcl ';' { }
        | vardcl ';' { }
        | error ';' { yyerror("***Error: illegal variable declaration\n");}
        ;

vardcl : idlist ':' type {
          char *var_name;

          while ( (var_name = ldequeue(&$1.dequeue)) != NULL )
            insert(var_name, $3.num, NextOffset(1 * $3.sz), $3.cl, $3.sz);
       }
       ;

idlist : idlist ',' ID {
          char* str;
          initDequeue(&($$.dequeue));
          while ( (str = rdequeue(&$1.dequeue)) != NULL)
            linsert(&$$.dequeue, str);
          rinsert(&$$.dequeue, $3.str);
       }
       | ID  {
          initDequeue(&($$.dequeue));
          linsert(&$$.dequeue, $1.str);
       }
  ;


type : ARRAY '[' ICONST ']' OF stype {
        $$.num = $6.num;
        $$.cl = CL_ARR;
        $$.sz = $3.num;
     }
     | stype {
        $$.num = $1.num;
        $$.cl = CL_SCALAR;
        $$.sz = 1;
     }
     ;

stype : INT { $$.num = TYPE_INT; }
      | BOOL { $$.num = TYPE_BOOL; }
      ;

stmtlist : stmtlist ';' stmt { }
         | stmt { }
         | error { yyerror("***Error: ';' expected or illegal statement \n");}
         ;

stmt    : ifstmt { }
        | fstmt { }
        | wstmt { }
        | astmt {
          $$.canVector = $1.canVector;
          $$.lhs = $1.lhs;
          $$.rhs = $1.rhs;
        }
        | writestmt { }
        | cmpdstmt { }
        ;

cmpdstmt: BEG stmtlist END { }
        ;

ifstmt : ifhead
         THEN {
            emit($1.controlLabel, NOP, EMPTY, EMPTY, EMPTY);
            // Warning: HaX
            $1.controlLabel = NextLabel();
         }
         stmt
         ELSE {
            emit(NOLABEL, BR, $1.controlLabel, EMPTY, EMPTY);
            emit($1.endLabel, NOP, EMPTY, EMPTY, EMPTY);
         }
         stmt {
            emit($1.controlLabel, NOP, EMPTY, EMPTY, EMPTY);
         }
         FI
  ;

ifhead : IF condexp {
          int ifLabel = NextLabel();
          int elseLabel = NextLabel();

          $$.controlLabel = ifLabel;
          $$.endLabel = elseLabel;

          emit(NOLABEL, CBR, $2.targetRegister, ifLabel, elseLabel);
       }
       ;

writestmt : PRINT '(' exp ')' {
            int newOffset = NextOffset(1); /* call generates byte address */
            emitComment("Code for \"PRINT\"");
            emit(NOLABEL, STOREAI, $3.targetRegister, 0, newOffset);
            emit(NOLABEL,
                 OUTPUT,
                 STATIC_AREA_ADDRESS + newOffset,
                 EMPTY,
                 EMPTY);
          }
          ;

fstmt : FOR ctrlexp DO
        stmt {
          // go back to start of loop
          emit(NOLABEL, BR, $2.controlLabel, EMPTY, EMPTY);

          int canVector = 0;
          // same variable on both sides?
          char *str;
          if ($4.rhs != NULL) {

            /* printf("\nlhs varname: %s\n", $4.lhs->varName); */
            /* printf("rhs: 0x%x\n", $4.rhs); */
            /* printf("rhs dequeue left: 0x%x\n", $4.rhs->vars.left); */
            /* printf("rhs dequeue left str: %s", (char*)$4.rhs->vars.left->data); */

            canVector = 1;
            while ( (str = rdequeue(&($4.rhs->vars))) != NULL ) {
              /* printf("\n checking %s against %s\n", str, $4.lhs->varName); */
              if ( strcmp(str, $4.lhs->varName) == 0) {
                /* printf("Found possible dependency on %s\n", str); */
                canVector = 0;
                break;
              }
            }

            if (!$4.lhs->deps.is_arr) {
              emitFoundOutputDependence($4.lhs->varName);
            }
            else if (!canVector) {
              struct arrDeps *rhsDeps;
              int c1 = $4.lhs->deps.c;
              int a = $4.lhs->deps.a;
              double d;
              if (! $4.rhs->deps.has_a)
                a = 1;
              canVector = 1;
              while ( (rhsDeps = rdequeue(&$4.rhs->arrExprs)) != NULL ) {
                if (rhsDeps->is_constant) {
                  // ZIV
                  int c2 = rhsDeps->c;
                  /* printf("\nZIV with c1=%d c2=%d\n", c1, c2); */
                  if ( c2 - c1 >= $2.startRange && c2 - c1 <= $2.endRange ) {
                    emitAssumeTrueDependence($4.lhs->varName);
                    canVector = 0;
                  }
                } else {
                  // SIV
                  int c2 = rhsDeps->c;
                  if (rhsDeps->a != a) {
                    canVector = 0;
                  }

                  d = ((double)(c1 - c2)) / (double)a;
                  /* printf("\nSIV with c1=%d c2=%d, a=%d, d=%f\n", c1, c2, a, d); */
                  if (d == (int)d && d <= $2.endRange - $2.startRange) {
                    // dependence with distance d
                    emitFoundTrueDependenceWithDistance($4.lhs->varName, d);
                    canVector = 0;
                  }
                }
              }
              }
            }
            if (canVector) {
              emitFoundNoDependenciesAndWillVectorize();
            } else {
              emitFoundDependenciesAndWillNotVectorize();
            }
            }
            /* printf("\ncanVector: %d\n", canVector); */
          } else {
            printf("rhs is null\n");
          }

          // turn vectorization on or off
          emit($2.vectLabel, NOP, EMPTY, EMPTY, EMPTY);
          if (canVector) {
            emit(NOLABEL, VECTON, EMPTY, EMPTY, EMPTY);
          }
          emit(NOLABEL, BR, $2.controlLabel, EMPTY, EMPTY);

          // we're done here
          emit($2.endLabel, VECTOFF, EMPTY, EMPTY, EMPTY);
        }
        ENDFOR
  ;

wstmt : WHILE {
          $1.controlLabel = NextLabel();
          $1.bodyLabel = NextLabel();
          $1.postLabel = NextLabel();
          emit($1.controlLabel, NOP, EMPTY, EMPTY, EMPTY);
        } condexp {
          emit(NOLABEL, CBR, $3.targetRegister, $1.bodyLabel, $1.postLabel);
          emit($1.bodyLabel, NOP, EMPTY, EMPTY, EMPTY);
        }
        DO stmt {
          emit(NOLABEL, BR, $1.controlLabel, EMPTY, EMPTY);
          emit($1.postLabel, NOP, EMPTY, EMPTY, EMPTY);
        }
        ENDWHILE
  ;


astmt : lhs ASG exp {
        if ( $1.type != $3.type ) {
          printf(ERR_ASSIGNMENT_TYPES_MATCH);
        }

        emit(NOLABEL,
             STORE,
             $3.targetRegister,
             $1.targetRegister,
             EMPTY
        );

        $$.lhs = &$1;
        $$.rhs = &$3;
      }
      ;

lhs : ID {
      SymTabEntry *var;
      int newReg1 = NextRegister();
      int newReg2 = NextRegister();

      $$.targetRegister = newReg2;

      if ( (var = lookup($1.str)) == NULL) {
        printf(ERR_VARIABLE_NOT_DECLARED, $1.str);
        return;
      }
      if ( var->cl != CL_SCALAR ) {
        printf(ERR_VARIABLE_NOT_SCALAR, $1.str);
      }

      strcpy($$.varName, $1.str);

      $$.type = var->type;
      $$.deps.has_c = 0;

      /* format_comment("Loading %s's offset into r%d", var->name, newReg2); */
      emit(NOLABEL, LOADI, var->offset, newReg1, EMPTY);
      emit(NOLABEL, ADD, 0, newReg1, newReg2);
    }
    |  ID '[' exp ']' {
      SymTabEntry *var = lookup($1.str);
      if (var == NULL) {
        printf(ERR_VARIABLE_NOT_DECLARED, $1.str);
        return;
      }
      if (var->cl != CL_ARR) {
        printf(ERR_VARIABLE_NOT_ARRAY, $1.str);
      }

      strcpy($$.varName, $1.str);

      memcpy(&$$.deps, &$3.deps, sizeof(struct arrDeps));
      $$.deps.is_arr = 1;

      format_comment("Load LHS value of array variable \"%s\" with based address %d",
                      $1.str, var->offset);
      dereference_arr(&$$, &$1, &$3);
    }
    ;


exp : exp '+' exp {
      int newReg = NextRegister();

      if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
        printf(ERR_OPERAND_NOT_INTEGER);
      }

      $$.type = $1.type;
      $$.targetRegister = newReg;
      emit(NOLABEL,
           ADD,
           $1.targetRegister,
           $3.targetRegister,
           newReg
      );

      if ( $1.deps.is_arr || $3.deps.is_arr) {
        initDequeue(&$$.arrExprs);
        if ( $1.deps.is_arr ) {
          linsert(&$$.arrExprs, &$1.deps);
        }
        if ( $3.deps.is_arr ) {
          linsert(&$$.arrExprs, &$3.deps);
        }
      }
      else if ( (*$1.deps.iName != '\0' && *$3.deps.iName == '\0') ) {
        strcpy($$.deps.iName, $1.deps.iName);
        $$.deps.has_a = $1.deps.has_a;
        $$.deps.a = $1.deps.a;
        $$.deps.c = $3.deps.c;
        $3.deps.has_c = $3.deps.has_c;
      } else if ( (*$1.deps.iName == '\0' && *$3.deps.iName != '\0') ) {
        strcpy($$.deps.iName, $3.deps.iName);
        $$.deps.has_a = $3.deps.has_a;
        $$.deps.a = $3.deps.a;
        $$.deps.c = $1.deps.c;
        $1.deps.has_c = $1.deps.has_c;
      }

      /* printf("\n\na:%d c: %d\n\n", $3.deps.a, $3.deps.c); */

      initDequeue(&$$.vars);
      char *str;
      while( (str = rdequeue(&$1.vars) ) != NULL )
        linsert(&$$.vars, str);
      while( (str = rdequeue(&$3.vars) ) != NULL )
        linsert(&$$.vars, str);
    }
    | exp '-' exp {
      int newReg = NextRegister();

      if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
        printf(ERR_OPERAND_NOT_INTEGER);
      }

      $$.type = $1.type;
      $$.targetRegister = newReg;
      emit(NOLABEL,
           SUB,
           $1.targetRegister,
           $3.targetRegister,
           newReg
      );

      initDequeue(&$$.vars);
      char *str;
      APPEND_DEQ(str, &$$.vars, &$1.vars);
      APPEND_DEQ(str, &$$.vars, &$3.vars);

      if ( $1.deps.is_arr || $3.deps.is_arr) {
        initDequeue(&$$.arrExprs);
        if ( $1.deps.is_arr ) {
          linsert(&$$.arrExprs, &$1.deps);
        }
        if ( $3.deps.is_arr ) {
          linsert(&$$.arrExprs, &$3.deps);
        }
      }

      $$.deps.is_constant = $1.deps.is_constant && $3.deps.is_constant;
      $$.deps.has_c = $1.deps.has_c + $3.deps.has_c;
      if (!$3.deps.is_arr)
        $$.deps.c = - $3.deps.c;
      strcpy($$.deps.iName, $1.deps.iName);
    }
    | exp '*' exp {
      int newReg = NextRegister();

      if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
        printf(ERR_OPERAND_NOT_INTEGER);
      }

      $$.type = $1.type;
      $$.targetRegister = newReg;
      emit(NOLABEL,
           MULT,
           $1.targetRegister,
           $3.targetRegister,
           newReg
      );

      initDequeue(&$$.vars);
      char *str;
      APPEND_DEQ(str, &$$.vars, &$1.vars);
      APPEND_DEQ(str, &$$.vars, &$3.vars);

      if ( $1.deps.is_arr || $3.deps.is_arr) {
        initDequeue(&$$.arrExprs);
        if ( $1.deps.is_arr ) {
          linsert(&$$.arrExprs, &$1.deps);
        }
        if ( $3.deps.is_arr ) {
          linsert(&$$.arrExprs, &$3.deps);
        }
      }

      $$.deps.is_constant = $1.deps.is_constant && $3.deps.is_constant;
      if ($1.deps.is_constant && !$3.deps.is_constant) {
        $$.deps.has_a = 1;
        $$.deps.a = $1.deps.c;
        $$.deps.has_c = 0;
        $$.deps.c = 0;
        strcpy($$.deps.iName, $3.deps.iName);
      } else if (!$1.deps.is_constant && $3.deps.is_constant) {
        $$.deps.has_a = 1;
        $$.deps.a = $3.deps.c;
        $$.deps.has_c = 0;
        $$.deps.c = 0;
        strcpy($$.deps.iName, $1.deps.iName);
      }

      /* printf("\n\na: %d, c: %d, iName: %s\n\n", $$.deps.a, $$.deps.c, $$.deps.iName); */
    }

    | exp AND exp {
      int newReg = NextRegister();

      if (! (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL))) {
        printf(ERR_OPERAND_NOT_BOOLEAN);
      }

      $$.type = $1.type;
      $$.targetRegister = newReg;
      emit(NOLABEL,
           AND_INSTR,
           $1.targetRegister,
           $3.targetRegister,
           newReg
      );

      initDequeue(&$$.vars);
      char *str;
      while( (str = rdequeue(&$1.vars) ) != NULL )
        linsert(&$$.vars, str);
      while( (str = rdequeue(&$3.vars) ) != NULL )
        linsert(&$$.vars, str);
    }
    | exp OR exp {
      int newReg = NextRegister();

      if (! (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL))) {
        printf(ERR_OPERAND_NOT_BOOLEAN);
      }

      $$.type = $1.type;
      $$.targetRegister = newReg;
      emit(NOLABEL,
           OR_INSTR,
           $1.targetRegister,
           $3.targetRegister,
           newReg
      );

      initDequeue(&$$.vars);
      char *str;
      while( (str = rdequeue(&$1.vars) ) != NULL )
        linsert(&$$.vars, str);
      while( (str = rdequeue(&$3.vars) ) != NULL )
        linsert(&$$.vars, str);
    }


    | ID {
        SymTabEntry *var;
        int newReg = NextRegister();

        if ( (var = lookup($1.str)) == NULL) {
            printf(ERR_VARIABLE_NOT_DECLARED, $1.str);
            return;
        }

        $$.targetRegister = newReg;
        $$.type = var->type;

        strcpy($$.varName, $1.str);
        strcpy($$.deps.iName, $1.str);

        format_comment("Loading %s into register r%d.", $1.str, newReg);
        emit(NOLABEL, LOADAI, 0, var->offset, newReg);

        initDequeue(&$$.vars);
        char *str;
        rinsert(&$$.vars, $1.str);
    }

    | ID '[' exp ']' {
      int newReg;
      SymTabEntry *var = lookup($1.str);
      if (var == NULL) {
        printf(ERR_VARIABLE_NOT_DECLARED, $1.str);
        return;
      }
      if (var->cl != CL_ARR) {
        printf(ERR_VARIABLE_NOT_ARRAY, $1.str);
      }

      strcpy($$.varName, $1.str);

      format_comment("Load RHS value of array variable \"%s\" with based address %d",
                      $1.str, var->offset);
      dereference_arr(&$$, &$1, &$3);
      newReg = NextRegister();
      emit(NOLABEL, LOAD, $$.targetRegister, newReg, EMPTY);
      $$.targetRegister = newReg;

      memcpy(&$$.deps, &$3.deps, sizeof(struct arrDeps));
      strcpy($$.deps.arrName, $1.str);
      $$.deps.is_arr = 1;

      char *str;
      initDequeue(&$$.vars);
      APPEND_DEQ(str, &$$.vars, &$3.vars);
      rinsert(&$$.vars, $1.str);
      initDequeue(&$$.arrExprs);
      struct arrDeps *tempDeps = malloc(sizeof(struct arrDeps));
      memcpy(tempDeps, &$$.deps, sizeof(struct arrDeps));
      rinsert(&$$.arrExprs, tempDeps);
    }



    | ICONST {
      int newReg = NextRegister();
      $$.targetRegister = newReg;
      $$.type = TYPE_INT;
      $$.deps.is_constant = 1;
      $$.deps.has_c = 1;
      $$.deps.c = $1.num;
      emit(NOLABEL, LOADI, $1.num, newReg, EMPTY);
      initDequeue(&$$.vars);
    }

    | TRUE {
      int newReg = NextRegister(); /* TRUE is encoded as value '1' */
      $$.targetRegister = newReg;
      $$.type = TYPE_BOOL;
      emit(NOLABEL, LOADI, 1, newReg, EMPTY);
      initDequeue(&$$.vars);
    }
    | FALSE {
      int newReg = NextRegister(); /* TRUE is encoded as value '0' */
      $$.targetRegister = newReg;
      $$.type = TYPE_BOOL;
      emit(NOLABEL, LOADI, 0, newReg, EMPTY);
      initDequeue(&$$.vars);
    }
    | error { yyerror("***Error: illegal expression\n");}
    ;


ctrlexp : ID ASG ICONST ',' ICONST {
          SymTabEntry *var;
          int tmpReg = NextRegister();
          int newReg = NextRegister();
          int testReg = NextRegister();
          int controlLabel = NextLabel();
          int bodyLabel = NextLabel();
          int endLabel = NextLabel();
          int vectLabel = NextLabel();

          if( (var = lookup($1.str)) == NULL) {
            printf(ERR_INDUCTION_NOT_DECLARED, $1.str);
            return;
          } else if ( var->type != TYPE_INT ) {
            printf(ERR_INDUCTION_NOT_SCALAR, $1.str);
          }

          $$.controlLabel = controlLabel;
          $$.endLabel = endLabel;
          $$.vectLabel = vectLabel;
          $$.startRange = $3.num;
          $$.endRange = $5.num;

          //Initialize the LCV
          emit(NOLABEL, LOADI, $3.num - 1, tmpReg, EMPTY);
          emit(NOLABEL, STOREAI, tmpReg, 0, var->offset);
          // Jump to the the vector nonsense
          emit(NOLABEL, BR, vectLabel, EMPTY, EMPTY);
          //Retrieve the LCV and test if the loop is over
          emit(controlLabel, LOADAI, 0, var->offset, newReg);
          emit(NOLABEL, ADDI, newReg, 1, newReg);
          emit(NOLABEL, STOREAI, newReg, 0, var->offset);
          emit(NOLABEL, LOADI, $5.num, testReg, EMPTY);
          emit(NOLABEL, CMPLE, newReg, testReg, testReg);
          emit(NOLABEL, CBR, testReg, bodyLabel, endLabel);
          emit(bodyLabel, NOP, EMPTY, EMPTY, EMPTY);
        }
        ;


condexp : exp NEQ exp {
          if ($1.type != $3.type) {
            printf(ERR_EQUALS_WITH_DIFFERENT_TYPES);
          }
          int newReg = NextRegister();
          $$.targetRegister = newReg;
          emit(NOLABEL, CMPNE, $1.targetRegister, $3.targetRegister, newReg);
        }
        | exp EQ exp {
          if ($1.type != $3.type) {
            printf(ERR_EQUALS_WITH_DIFFERENT_TYPES);
          }
          int newReg = NextRegister();
          $$.targetRegister = newReg;
          emit(NOLABEL, CMPEQ, $1.targetRegister, $3.targetRegister, newReg);
        }
        | exp LT exp {
          if ( !($1.type == TYPE_INT && $3.type == TYPE_INT) ) {
            printf(ERR_RELATIONAL_ILLEGAL_TYPE);
          }

          int newReg = NextRegister();
          $$.targetRegister = newReg;
          emit(NOLABEL, CMPLT, $1.targetRegister, $3.targetRegister, newReg);
        }
        | exp LEQ exp {
          if ( !($1.type == TYPE_INT && $3.type == TYPE_INT) ) {
            printf(ERR_RELATIONAL_ILLEGAL_TYPE);
          }

          int newReg = NextRegister();
          $$.targetRegister = newReg;
          emit(NOLABEL, CMPLE, $1.targetRegister, $3.targetRegister, newReg);
        }
        | exp GT exp {
          if ( !($1.type == TYPE_INT && $3.type == TYPE_INT) ) {
            printf(ERR_RELATIONAL_ILLEGAL_TYPE);
          }

          int newReg = NextRegister();
          $$.targetRegister = newReg;
          emit(NOLABEL, CMPGT, $1.targetRegister, $3.targetRegister, newReg);
        }
        | exp GEQ exp {
          if ( !($1.type == TYPE_INT && $3.type == TYPE_INT) ) {
            printf(ERR_RELATIONAL_ILLEGAL_TYPE);
          }

          int newReg = NextRegister();
          $$.targetRegister = newReg;
          emit(NOLABEL, CMPGE, $1.targetRegister, $3.targetRegister, newReg);
        }
        | error { yyerror("***Error: illegal conditional expression\n");}
        ;

%%

void yyerror(char* s) {
        fprintf(stderr,"%s\n",s);
        }

int optimize_flag = 0;


int
main(int argc, char* argv[]) {

  printf("\n     CS415 Spring 2015 Vectorizing Compiler\n\n");

  outfile = fopen("iloc.out", "w");
    if (outfile == NULL) {
      printf("ERROR: cannot open output file \"iloc.out\".\n");
      return -1;
    }

  depstats = fopen("stats.log", "w");
  if (depstats == NULL) {
    printf("ERROR: cannot open output file \"stats.log\".\n");
    return -1;
  }

  CommentBuffer = (char *) malloc(500);
  InitSymbolTable();

  printf("1\t");
  yyparse();
  printf("\n");

  PrintSymbolTable();

  fclose(outfile);
  fclose(depstats);

  return 0;
}
