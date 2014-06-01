/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <iostream>
#include <sstream>
#include <string>

#include <gmpxx.h>

#include "config.hh"
#include "code.hh"



using namespace code::cmd;

std::string
code::subtract(const ISymbolTable::Entry& a, const ISymbolTable::Entry& b)
{
	std::ostringstream machine_code;

	if (F_CONST_EXPR and (a.has_value and b.has_value))	// obie stałe
	{
		// optymalizacja: a - b
		std::cerr << ">> optymalizacja: a - b\n";
		mpz_class av(a.value), bv(b.value), res = av - bv;
		if (av <= bv) res = mpz_class("0");
		
		machine_code << generate_number(res.get_str());
		
		return machine_code.str();
	}

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
		if (F_SUB_DEC and b.has_value)
		{
			mpz_class bv(b.value);
			if (bv < 100)
			{
				// optymalizacja: inkremetnacja zamiast dodawania
				std::cerr << ">> optymalizacja: - => --\n";
			
				machine_code << LOAD << " " << a.current_addr << "\n";
				for (std::int32_t i = 0; i < bv; i++) machine_code << DEC << "\n"; 
			
				return machine_code.str();
			}
		}
	
		machine_code
				<< LOAD << " " << a.current_addr << "\n"
				<< SUB << " " << b.current_addr << "\n";
	}

	return machine_code.str();
}

