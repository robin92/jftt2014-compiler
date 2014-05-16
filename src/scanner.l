
%option noyywrap
%option yylineno

%{
    #include <iostream>
    #include <string>
    #include <sstream>
  
    #include <cstdio>

    #include "parser.tab.hh"
%}

NUMBER	[0-9]+
ID		[_a-z]+

%x comment

%%

"(*"	{ BEGIN(comment); }

"CONST"	{ return CONST; }

"VAR"	{ return VAR; }

"BEGIN"	{ return PBEGIN; }

"IF"	{ return IF; }
		
"THEN"	{ return THEN; }

"ELSE"	{ return ELSE; }
		
"WHILE"	{ return WHILE; }

"DO"	{ return DO; }

"END"	{ return END; }

"READ"	{ return READ; }

"WRITE"	{ return WRITE; }

{ID}	{ yylval.id = strdup(yytext); return IDENTIFIER; }

{NUMBER}	{ yylval.bignum = strdup(yytext); return NUM; }

"="		{ return OPERATOR_CONST_ASSIGNMENT; }

":="	{ return OPERATOR_ASSIGNMENT; }

"=="	{ return OPERATOR_EQ; }

"!="	{ return OPERATOR_NE; }

"<"	{ return OPERATOR_LT; }

">"	{ return OPERATOR_GT; }

"<="	{ return OPERATOR_LE; }

">="	{ return OPERATOR_GE; }

"+"	{ return OPERATOR_ADD; }

"-"	{ return OPERATOR_SUBTRACT; }

"*"	{ return OPERATOR_MULTIPLY; }

"/"	{ return OPERATOR_DIVIDE; }

"%"	{ return OPERATOR_MODULO; }

";"	{ return SEMICOLON; }

"\n"	{}	// Flex zlicza linie dzięki odpowiedniej opcji

.		{}

<comment>{
	"*)"	{ BEGIN(INITIAL); }
	
	"\n"	{ }
	
	.		{ }
}

%%

