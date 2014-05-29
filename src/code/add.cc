
#include <iostream>
#include <sstream>
#include <string>

#include "config.hh"
#include "code.hh"



using namespace code::cmd;

std::string
code::add(const ISymbolTable::Entry& a, const ISymbolTable::Entry& b)
{
	std::ostringstream machine_code;

	bool aIsZero = a.has_value and (std::int32_t) a.value.find_first_not_of('0') == -1,
			bIsZero = b.has_value and (std::int32_t) b.value.find_first_not_of('0') == -1;
	bool aIsOne = a.has_value and a.value == "1", bIsOne = b.has_value and b.value == "1";
	if (F_ADD_ZERO and (aIsZero or bIsZero))
	{
	    // optymalizacja: dodawanie zera
		std::cerr << ">> optymalizacja: +0\n";

		const ISymbolTable::Entry *sym = not(aIsZero) ? &a : &b;
		machine_code << LOAD << " " << sym->current_addr << "\n";
	}
	else if (F_ADD_ONE and (aIsOne or bIsOne))
	{
		// optymalizacja: dodwananie jedynki
		std::cerr << ">> optymalizacja: +1\n";

		// right - jedynka
		const ISymbolTable::Entry *left = &a, *right = &b;
		if (aIsOne)
		{
			left = &b;
			right = &a;
		}

		machine_code
				<< LOAD << " " << left->current_addr << "\n"
				<< INC << "\n";
	}
	else
	{
		machine_code
				<< LOAD << " " << a.current_addr << "\n"
				<< ADD << " " << b.current_addr << "\n";
	}

	return machine_code.str();
}

