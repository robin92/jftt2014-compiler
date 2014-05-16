
#include <stdexcept>
#include <string>

#include "ISymbolTable.hh"



bool
ISymbolTable::contains(const std::string& symbol) const
{
	try
	{
		Entry entry = this->get(symbol);
	}
	catch (std::out_of_range& e)
	{
		return false;
	}
	
	return true;
}

ISymbolTable::Entry
ISymbolTable::operator[] (const std::string& symbol) const
{
	return this->get(symbol);
}

