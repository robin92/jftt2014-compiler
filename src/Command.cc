
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>

#include "Condition.hh"
#include "Expression.hh"
#include "Command.hh"
#include "Commands.hh"

#include "code/code.hh"



extern
void
yyerror(const std::string&);

static
std::uint32_t
count_lines(const std::string& str);

static
std::int32_t
handle_assignment(
		std::string *out,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset,
		Command* self);

static
std::int32_t
handle_read(
		std::string *out,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset,
		Command* self);

static
std::int32_t
handle_write(
		std::string *out,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset,
		Command* self);

static
std::int32_t
handle_if(
		std::string *out,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset,
		Command* self);

static
std::int32_t
parse_complex_expr(
		std::ostream& machine_code,
		ISymbolTable* symtbl,
		const ISymbolTable::Entry& entry,
		const Expression& expr,
		const std::uint32_t& offset = 0);
		
static inline
std::int32_t parse_commands(
		std::string* out,
		std::uint32_t* length,
		Commands *cmds,
		ISymbolTable *symtbl,
		const std::uint32_t& offset = 0);



std::string
Command::str(Command::Type type)
{
	switch (type)
	{
		case Type::ASSIGNMENT: return "ASSIGNMENT";
		case Type::IF: return "IF";
		case Type::WHILE: return "WHILE";
		case Type::READ: return "READ";
		case Type::WRITE: return "WRITE";
		default: return "UNDEFINED";
	}
	
	return "UNDEFINED";
}



std::int32_t
Command::generate(
		std::ostream& output,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset)
{
	std::int32_t err = 0;
	std::string tmp;

	switch (type)
	{
		case Type::ASSIGNMENT:
			{
				err = handle_assignment(&tmp, length, symtbl, offset, this);
			}
			break;

		case Type::READ:
			{
				err = handle_read(&tmp, length, symtbl, offset, this);
			}
			break;

		case Type::WRITE:
			{
				err = handle_write(&tmp, length, symtbl, offset, this);	
			}
			break;

		case Type::IF:
			{
				err = handle_if(&tmp, length, symtbl, offset, this);
			}
			break;

		case Type::WHILE:
		default:
			break;
	}
	
	output << tmp;
	return err;
}
	
std::int32_t
Command::operator() (
		std::ostream& output,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset)
{
	return this->generate(output, length, symtbl, offset);
}



std::uint32_t
count_lines(const std::string& str)
{
	return (std::uint32_t) std::count(str.begin(), str.end(), '\n');
}

std::int32_t
handle_assignment(
		std::string *out,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset,
		Command* self)
{
	std::ostringstream machine_code;
	ISymbolTable::Entry entry = symtbl->get(self->identifier);
	
	switch (self->expr->type)
	{
		// przypisywanie zmiennej wartości numerycznej
		case Expression::Type::NUMBER:
			{
				machine_code
					<< code::generate_number(self->expr->number)
					<< code::cmd::STORE << " " << entry.current_addr << "\n";
			}
			break;

		// przypisywanie zmiennej wartości stałej lub innej zmiennej
		case Expression::Type::IDENTIFIER:
			{
				ISymbolTable::Entry remoteEntry = symtbl->get(self->expr->identifier);
				machine_code
						<< code::copy_value(entry, remoteEntry);	
			}
			break;

			// TODO
			// przypisywanie zmiennej wartości wyrażenia arytmetycznego
			case Expression::Type::COMPLEX:
				if (parse_complex_expr(machine_code, symtbl, entry, *self->expr, offset) != 0) return 4;
				break;

			default:
				break;
	}
	
	*out = machine_code.str();
	*length = count_lines(*out);
	return 0;
}

std::int32_t
handle_read(
		std::string *out,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset,
		Command* self)
{
	std::ostringstream machine_code;

	ISymbolTable::Entry entry = symtbl->get(self->identifier);
	
	machine_code
			<< code::cmd::SCAN << " " << entry.current_addr << "\n";			

	*out = machine_code.str();
	*length = count_lines(*out);
	return 0;
}

