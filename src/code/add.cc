
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

std::string
code::add(const ISymbolTable::Entry& a, const ISymbolTable::Entry& b)
{
	std::ostringstream machine_code;

	machine_code
			<< LOAD << " " << a.current_addr << "\n"
			<< ADD << " " << b.current_addr << "\n";

	return machine_code.str();
}

