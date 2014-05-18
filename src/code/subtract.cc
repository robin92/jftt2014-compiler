
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

std::string
code::subtract(const std::uint32_t& a, const std::uint32_t& b)
{
	std::ostringstream machine_code;

	machine_code
			<< LOAD << " " << a << "\n"
			<< SUB << " " << b << "\n";

	return machine_code.str();
}

