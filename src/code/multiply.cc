/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <algorithm>
#include <iostream>
#include <sstream>
#include <string>

#include <gmpxx.h>

#include "config.hh"
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
            << get_jump(7, offset)    // goto @main
            << "LOAD 2\n"            // jedynka
            << "ADD 0\n"
            << "STORE 2\n"
            << get_jump(9, offset)    // goto @step
            << "LOAD 1\n"            // main
            << get_jodd(3, offset)    // goto @jedynka
            << "LOAD 1\n"            // step
            << "SHR\n"
            << "STORE 1\n"
            << "LOAD 0\n"
            << "SHL\n"
            << "STORE 0\n"
            << "LOAD 1\n"
            << get_jg(7, offset)    // goto @main
            << "";

    return machine_code.str();
}



std::string
code::multiply(const ISymbolTable::Entry& a,
        const ISymbolTable::Entry& b,
        const std::uint32_t offset)
{
    std::ostringstream machine_code;

    if (F_CONST_EXPR and (a.has_value and b.has_value))    // obie stałe
    {
        // optymalizacja: a * b
        std::cerr << ">> optymalizacja: a * b\n";
        mpz_class av(a.value), bv(b.value), res = av * bv;

        machine_code << generate_number(res.get_str());

        return machine_code.str();
    }

    std::uint64_t apower = 0, bpower = 0;
    bool atwopower = a.has_value and helper::is_two_power(&apower, a.value),
            btwopower = b.has_value and helper::is_two_power(&bpower, b.value);
    if (F_MULTIPLY_BY_ZERO and (
            (a.has_value and (std::int32_t) a.value.find_first_not_of('0') == -1) or
            (b.has_value and (std::int32_t) b.value.find_first_not_of('0') == -1) ))
    {
        // optymalizacja: mnozenie przez zero
        std::cerr << ">> optymalizacja: mnozenie przez 0\n";

        machine_code << ZERO << "\n";
    }
    else if (F_MULTIPLY_BY_TWO_POWERS and (atwopower or btwopower))
    {
        std::uint32_t power = bpower;
        const ISymbolTable::Entry *multiplier = &a, *multiplicator = &b;
        if (atwopower)
        {
            power = apower;
            multiplier = &b;
            multiplicator = &a;
        }

        // optymalizacja: mnożenie przez potęgę dwójki
        std::cerr
                << ">> optymalizacja: mnożenie przez 2^i, value = "
                << multiplicator->value << ", power = " << power << "\n";

        machine_code << LOAD << " " << multiplier->current_addr << "\n";
        for (std::uint64_t i = 0; i < power; i++) machine_code << SHL << "\n";
    }
    else
    {
        machine_code
                << LOAD << " " << a.current_addr << "\n"        // przygotowanie mnożnej
                << STORE << " " << "0" << "\n"
                << LOAD << " " << b.current_addr << "\n"        // przygotowanie mnożnika
                << STORE << " " << "1" << "\n"
                << LOAD << " " << "0" << "\n"                    // a >= b
                << SUB << " " << "1" << "\n"
                << JG << " " << offset + 14 << "\n"
                << LOAD << " " << "0" << "\n"                    // swap(a, b)
                << STORE << " " << "2" << "\n"
                << LOAD << " " << "1" << "\n"
                << STORE << " " << "0" << "\n"
                << LOAD << " " << "2" << "\n"
                << STORE << " " << "1" << "\n"
                << LOAD << " " << "0" << "\n"
                << get_multiply(offset + 14)                    // dodano 14 komend
                << LOAD << " " << "2" << "\n";                    // załadowanie wyniku do rejestru a
    }

    return machine_code.str();
}

