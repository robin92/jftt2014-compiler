	
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

std::string
code::add(const std::uint32_t& a, const std::uint32_t& b)
{
	std::ostringstream machine_code;
	
	machine_code
			<< LOAD << " " << a << "\n"
			<< ADD << " " << b << "\n";
	
	return machine_code.str();
}

