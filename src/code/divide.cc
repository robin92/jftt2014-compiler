
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <gmpxx.h>

#include "code.hh"



static inline
std::string
get_divide_code(std::uint32_t *length, const std::uint32_t& offset = 0);



using namespace code::cmd;

// FIXME: dzielenie przez 0 ma dawać 0, teraz zacina
// TODO: możliwe optymalizacje:
//	+ /2 => SHR; /4 SHR, SHR; /8 => SHR, SHR, SHR
//	+ /0 => ZERO
std::string
code::divide(
		const ISymbolTable::Entry& a,
		const ISymbolTable::Entry& b,
		const std::uint32_t offset)
{
	std::ostringstream machine_code;
	std::uint32_t padLen = 0, divLen = 0;

	std::string padding = helper::pad_left(&padLen, offset + 4);
	std::string division = get_divide_code(&divLen, offset + 4 + padLen);

	machine_code
			<< LOAD << " " << a.current_addr << "\n"
			<< STORE << " " << 0 << "\n"
			<< LOAD << " " << b.current_addr << "\n"
			<< STORE << " " << 1 << "\n"
			<< padding
			<< division
			<< LOAD << " " << 3 << "\n";

	return machine_code.str();
}



std::string
get_divide_code(std::uint32_t *length, const std::uint32_t& offset)
{
	std::ostringstream machine_code;
	std::cerr << ">> div, offset = " << offset << "\n";
	
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

