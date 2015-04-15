%{
#include <stdio.h>
#include "attr.h"
#include "instrutil.h"
int yylex();
void yyerror(char * s);
#include "symtab.h"
#include "dequeue.h"
#include <string.h>

FILE *outfile;
char *CommentBuffer;

%}

%union {
  tokentype token;
  regInfo targetReg;
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

%start program

%nonassoc EQ NEQ LT LEQ GT GEQ
%left '+' '-' AND
%left '*' OR

%nonassoc THEN
%nonassoc ELSE

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
            insert(var_name, $3.num, NextOffset(1));
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


type : ARRAY '[' ICONST ']' OF stype {  }

     | stype { $$.num = $1.num; }
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
        | astmt { }
        | writestmt { }
        | cmpdstmt { }
        ;

cmpdstmt: BEG stmtlist END { }
        ;

ifstmt :  ifhead
       THEN stmt
      ELSE
          stmt
          FI
  ;

ifhead : IF condexp {  }
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
      stmt {  }
          ENDFOR
  ;

wstmt : WHILE  {  }
      condexp {  }
          DO stmt  {  }
          ENDWHILE
  ;


astmt : lhs ASG exp {
        if (! ((($1.type == TYPE_INT) && ($3.type == TYPE_INT)) ||
              (($1.type == TYPE_BOOL) && ($3.type == TYPE_BOOL)))) {
          printf("*** ERROR ***: Assignment types do not match.\n");
        }

        emit(NOLABEL,
             STORE,
             $3.targetRegister,
             $1.targetRegister,
             EMPTY
        );
      }
      ;

lhs : ID { /* BOGUS  - needs to be fixed */
      SymTabEntry *var;
      char *commentString = malloc(sizeof(char) * 512);
      int newReg1 = NextRegister();
      int newReg2 = NextRegister();

      $$.targetRegister = newReg2;

      if ( (var = lookup($1.str)) == NULL) {
        printf("*** ERROR ***: Variable %s used but not defined.\n", $1.str);
        return;
      }

      $$.type = var->type;

      sprintf(commentString, "Loading %s's offset into r%d", var->name, newReg2);
      emitComment(commentString);
      emit(NOLABEL, LOADI, var->offset, newReg1, EMPTY);
      emit(NOLABEL, ADD, 0, newReg1, newReg2);
      free(commentString);
    }
    |  ID '[' exp ']' {  }
    ;


exp : exp '+' exp {
      int newReg = NextRegister();

      if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
        printf("*** ERROR ***: Operator types must be integer.\n");
      }

      $$.type = $1.type;
      $$.targetRegister = newReg;
      emit(NOLABEL,
           ADD,
           $1.targetRegister,
           $3.targetRegister,
           newReg
      );
    }
    | exp '-' exp {
      int newReg = NextRegister();

      if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
        printf("*** ERROR ***: Operator types must be integer.\n");
      }

      $$.type = $1.type;
      $$.targetRegister = newReg;
      emit(NOLABEL,
           SUB,
           $1.targetRegister,
           $3.targetRegister,
           newReg
      );
    }
    | exp '*' exp {
      int newReg = NextRegister();

      if (! (($1.type == TYPE_INT) && ($3.type == TYPE_INT))) {
        printf("*** ERROR ***: Operator types must be integer.\n");
      }

      $$.type = $1.type;
      $$.targetRegister = newReg;
      emit(NOLABEL,
           MULT,
           $1.targetRegister,
           $3.targetRegister,
           newReg
      );
    }

    | exp AND exp  {  }


    | exp OR exp        {  }


    | ID {
        SymTabEntry *var;
        int newReg = NextRegister();
        char *commentStr = malloc(sizeof(char) * 512);

        if ( (var = lookup($1.str)) == NULL)
            printf("*** ERROR ***: Variable %s used without being defined.\n", $1.str);

        $$.targetRegister = newReg;
        $$.type = TYPE_INT;

        sprintf(commentStr, "Loading %s into register r%d.", $1.str, newReg);
        emitComment(commentStr);
        free(commentStr);

        emit(NOLABEL, LOADAI, 0, var->offset, newReg);

    }

    | ID '[' exp ']' {   }



    | ICONST                 { int newReg = NextRegister();
                             $$.targetRegister = newReg;
           $$.type = TYPE_INT;
           emit(NOLABEL, LOADI, $1.num, newReg, EMPTY); }

    | TRUE                   { int newReg = NextRegister(); /* TRUE is encoded as value '1' */
                             $$.targetRegister = newReg;
           $$.type = TYPE_BOOL;
           emit(NOLABEL, LOADI, 1, newReg, EMPTY); }

    | FALSE                   { int newReg = NextRegister(); /* TRUE is encoded as value '0' */
                             $$.targetRegister = newReg;
           $$.type = TYPE_BOOL;
           emit(NOLABEL, LOADI, 0, newReg, EMPTY); }

    | error { yyerror("***Error: illegal expression\n");}
    ;


ctrlexp : ID ASG ICONST ',' ICONST {   }
        ;


condexp : exp NEQ exp  {  }

| exp EQ exp  {  }

| exp LT exp  {  }

| exp LEQ exp  {  }

| exp GT exp  {  }

| exp GEQ exp  {  }

| error { yyerror("***Error: illegal conditional expression\n");}
        ;

%%

void yyerror(char* s) {
        fprintf(stderr,"%s\n",s);
        }

int optimize_flag = 0;


int
main(int argc, char* argv[]) {

  printf("\n     CS415 Spring 2015 Compiler\n\n");

  outfile = fopen("iloc.out", "w");
    if (outfile == NULL) {
      printf("ERROR: cannot open output file \"iloc.out\".\n");
      return -1;
    }

  CommentBuffer = (char *) malloc(500);
  InitSymbolTable();

  printf("1\t");
  yyparse();
  printf("\n");

  PrintSymbolTable();

  fclose(outfile);

  return 1;
}
