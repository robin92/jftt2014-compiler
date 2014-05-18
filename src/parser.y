
%error-verbose

%{
	#include <algorithm>
	#include <iostream>
	#include <sstream>

	#include <cassert>
	#include <cstdio>

	#include "SymbolTable.hh"
	#include "code/code.hh"
%}

%code requires {
	#include "Expression.hh"
	#include "Condition.hh"
	#include "Command.hh"
	#include "Commands.hh"
}

%union {
	char *id;
	char *bignum;
	Expression *expr;
	Condition *condition;
	Command *command;
	Commands *commands;
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

	static std::ostringstream machine_code;

	static ISymbolTable *symtbl = new SymbolTable();

	static inline std::uint32_t next_mem_addr();
%}

%type <expr> expression
%type <command> command
%type <commands> commands;
%type <condition> condition;

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
		std::string buf = machine_code.str();
		std::uint32_t totalLength = 0;

		std::cout << buf;		
		totalLength += (std::uint32_t) std::count(buf.begin(), buf.end(), '\n');
		for (Command *cmd : $6->cmds)
		{
			std::uint32_t length = 0;
			std::cerr << ">> command: " << Command::str(cmd->type) << "\n";
		
			if ((*cmd)(std::cout, &length, symtbl, totalLength) != 0) return ERROR;
			totalLength += length;
		}

		std::cout << code::cmd::HALT << "\n";
		totalLength++;

		std::cerr
				<< ">> kompilacja zakończona, program wynikowy ma " << totalLength << " instrukcji\n";
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
		
		ISymbolTable::Entry entry;
		entry.current_addr = next_mem_addr();
		
		symtbl->insert($2);
		symtbl->update($2, entry);
		
		assert(symtbl->contains($2));
	}
|	%empty
;

commands:
	commands command {
		$1->cmds.push_back($2);
	}
|	%empty {
		Commands *commands = new Commands();
		$$ = commands;
	}
;

command:
	IDENTIFIER OPERATOR_ASSIGNMENT expression SEMICOLON {
		Command *command = new Command();
		command->type = Command::Type::ASSIGNMENT;
		command->identifier = $1;
		command->expr = $3;
		$$ = command;
	}
|	IF condition THEN commands ELSE commands END {
		Command *command = new Command();
		command->type = Command::Type::IF;
		$$ = command;
	}
|	WHILE condition DO commands END {
		Command *command = new Command();
		command->type = Command::Type::WHILE;
		$$ = command;
	}
|	READ IDENTIFIER SEMICOLON {
		Command *command = new Command();
		command->type = Command::Type::READ;
		$$ = command;
	}
|	WRITE IDENTIFIER SEMICOLON {
		Command *command = new Command();
		command->type = Command::Type::WRITE;
		command->identifier = $2;
		$$ = command;
	}
;

expression:
	NUM {
		Expression *expr = new Expression();
		expr->type = Expression::Type::NUMBER;
		expr->number = $1;
		$$ = expr;
	}
|	IDENTIFIER {
		Expression *expr = new Expression();
		expr->type = Expression::Type::IDENTIFIER;
		expr->identifier = $1;
		$$ = expr;
	}
|	IDENTIFIER OPERATOR_ADD IDENTIFIER {
		Expression *expr = new Expression();
		expr->type = Expression::Type::COMPLEX;
		expr->complex = std::make_tuple($1, $3, Expression::Operation::ADD);
		$$ = expr;
	}
|	IDENTIFIER OPERATOR_SUBTRACT IDENTIFIER {
		Expression *expr = new Expression();
		expr->type = Expression::Type::COMPLEX;
		expr->complex = std::make_tuple($1, $3, Expression::Operation::SUBTRACT);
		$$ = expr;
	}
|	IDENTIFIER OPERATOR_MULTIPLY IDENTIFIER {
		Expression *expr = new Expression();
		expr->type = Expression::Type::COMPLEX;
		expr->complex = std::make_tuple($1, $3, Expression::Operation::MULTIPLY);
		$$ = expr;
	}
|	IDENTIFIER OPERATOR_DIVIDE IDENTIFIER {
		Expression *expr = new Expression();
		expr->type = Expression::Type::COMPLEX;
		expr->complex = std::make_tuple($1, $3, Expression::Operation::DIVIDE);
		$$ = expr;
	}
|	IDENTIFIER OPERATOR_MODULO IDENTIFIER {
		Expression *expr = new Expression();
		expr->type = Expression::Type::COMPLEX;
		expr->complex = std::make_tuple($1, $3, Expression::Operation::MODULO);
		$$ = expr;
	}
;

condition:
	IDENTIFIER OPERATOR_EQ IDENTIFIER {
		Condition *condition = new Condition();
		condition->type = Condition::Type::EQ;
		$$ = condition;
	}
|	IDENTIFIER OPERATOR_NE IDENTIFIER {
		Condition *condition = new Condition();
		condition->type = Condition::Type::NE;
		$$ = condition;
	}
|	IDENTIFIER OPERATOR_LT IDENTIFIER {
		Condition *condition = new Condition();
		condition->type = Condition::Type::LT;
		$$ = condition;
	}
|	IDENTIFIER OPERATOR_GT IDENTIFIER {
		Condition *condition = new Condition();
		condition->type = Condition::Type::GT;
		$$ = condition;
	}
|	IDENTIFIER OPERATOR_LE IDENTIFIER {
		Condition *condition = new Condition();
		condition->type = Condition::Type::LE;
		$$ = condition;
	}
|	IDENTIFIER OPERATOR_GE IDENTIFIER {
		Condition *condition = new Condition();
		condition->type = Condition::Type::GE;
		$$ = condition;
	}
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
	static std::uint32_t addr = 3;	// pierwsze 3 sa tansze wiec lepsze do obliczen
	return addr++;
}

