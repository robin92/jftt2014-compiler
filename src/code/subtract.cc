
#include <iostream>
#include <sstream>
#include <string>

#include "config.hh"
#include "code.hh"



using namespace code::cmd;

std::string
code::subtract(const ISymbolTable::Entry& a, const ISymbolTable::Entry& b)
{
	std::ostringstream machine_code;

	bool aIsZero = a.has_value and (std::int32_t) a.value.find_first_not_of('0') == -1,
			bIsZero = b.has_value and (std::int32_t) b.value.find_first_not_of('0') == -1;
	bool bIsOne = b.has_value and b.value == "1";
	if (F_SUB_ZERO and (aIsZero or bIsZero))
	{
		// optymalizacja: odejmowanie zera
		std::cerr << ">> optymalizacja: -0\n";

		if (aIsZero) machine_code << ZERO << "\n";					// odejmowanie od zera = 0
		else machine_code << LOAD << " " << a.current_addr << "\n";	// odejmowanie zera
	}
	else if (F_SUB_ONE and bIsOne)
	{
		// optymalizacja: odejmowanie jedynki
		std::cerr << ">> optymalizacja: -1\n";

		machine_code
				<< LOAD << " " << a.current_addr << "\n"
				<< DEC << "\n";
	}
	else
	{
		machine_code
				<< LOAD << " " << a.current_addr << "\n"
				<< SUB << " " << b.current_addr << "\n";
	}

	return machine_code.str();
}

