
%{
	#include <iostream>
	#include <sstream>

	#include <cassert>
	#include <cstdio>

	#include "code.hh"
	#include "SymbolTable.hh"
%}

%union {
	char *id;
	char *bignum;
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

	std::ostringstream machine_code;

	ISymbolTable *symtbl = new SymbolTable();
%}

%token <id> IDENTIFIER
%token <bignum> NUM
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
	CONST cdeclarations VAR vdeclarations PBEGIN commands END {
		std::ostringstream oss;		
		for (auto symbol : symtbl->all())
		{
			oss << "(" << *symbol.first << ", ";
			symbol.second->has_value ? oss << symbol.second->value : oss << "null";
			oss << ") ";
		}
	
		std::cerr << ">> symbols: " << oss.str() << "\n";

		std::cout << machine_code.str()
				<< "HALT\n";
	}
;

cdeclarations:
	cdeclarations IDENTIFIER OPERATOR_CONST_ASSIGNMENT NUM	{
		fprintf(stderr, ">> nowa stała: [%s]: %ld\n", $2, $4);
		
		if (symtbl->contains($2)) {
			// TODO: błąd - już zdefiniowano
			return ERROR;
		}

		ISymbolTable::Entry entry;
		entry.has_value = true;
		entry.value = std::string($4);

		std::uint32_t addr = 0;
		std::string tmp = code::generate_number(&addr, entry.value);
		machine_code << tmp;

		symtbl->insert($2);
		symtbl->update($2, entry);
		
		assert(symtbl->contains($2));
		assert(symtbl->get($2).value == std::string($4));
	}
|	%empty
;

vdeclarations:
	vdeclarations IDENTIFIER {
		fprintf(stderr, ">> nowa zmienna: [%s]\n", $2);
		
		if (symtbl->contains($2)) {
			// TODO: błąd - już zdefiniowano
			return ERROR;
		}
		
		symtbl->insert($2);
		
		assert(symtbl->contains($2));
	}
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

