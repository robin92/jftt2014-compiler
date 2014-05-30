/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <iostream>
#include <sstream>
#include <string>

#include "config.hh"
#include "code.hh"



using namespace code::cmd;

std::string
code::compare_eq(
	const ISymbolTable::Entry& a,
	const ISymbolTable::Entry& b,
	const std::uint32_t offset)
{
	std::ostringstream machine_code;
	
	bool aIsZero = a.has_value and (std::int32_t) a.value.find_first_not_of('0') == -1,
			bIsZero = b.has_value and (std::int32_t) b.value.find_first_not_of('0') == -1;
	if (F_COND_EQ_ZERO and (aIsZero or bIsZero))
	{
		// optymalizacja: == 0
		std::cerr << ">> optymalizacja: == 0\n";
	
		const ISymbolTable::Entry *sym = not(aIsZero) ? &a : &b;
		machine_code
				<< LOAD << " " << sym->current_addr << "\n"
				<< JZ << " " << offset + 4 << "\n"			// 	if sym == 0
				<< ZERO << "\n"								// 		false
				<< JUMP << " " << offset + 5 << "\n"
				<< INC << "\n";								// 		true
	}
	else
	{
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
	}
						
	return machine_code.str();
}

