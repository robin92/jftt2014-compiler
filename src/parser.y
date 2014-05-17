
%error-verbose

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

	extern
	std::int32_t
	yylex();
	
	extern std::int32_t yylineno;

	//
	// parser objects
	//

	void 
	yyerror(const std::string& str);

	static std::ostringstream machine_code;

	static ISymbolTable *symtbl = new SymbolTable();

	static inline std::uint32_t next_mem_addr();
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
			std::ostringstream oss;
			oss << "identifier '" << $2 << "' already defined";
			yyerror(oss.str());
			return ERROR;
		}

		ISymbolTable::Entry entry;
		entry.has_value = true;
		entry.value = std::string($4);
		entry.current_addr = next_mem_addr();
		
		std::string tmp = code::generate_number(entry.value);
		machine_code
				<< tmp
				<< code::cmd::STORE << " " << entry.current_addr << "\n";

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
			std::ostringstream oss;
			oss << "identifier '" << $2 << "' already defined";
			yyerror(oss.str());
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
|	WRITE IDENTIFIER SEMICOLON {
		fprintf(stderr, ">> wypisywanie wartości\n");
		
		if ( not(symtbl->contains($2)) )
		{
			std::ostringstream oss;
			oss << "identifier '" << $2 << "' has not been declared";
			yyerror(oss.str());
			return ERROR;
		}
		
		ISymbolTable::Entry entry = symtbl->get($2);
		machine_code
			<< code::cmd::PRINT << " " << entry.current_addr << "\n";
	}
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

std::uint32_t
next_mem_addr()
{
	static std::uint32_t addr = 0;
	return addr++;
}

