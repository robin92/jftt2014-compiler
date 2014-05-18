#ifndef COMPILER_CODE_HH_
#define COMPILER_CODE_HH_

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

	// FIXME: powinna przyjmować ISymbolTable::Entry
	std::string
	copy_value(const std::uint32_t& dst, const std::uint32_t& src);

	// FIXME: powinna przyjmować ISymbolTable::Entry
	std::string
	add(const std::uint32_t& a, const std::uint32_t& b);

	// FIXME: powinna przyjmować ISymbolTable::Entry
	std::string
	subtract(const std::uint32_t& a, const std::uint32_t& b);
}	// namespace code

#endif	// COMPILER_CODE_HH_

