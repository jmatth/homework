%{
#include <stdio.h>
#include "attr.h"
int yylex();
void yyerror(char * s);
%}

%union {tokentype token;
        multCounter multCnt;
       }

/* There are a few redundant (not necessary) tokens here. */

%token PROG PERIOD VAR ARRAY RANGE OF
%token INT WRITELN THEN ELSE IF
%token BEG END ASG
%token EQ NEQ LT LEQ OR EXOR AND NOT TRUE FALSE
%token <token> ID ICONST

%type <multCnt> exp astmt stmtlist cmpdstmt

%start program

%nonassoc EQ NEQ LT LEQ
%left '+' '-' OR EXOR
%left '*' AND
%right NOT

%nonassoc THEN
%nonassoc ELSE

%%
program : PROG ID ';' block PERIOD { printf("\n\n     Done with compiling program \"%s\"\n", $2.str);}
        ;

block	: cmpdstmt  { printf( "\n\n  >>> Program has %d multiplications\n", $1.mcount); }
      ;

cmpdstmt: BEG stmtlist END   {$$.mcount = $2.mcount;}
        ;

stmtlist : stmtlist ';' astmt {$$.mcount = $1.mcount + $3.mcount;}
         | astmt        {$$.mcount = $1.mcount;}
         | error ';' { yyerror("***Error: illegal stmt\n");}
  ;

astmt : var ASG exp             {$$.mcount = $3.mcount; }
      ;

exp	: exp '+' exp		{$$.mcount = $1.mcount + $3.mcount; }
    | exp '-' exp		{$$.mcount = $1.mcount + $3.mcount; }
  | exp '*' exp		{$$.mcount = $1.mcount + $3.mcount + 1; }
        | ID			{$$.mcount = 0; }
        | ICONST                {$$.mcount = 0; }
  ;

var	: ID			{ }
    ;

%%

void yyerror(char* s) {
        fprintf(stderr,"%s\n",s);
        }

int
main() {
  printf("\n              CS415 Front-End Compiler Example\n");
  printf("      Counting Multiplication Operators, Spring 2015\n\n");
  printf("1\t");
  yyparse();
  return 1;
}

