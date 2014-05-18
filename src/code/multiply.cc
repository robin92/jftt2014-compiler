
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

static inline
std::string
get_jump(const std::uint32_t& addr, const std::uint32_t& offset = 0)
{
	std::ostringstream machine_code;

	machine_code
			<< JUMP << " " << addr + offset << "\n";

	return machine_code.str();
}

static inline
std::string
get_jg(const std::uint32_t& addr, const std::uint32_t& offset)
{
	std::ostringstream machine_code;

	machine_code
			<< JUMP << " " << addr + offset << "\n";

	return machine_code.str();
}

static inline
std::string
get_multiply(const std::uint32_t& offset = 0)
{
	std::ostringstream machine_code;

	/**
	 * Mnożenie logarytmiczne dwóch liczb na maszynie.
	 *
	 * Kod mnoży dwie liczby znajdujące się w p[0] (mnoznik) i w p[1] (mnozna).
	 * Wynik dostępny jest w p[2].
	 */
	// FIXME
	//	+ adresy sa wpisane na stałe, a są zależne od długości kodu, trzeba uwzględnić offset
	//	+ dużo razy wykonywane jest po sobie LOAD 1, może da się zmiejszyć ich ilośćstatic
	machine_code
			<< "ZERO\n"
			<< "STORE 2\n"
			<< get_jump(7, offset)	// goto @main
			<< "LOAD 2\n"			// jedynka
			<< "ADD 0\n"
			<< "STORE 2\n"
			<< get_jump(9, offset)	// goto @step
			<< "LOAD 1\n"			// main
			<< "JODD 3\n"			// goto @jedynka
			<< "LOAD 1\n"			// step
			<< "SHR\n"
			<< "STORE 1\n"
			<< "LOAD 0\n"
			<< "SHL\n"
			<< "STORE 0\n"
			<< "LOAD 1\n"
			<< get_jg(7, offset)	// goto @main
			<< "";

	return machine_code.str();
}

std::string
code::multiply(const ISymbolTable::Entry& a,
		const ISymbolTable::Entry& b,
		const std::uint32_t offset)
{
	std::ostringstream machine_code;

	machine_code
			<< LOAD << " " << a.current_addr << "\n"
			<< STORE << " " << "0" << "\n"
			<< LOAD << " " << b.current_addr << "\n"
			<< STORE << " " << "1" << "\n"
			<< get_multiply(offset)
			<< LOAD << " " << "2" << "\n";

	return machine_code.str();
}

