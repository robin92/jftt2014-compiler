
#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <gmpxx.h>

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
get_jodd(const std::uint32_t& addr, const std::uint32_t& offset = 0)
{
	std::ostringstream machine_code;

	machine_code
			<< JODD << " " << addr + offset << "\n";

	return machine_code.str();
}


static inline
std::string
get_jg(const std::uint32_t& addr, const std::uint32_t& offset)
{
	std::ostringstream machine_code;

	machine_code
			<< JG << " " << addr + offset << "\n";

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
	machine_code
			<< "ZERO\n"
			<< "STORE 2\n"
			<< get_jump(7, offset)	// goto @main
			<< "LOAD 2\n"			// jedynka
			<< "ADD 0\n"
			<< "STORE 2\n"
			<< get_jump(9, offset)	// goto @step
			<< "LOAD 1\n"			// main
			<< get_jodd(3, offset)	// goto @jedynka
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

// TODO: możliwe optymalizacje:
//	+ x2 => SHL; x4 SHL, SHL; x8 => SHL, SHL, SHL
//	+ WIEKSZA * MNIEJSZA (dla stala * zmienna, zmienna * stala, zmienna * zmienna) w kodzie maszynowym
//	+ mnożenie x0 => ZERO
std::string
code::multiply(const ISymbolTable::Entry& a,
		const ISymbolTable::Entry& b,
		const std::uint32_t offset)
{
	std::ostringstream machine_code;

	// optymalizacja:
	//	jeśli oba argumenty są stałymi to mniejszy z nich będzię mnożnikiem, np.
	//		7 * 2 => 7 * 2
	//		5 * 12 => 12 * 5 (mniej kroków: log(12) > log(5))
	const ISymbolTable::Entry *tmpa = &a, *tmpb = &b;
	if (a.has_value and b.has_value)
	{
		mpz_class av(a.value), bv(b.value);
		if (av < bv) std::swap(tmpa, tmpb);
	}
	// TODO: powyższa optymalizacja może być zastosowana
	//	w kodzie maszynowym dla VAR * CONST, CONST * VAR, VAR * VAR

	machine_code
			<< LOAD << " " << tmpa->current_addr << "\n"	// przygotowanie mnożnej
			<< STORE << " " << "0" << "\n"
			<< LOAD << " " << tmpb->current_addr << "\n"	// przygotowanie mnożnika
			<< STORE << " " << "1" << "\n"
			<< get_multiply(offset + 4)						// dodano 4 komendy
			<< LOAD << " " << "2" << "\n";					// załadowanie wyniku do rejestru a

	return machine_code.str();
}

