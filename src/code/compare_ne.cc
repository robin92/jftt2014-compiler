
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

std::string
code::compare_ne(
	const ISymbolTable::Entry& a,
	const ISymbolTable::Entry& b,
	const std::uint32_t offset)
{
	std::ostringstream machine_code;
	
	machine_code
			// ładowanie identyfikatorów do rejestrów do obliczeń
			<< LOAD << " " << a.current_addr << "\n"
			<< STORE << " " << 0 << "\n"
			<< LOAD << " " << b.current_addr << "\n"
			<< STORE << " " << 1 << "\n"
			// sprawdzanie warunku
			<< LOAD << " " << 0 << "\n"
			<< SUB << " " << 1 << "\n"
			<< JG << " " << offset + 12 << "\n"
			<< LOAD << " " << 1 << "\n"
			<< SUB << " " << 0 << "\n"
			<< JG << " " << offset + 12 << "\n"
			<< ZERO << "\n"
			<< JUMP << " " << offset + 13 << "\n"
			<< INC << "\n";
						
	return machine_code.str();
}

