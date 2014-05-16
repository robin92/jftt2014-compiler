
%{
	#include <iostream>
	#include <sstream>

	#include <cassert>
	#include <cstdio>

	#include "SymbolTable.hh"
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

	std::ostringstream machine_code;

	ISymbolTable *symtbl = new SymbolTable();

	namespace code
	{
		std::string
		generate_const(std::uint32_t* addr, const std::string& decnum);
	}
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
		entry.value = std::to_string($4);

		std::uint32_t addr = 0;
		std::string tmp = code::generate_const(&addr, entry.value);
		machine_code << tmp;

		symtbl->insert($2);
		symtbl->update($2, entry);
		
		assert(symtbl->contains($2));
		assert(std::stol(symtbl->get($2).value) == $4);
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

std::string
code::generate_const(std::uint32_t* addr, const std::string& decnum)
{
	// TODO: wsparcie dla dużych liczb (> 64b)
	std::cerr << ">> generowanie kodu dla stałej " << decnum << "\n";

	std::list<std::string> lines;
	std::uint64_t number = std::stol(decnum);
	bool was_one = false;

	lines.push_back("ZERO");
	for (std::int32_t i = 0; i < sizeof(std::uint64_t) << 3; i++)
	{
		if ( (number & 0x8000000000000000) > 0 )
		{
			// bit 1
			was_one = true;
			lines.push_back("INC");
		}
		if ( (i + 1 != sizeof(std::uint64_t) << 3) and was_one) lines.push_back("SHL");

		number <<= 1;
	}

	std::ostringstream code;
	for (std::string& str : lines) code << str << "\n";
	code << "STORE 0\n"
			<< "PRINT 0\n";

	return code.str();
}

