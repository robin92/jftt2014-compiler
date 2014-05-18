
#include <iostream>
#include <sstream>
#include <string>

#include <gmpxx.h>

#include "code.hh"



using namespace code::cmd;

std::string
code::subtract(const ISymbolTable::Entry& a, const ISymbolTable::Entry& b)
{
	std::ostringstream machine_code;

	// optymalizacja: oba symbole to stałe, jeśli a <= b to a - b <= 0
	// a w maszyna nie obsługuje liczb < 0, koszt wyzerowania jest mniejszy
	// niż koszt odejmowania
	bool done = false;
	if (a.has_value and b.has_value)
	{
		mpz_class an(a.value), bn(b.value);
		if ( an <= bn )
		{
			machine_code
					<< code::cmd::ZERO << "\n";
			done = true;
		}
	}

	if (not(done))
	{
		machine_code
				<< LOAD << " " << a.current_addr << "\n"
				<< SUB << " " << b.current_addr << "\n";
	}

	return machine_code.str();
}

