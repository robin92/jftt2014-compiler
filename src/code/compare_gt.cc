
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

std::string
code::compare_gt(
	const ISymbolTable::Entry& a,
	const ISymbolTable::Entry& b,
	const std::uint32_t offset)
{					
	return compare_lt(b, a, offset);
}