std::int32_t
handle_write(
		std::string *out,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset,
		Command* self)
{
	std::ostringstream machine_code;

	ISymbolTable::Entry entry = symtbl->get(self->identifier);
	machine_code
			<< code::cmd::PRINT << " " << entry.current_addr << "\n";			

	*out = machine_code.str();
	*length = count_lines(*out);
	return 0;
}

std::int32_t
handle_if(
		std::string *out,
		std::uint32_t* length,
		ISymbolTable* symtbl,
		const std::uint32_t& offset,
		Command* self)
{
	std::ostringstream machine_code;

	ISymbolTable::Entry first = symtbl->get(self->cond->ids.first),
			second = symtbl->get(self->cond->ids.second);

	switch (self->cond->type)
	{
		case Condition::Type::EQ:
			machine_code << code::compare_eq(first, second, offset);
			break;
		
		case Condition::Type::NE:
		case Condition::Type::LT:
		case Condition::Type::GT:
		case Condition::Type::LE:
		case Condition::Type::GE:
		default:
			break;
	}

	// TODO: wymaga rzetelnych testów
	std::string thenClause, elseClause;
	std::uint32_t condLen = count_lines(machine_code.str());
	std::uint32_t thenLen = 0, elseLen = 0;
	std::int32_t thenErr = parse_commands(&thenClause, &thenLen, self->thencmds, symtbl, offset + condLen + 1),	
			elseErr = parse_commands(&elseClause, &elseLen, self->elsecmds, symtbl, offset + condLen + thenLen + 2);

	machine_code
//			<< code::cmd::JZ << " " << "@next_else_clause" << "\n"
//			<< "[THEN CLAUSE]\n"
			<< code::cmd::JZ << " " << condLen + offset + thenLen + 2 << "\n"
			<< thenClause
//			<< code::cmd::JUMP << " " << "@after_next_else" << "\n"
//			<< "[ELSE CLAUSE]\n";
			<< code::cmd::JUMP << " " << offset + condLen + 1 + thenLen + 1 + elseLen << "\n"
			<< elseClause;

	*out = machine_code.str();
	*length = count_lines(*out);
	return 0;
}

std::int32_t
parse_complex_expr(
		std::ostream& machine_code,
		ISymbolTable* symtbl,
		const ISymbolTable::Entry& entry,
		const Expression& expr,
		const std::uint32_t& offset)
{
	std::string firstId = std::get<0>(expr.complex),
			secondId = std::get<1>(expr.complex);

	ISymbolTable::Entry first = symtbl->get(firstId),
		second = symtbl->get(secondId);

	switch (std::get<2>(expr.complex))
	{
		case Expression::Operation::ADD:
			{
				machine_code
						<< code::add(first, second)
						<< code::cmd::STORE << " " << entry.current_addr << "\n";
			}
			break;

		case Expression::Operation::SUBTRACT:
			{
				machine_code
						<< code::subtract(first, second)
						<< code::cmd::STORE << " " << entry.current_addr << "\n";
			}
			break;

		case Expression::Operation::MULTIPLY:
			{
				machine_code
						<< code::multiply(first, second, offset)
						<< code::cmd::STORE << " " << entry.current_addr << "\n";
			}
			break;
		
		// TODO
		case Expression::Operation::DIVIDE:
		case Expression::Operation::MODULO:
		default:
			break;
	}

	return 0;
}

std::int32_t
parse_commands(
		std::string* out,
		std::uint32_t* length,
		Commands *cmds,
		ISymbolTable *symtbl,
		const std::uint32_t& offset)
{
	std::ostringstream oss;

	std::uint32_t totalLength = 0;
	for (Command *cmd : cmds->cmds)
	{
		std::uint32_t tmp = 0;
//		std::cerr << ">> command: " << Command::str(cmd->type) << "\n";

		if ((*cmd)(oss, &tmp, symtbl, offset + totalLength) != 0) return 1;
		totalLength += tmp;
	}

	*length = totalLength;
	*out = oss.str();

	return 0;
}

