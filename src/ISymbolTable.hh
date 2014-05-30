/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#ifndef COMPILER_ISYMBOLTABLE_HH_
#define COMPILER_ISYMBOLTABLE_HH_

#include <list>
#include <tuple>

class ISymbolTable
{
public:
	struct Entry
	{
		enum
		{
			NULLADDR	= -2,
			REG_A		= -1,
		};

		bool has_value {false};
		std::string value {};
		std::int32_t current_addr {NULLADDR};
	};

	typedef std::list<std::pair<const std::string*, const Entry*>> SymbolList;


	virtual void insert(const std::string& symbol) = 0;

	virtual void update(const std::string& symbol, const Entry& entry) = 0;




	virtual Entry get(const std::string& symbol) const = 0;

	virtual SymbolList all() const = 0;

	virtual Entry operator[] (const std::string& symbol) const;

	virtual bool contains(const std::string& symbol) const;
};

#endif  // COMPILER_ISYMBOLTABLE_HH_

