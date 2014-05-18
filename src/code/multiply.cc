
#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"




/**
 * Mnożenie logarytmiczne dwóch liczb na maszynie.
 *
 * Kod mnoży dwie liczby znajdujące się w p[0] (mnoznik) i w p[1] (mnozna).
 * Wynik dostępny jest w p[2].
 */
// FIXME
//	+ adresy sa wpisane na stałe, a są zależne od długości kodu, trzeba uwzględnić offset
//	+ dużo razy wykonywane jest po sobie LOAD 1, może da się zmiejszyć ich ilośćstatic
static
const std::string multiply_method =
		"ZERO\n"
		"STORE 2\n"
		"JUMP 9\n"		// goto @main
		"LOAD 2\n"		// jedynka
		"ADD 0\n"
		"STORE 2\n"
		"JUMP 11\n"		// goto @step
		"LOAD 1\n"		// main
		"JODD 5\n"		// goto @jedynka
		"LOAD 1\n"		// step
		"SHR\n"
		"STORE 1\n"
		"LOAD 0\n"
		"SHL\n"
		"STORE 0\n"
		"LOAD 1\n"
		"JG 9\n"		// goto @main
		"";



using namespace code::cmd;

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
			<< multiply_method							// TODO: uwzględnić offset
			<< LOAD << " " << "2" << "\n";

	return machine_code.str();
}

