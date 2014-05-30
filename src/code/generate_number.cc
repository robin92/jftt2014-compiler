
#include <iostream>
#include <list>
#include <sstream>
#include <string>

#include <gmpxx.h>

#include "code.hh"



using namespace code::cmd;

std::int32_t
get_leftmost_bit(std::string* binnum)
{
	char c = (*binnum)[0];
	*binnum = binnum->length() >= 2 ? binnum->substr(1) : "0";

	return c == '0' ? 0 : 1;
}

std::string
code::generate_number(const std::string& decnum)
{
	std::list<std::string> lines;
	mpz_class number(decnum);
	std::string bin = number.get_str(2);
	std::uint64_t limit = mpz_sizeinbase(number.get_mpz_t(), 2);
	bool was_one = false;

	lines.push_back(ZERO);
	for (std::int32_t i = 0; i < limit; i++)
	{
		if ( get_leftmost_bit(&bin) > 0 )
		{
			// bit 1
			was_one = true;
			lines.push_back(INC);
		}
		if ( (i != limit - 1) and was_one) lines.push_back(SHL);
	}

	std::ostringstream code;
	for (std::string& str : lines) code << str << "\n";

	return code.str();
}

