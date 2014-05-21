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

	namespace helper
	{
		/**
		 * Wyrównuje bity liczb w rejestrach p[0] i p[1].
		 * Dodatkowo w rejestrze p[2] umieszcza liczbę 1 na pozycji najstarszego bitu p[0].
		 */
		std::string
		pad_left(std::uint32_t *length = nullptr, const std::uint32_t& offset = 0);
	}

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
	compare_ne(
			const ISymbolTable::Entry& a,
			const ISymbolTable::Entry& b,
			const std::uint32_t offset = 0);

	std::string
	compare_lt(
			const ISymbolTable::Entry& a,
			const ISymbolTable::Entry& b,
			const std::uint32_t offset = 0);

	std::string
	compare_gt(
			const ISymbolTable::Entry& a,
			const ISymbolTable::Entry& b,
			const std::uint32_t offset = 0);

	std::string
	compare_le(
			const ISymbolTable::Entry& a,
			const ISymbolTable::Entry& b,
			const std::uint32_t offset = 0);

	std::string
	compare_ge(
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

	std::string
	divide(
			const ISymbolTable::Entry& a,
			const ISymbolTable::Entry& b,
			const std::uint32_t offset = 0);
}	// namespace code

#endif	// COMPILER_CODE_HH_

