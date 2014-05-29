
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include "config.hh"
#include "code.hh"



static inline
std::string
get_modulo_code(std::uint32_t *length, const std::uint32_t& offset = 0);



using namespace code::cmd;

std::string
code::modulo(
		const ISymbolTable::Entry& a,
		const ISymbolTable::Entry& b,
		const std::uint32_t offset)
{
	std::ostringstream machine_code;

	bool bIsZero = b.has_value and (std::int32_t) b.value.find_first_not_of('0') == -1,
			bIsTwo = b.has_value and b.value == "2";
	if (F_MODULO_TWO and bIsTwo)
	{
		// optymalizacja: x mod 2
		std::cerr << ">> optymalizacja: mod 2\n";

		machine_code
				<< LOAD << " " << a.current_addr << "\n"
				<< JODD << " " << offset + 4 << "\n"
				<< ZERO << "\n"
				<< JUMP << " " << offset + 6 << "\n"
				<< ZERO << "\n"
				<< INC << "\n";
	}
	else
	{
		std::uint32_t padLen = 0, modLen = 0;

		std::string padding = helper::pad_left(&padLen, offset + 6);
		std::string mod = get_modulo_code(&modLen, offset + 6 + padLen);

		machine_code
				<< LOAD << " " << a.current_addr << "\n"
				<< STORE << " " << 0 << "\n"
				<< LOAD << " " << b.current_addr << "\n"
				<< STORE << " " << 1 << "\n"
				<< LOAD << " " << 1 << "\n"							// jeśli b == 0 to wynikiem jest 0
				<< JZ << " " << offset + padLen + modLen + 1 << "\n"
				<< padding
				<< mod
				<< LOAD << " " << 0 << "\n";
	}
	
	return machine_code.str();
}



std::string
get_modulo_code(std::uint32_t *length, const std::uint32_t& offset)
{
	std::ostringstream machine_code;
	std::cerr << ">> div, offset = " << offset << "\n";
	
	machine_code
			<< LOAD << " " << 2 << "\n"				//	while d > 0 do
			<< JZ << " " << offset + 15 << "\n"
			<< LOAD << " " << 1 << "\n"				//	 	if b <= a then
			<< SUB << " " << 0 << "\n"
			<< JG << " " << offset + 8 << "\n"	
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

