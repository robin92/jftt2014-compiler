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
code::compare_ne(
	const ISymbolTable::Entry& a,
	const ISymbolTable::Entry& b,
	const std::uint32_t offset)
{
	std::ostringstream machine_code;
	
	bool aIsZero = a.has_value and (std::int32_t) a.value.find_first_not_of('0') == -1,
			bIsZero = b.has_value and (std::int32_t) b.value.find_first_not_of('0') == -1;
	if (F_COND_NE_ZERO and (aIsZero or bIsZero))
	{
		// optymalizacja: != 0
		std::cerr << ">> optymalizacja: != 0\n";
	
		const ISymbolTable::Entry *sym = not(aIsZero) ? &a : &b;
		machine_code
				<< LOAD << " " << sym->current_addr << "\n"
				<< JZ << " " << offset + 3 << "\n"			// 	if sym == 0
				<< JUMP << " " << offset + 4 << "\n"		//	true
				<< ZERO << "\n";							//	false
	}
	else
	{
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
	}
							
	return machine_code.str();
}

