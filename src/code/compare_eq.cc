
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



std::string
code::compare_eq(
	const ISymbolTable::Entry& a,
	const ISymbolTable::Entry& b,
	const std::uint32_t offset)
{
	std::ostringstream machine_code;
	
	machine_code
			// ładowanie identyfikatorów do rejestrów do obliczeń
			<< code::cmd::LOAD << " " << a.current_addr << "\n"
			<< code::cmd::STORE << " " << "0" << "\n"
			<< code::cmd::LOAD << " " << b.current_addr << "\n"
			<< code::cmd::STORE << " " << "1" << "\n"
			// sprawdzanie warunku
			<< code::cmd::LOAD << " " << "0" << "\n"
			<< code::cmd::SUB << " " << "1" << "\n"
			<< code::cmd::JG << " " << offset + 12 << "\n"
			<< code::cmd::LOAD << " " << "1" << "\n"
			<< code::cmd::SUB << " " << "0" << "\n"
			<< code::cmd::JG << " " << offset + 12 << "\n"
			<< code::cmd::INC << "\n"
			<< code::cmd::JUMP << " " << offset + 13 << "\n"	// skok o dwie instrukcje
			<< code::cmd::ZERO << "\n";
						
	return machine_code.str();
}

