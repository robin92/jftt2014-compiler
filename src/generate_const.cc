
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

std::string
code::generate_const(std::uint32_t* addr, const std::string& decnum)
{
	// TODO: wsparcie dla dużych liczb (> 64b)
	std::cerr << ">> generowanie kodu dla stałej " << decnum << "\n";

	std::list<std::string> lines;
	std::uint64_t number = std::stol(decnum);
	bool was_one = false;

	lines.push_back(ZERO);
	for (std::int32_t i = 0; i < sizeof(std::uint64_t) << 3; i++)
	{
		if ( (number & 0x8000000000000000) > 0 )
		{
			// bit 1
			was_one = true;
			lines.push_back(INC);
		}
		if ( (i + 1 != sizeof(std::uint64_t) << 3) and was_one) lines.push_back(SHL);

		number <<= 1;
	}

	std::ostringstream code;
	for (std::string& str : lines) code << str << "\n";
	code << STORE << " 0\n"
			<< PRINT << " 0\n";

	return code.str();
}

