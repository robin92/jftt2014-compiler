/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <gmpxx.h>

#include "config.hh"
#include "code.hh"



static inline
std::string
get_divide_code(std::uint32_t *length, const std::uint32_t& offset = 0);



using namespace code::cmd;

std::string
code::divide(
		const ISymbolTable::Entry& a,
		const ISymbolTable::Entry& b,
		const std::uint32_t offset)
{
	std::ostringstream machine_code;

	if (F_CONST_EXPR and (a.has_value and b.has_value))	// obie stałe
	{
		// optymalizacja: a / b
		std::cerr << ">> optymalizacja: a / b\n";
		mpz_class av(a.value), bv(b.value), res = av / bv;
		if (av < bv or av == mpz_class("0") or bv == mpz_class("0")) res = mpz_class("0");
		
		machine_code << generate_number(res.get_str());
		
		return machine_code.str();
	}	

	std::uint64_t power = 0;
	if (F_DIVIDE_BY_TWO_POWERS and
			(b.has_value and helper::is_two_power(&power, b.value)))	// dzielnk jest potęgą dwójki
	{
		// optymalizacja: dzielenie przez potęgę dwójki
		std::cerr
				<< ">> optymalizacja: dzielenie przez 2^i, b = "
				<< b.value << ", i = " << power << "\n";

		machine_code << LOAD << " " << a.current_addr << "\n";
		for (std::uint64_t i = 0; i < power; i++) machine_code << SHR << "\n";
	}
	else if (F_DIVIDE_BY_ZERO and
			(b.has_value and (std::int32_t) b.value.find_first_not_of('0') == -1))	// dzielnik jest zerem (stałą)
	{
		// optymalizacja: dzielenie przez zero
		std::cerr << ">> optymalizacja: dzielenie przez 0\n";

		machine_code << ZERO << "\n";
	}
	else
	{
		std::uint32_t padLen = 0, divLen = 0;
		std::string padding = helper::pad_left(&padLen, offset + 8);
		std::string division = get_divide_code(&divLen, offset + 8 + padLen);

		machine_code
				<< LOAD << " " << a.current_addr << "\n"
				<< STORE << " " << 0 << "\n"
				<< LOAD << " " << b.current_addr << "\n"
				<< STORE << " " << 1 << "\n"
				<< ZERO << "\n"
				<< STORE << " " << 3 << "\n"
				<< LOAD << " " << 1 << "\n"							// jeśli b == 0 to wynikiem jest 0
				<< JZ << " " << offset + padLen + divLen << "\n"
				<< padding
				<< division
				<< LOAD << " " << 3 << "\n";
	}

	return machine_code.str();
}



std::string
get_divide_code(std::uint32_t *length, const std::uint32_t& offset)
{
	std::ostringstream machine_code;
	
	machine_code
			<< LOAD << " " << 2 << "\n"				//	while d > 0 do
			<< JZ << " " << offset + 18 << "\n"
			<< LOAD << " " << 1 << "\n"				//	 	if b <= a then
			<< SUB << " " << 0 << "\n"
			<< JG << " " << offset + 11 << "\n"	
			<< LOAD << " " << 3 << "\n"				//			c += d;
			<< ADD << " " << 2 << "\n"
			<< STORE << " " << 3 << "\n"
			<< LOAD << " " << 0 << "\n"				//			a -= b;
			<< SUB << " " << 1 << "\n"
			<< STORE << " " << 0 << "\n"			//		end
			<< LOAD << " " << 1 << "\n"				//		b >>= 1;
			<< SHR << "\n"
			<< STORE << " " << 1 << "\n"
			<< LOAD << " " << 2 << "\n"				//		d >>= 1;
			<< SHR << "\n"
			<< STORE << " " << 2 << "\n"
			<< JUMP << " " << offset << "\n";		// end			
	
	std::string str = machine_code.str();
	*length = std::count(str.begin(), str.end(), '\n');
	
	return str;
}

