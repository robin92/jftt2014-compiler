/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#ifndef COMPILER_SYMBOLTABLE_HH_
#define COMPILER_SYMBOLTABLE_HH_

#include <map>
#include <string>

#include "ISymbolTable.hh"



class SymbolTable : public ISymbolTable
{
public:
	typedef std::map<std::string, SymbolTable::Entry> Map;



	virtual void insert(const std::string& symbol);

	virtual void update(const std::string& symbol, const Entry& entry);



	virtual Entry get(const std::string& symbol) const;

	virtual SymbolList all() const;

private:
	 Map _symbols;
};

#endif  // COMPILER_SYMBOLTABLE_HH_

