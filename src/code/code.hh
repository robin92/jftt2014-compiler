#ifndef COMPILER_CODE_HH_
#define COMPILER_CODE_HH_

#include "../ISymbolTable.hh"

namespace code
{
	namespace cmd
	{
		const std::string SCAN	= "SCAN";
		const std::string PRINT	= "PRINT";
		
		const std::string LOAD	= "LOAD";
		const std::string STORE	= "STORE";
		
		const std::string ADD	= "ADD";
		const std::string SUB	= "SUB";

		const std::string SHR	= "SHR";
		const std::string SHL	= "SHL";
		const std::string INC	= "INC";
		const std::string DEC	= "DEC";
		const std::string ZERO	= "ZERO";

		const std::string JUMP	= "JUMP";
		const std::string JZ	= "JZ";
		const std::string JG	= "JG";
		const std::string JODD	= "JODD";

		const std::string HALT	= "HALT";
	}	// namespace cmd

	std::string
	generate_number(const std::string& decnum);

	std::string
	copy_value(const ISymbolTable::Entry& dst, const ISymbolTable::Entry& src);

	std::string
	compare_eq(
			const ISymbolTable::Entry& a,
			const ISymbolTable::Entry& b,
			const std::uint32_t offset = 0);

	std::string
	add(const ISymbolTable::Entry& a, const ISymbolTable::Entry& b);

	std::string
	subtract(const ISymbolTable::Entry& a, const ISymbolTable::Entry& b);
	
	std::string
	multiply(
			const ISymbolTable::Entry& a,
			const ISymbolTable::Entry& b,
			const std::uint32_t offset = 0);
}	// namespace code

#endif	// COMPILER_CODE_HH_

