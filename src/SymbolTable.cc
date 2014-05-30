/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <stdexcept>

#include "SymbolTable.hh"



void
SymbolTable::insert(const std::string& symbol)
{
	if ( this->contains(symbol) ) throw std::invalid_argument("already in the symbol table");

	Entry entry;
	_symbols.insert(std::pair<std::string, Entry>(symbol, entry));
}

void
SymbolTable::update(const std::string& symbol, const SymbolTable::Entry& entry)
{
	_symbols.at(symbol) = entry;
}



SymbolTable::Entry
SymbolTable::get(const std::string& symbol) const
{
	return _symbols.at(symbol);
}

SymbolTable::SymbolList
SymbolTable::all() const
{
	SymbolList list;

	for (auto it = _symbols.cbegin(); it != _symbols.cend(); it++)
	{
		list.push_back(std::pair<const std::string*, const Entry*>(&it->first, &it->second));
	}

	return list;
}

