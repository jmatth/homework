%{
#include <stdio.h>
#include "attr.h"
#include "memory.h"
int yylex();
void yyerror(char * s);
%}

%union {tokentype token; 
        Value value;
       }

%token PROG PERIOD BEG END ASG PRINT
%token <token> ID ICONST

%start program

%left '+' '-'
%left '*' 


%%
program : PROG ID ';' block PERIOD { printf("\n\n  Done with interpretation of program \"%s\"\n", $2.str);}
	;

block	: cmpdstmt  { }
	;

cmpdstmt: BEG stmtlist END   { }
	;

stmtlist : stmtlist ';' stmt { } 
         | stmt        { }        
         | error ';' { yyerror("***Error: illegal stmt\n");}  
	; 

stmt    : ID ASG exp       {  }  
        | PRINT '(' ID ')' { printf("\n\n     --> value of variable \"%s\" is %d\n", $3.str, -1)  ;} 
	;

exp	: exp '+' exp		{  }
        | exp '-' exp		{  } 
	| exp '*' exp		{  } 
        | ID			{  }
        | ICONST                {  }   
        | '(' exp ')' 	       	{  }
	;

%%

void yyerror(char* s) {
        fprintf(stderr,"%s\n",s);
        }

int
main() {
  printf("\n              CS415 Homework 6 Interpreter\n\n");
  InitMemory();
  printf("1\t");
  yyparse();
  MemoryDump();
  
  return 1;
}

