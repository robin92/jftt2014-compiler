
%require  "3.0"

%skeleton "lalr1.cc"

%defines
%define api.value.type {long}
 
%code requires{
    class Scanner;
}
 
%lex-param   { Scanner& scanner }
%parse-param { Scanner& scanner }
 
%code{
    #include <iostream>
    #include <sstream>

    //
    // variables for parsing
    //
    
    std::ostringstream onpNotation;

    //
    // functions used in parser
    //
    
    // wrapper for yylex
    static int yylex(yy::parser::semantic_type *yylval, Scanner& scanner);

	void yyerror(const std::string& str);
}

%token IDENTIFIER
%token NUM
%token ERROR

%right ":="
%left '%'
%left '-' '+'
%left '*' '/'

%%

program:
|	"CONST" cdeclarations "VAR" vdeclarations "BEGIN" commands "END"	{}
;

cdeclarations:
|	cdeclarations IDENTIFIER "=" NUM	{}
|	""
;

vdeclarations:
|	vdeclarations IDENTIFIER	{}
|	""
;

commands:
|	commands command
|	""
;

command:
|	IDENTIFIER ":=" expression ";"
|	"IF" condition "THEN" command "ELSE" command "END"
|	"WHILE" condition "DO" command "END"
;

expression:
|	NUM
|	IDENTIFIER
|	IDENTIFIER "+" IDENTIFIER
|	IDENTIFIER "-" IDENTIFIER
|	IDENTIFIER "*" IDENTIFIER
|	IDENTIFIER "/" IDENTIFIER
|	IDENTIFIER "%" IDENTIFIER
;

condition:
|	"true"
|	"false"
|	IDENTIFIER "==" IDENTIFIER
|	IDENTIFIER "!=" IDENTIFIER
|	IDENTIFIER "<" IDENTIFIER
|	IDENTIFIER ">" IDENTIFIER
|	IDENTIFIER "<=" IDENTIFIER
|	IDENTIFIER ">=" IDENTIFIER
;

%%

void
yy::parser::error(std::string const& error)
{
    std::cerr << "Error: " << error << "\n";
}

/* include for access to scanner.yylex */
#include "scanner.hh"

static
int 
yylex(yy::parser::semantic_type *yylval, Scanner& scanner)
{
   return scanner.yylex(yylval);
}

