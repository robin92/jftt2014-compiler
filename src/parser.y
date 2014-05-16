
%{
	#include <iostream>
	#include <sstream>

	#include <cassert>
	#include <cstdio>
%}

%union {
	char *id;
	std::uint64_t const_num;
}
 
%{
	//
	// scanner objects
	//

	extern std::int32_t yylex();
	
	extern std::int32_t yylineno;

	//
	// parser objects
	//

	void yyerror(const std::string& str);
%}

%token <id> IDENTIFIER
%token <const_num> NUM
%token ERROR
%token SEMICOLON

%token CONST
%token VAR
%token PBEGIN
%token END
%token IF
%token THEN
%token ELSE
%token WHILE
%token DO
%token TRUE
%token FALSE
%token READ
%token WRITE

%token OPERATOR_CONST_ASSIGNMENT
%token OPERATOR_ASSIGNMENT
%token OPERATOR_ADD
%token OPERATOR_SUBTRACT
%token OPERATOR_MULTIPLY
%token OPERATOR_DIVIDE
%token OPERATOR_MODULO
%token OPERATOR_EQ
%token OPERATOR_NE
%token OPERATOR_LT
%token OPERATOR_GT
%token OPERATOR_LE
%token OPERATOR_GE

%right ":="
%left '%'
%left '-' '+'
%left '*' '/'

%%

program:
	CONST cdeclarations VAR vdeclarations PBEGIN commands END
;

cdeclarations:
	cdeclarations IDENTIFIER OPERATOR_CONST_ASSIGNMENT NUM
|	%empty
;

vdeclarations:
	vdeclarations IDENTIFIER
|	%empty
;

commands:
	commands command
|	%empty
;

command:
	IDENTIFIER OPERATOR_ASSIGNMENT expression SEMICOLON
|	IF condition THEN commands ELSE commands END
|	WHILE condition DO commands END
|	READ IDENTIFIER SEMICOLON
|	WRITE IDENTIFIER SEMICOLON
;

expression:
	NUM
|	IDENTIFIER
|	IDENTIFIER OPERATOR_ADD IDENTIFIER
|	IDENTIFIER OPERATOR_SUBTRACT IDENTIFIER
|	IDENTIFIER OPERATOR_MULTIPLY IDENTIFIER
|	IDENTIFIER OPERATOR_DIVIDE IDENTIFIER
|	IDENTIFIER OPERATOR_MODULO IDENTIFIER
;

condition:
	IDENTIFIER OPERATOR_EQ IDENTIFIER
|	IDENTIFIER OPERATOR_NE IDENTIFIER
|	IDENTIFIER OPERATOR_LT IDENTIFIER
|	IDENTIFIER OPERATOR_GT IDENTIFIER
|	IDENTIFIER OPERATOR_LE IDENTIFIER
|	IDENTIFIER OPERATOR_GE IDENTIFIER
;

%%

void
yyerror(std::string const& error)
{
	fprintf(stderr, "[%d line] Error: %s\n",
			yylineno,
			error.c_str());
}

