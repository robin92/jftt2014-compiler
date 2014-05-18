
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

std::string
code::copy_value(const std::uint32_t& dst, const std::uint32_t& src)
{
	std::ostringstream machine_code;

	machine_code
			<< LOAD << " " << src << "\n"
			<< STORE << " " << dst << "\n";

	return machine_code.str();
}

