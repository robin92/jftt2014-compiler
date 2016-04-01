/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <iostream>
#include <sstream>
#include <string>

#include "code.hh"



using namespace code::cmd;

std::string
code::copy_value(const ISymbolTable::Entry& dst, const ISymbolTable::Entry& src)
{
    std::ostringstream machine_code;

    machine_code
            << LOAD << " " << src.current_addr << "\n"
            << STORE << " " << dst.current_addr << "\n";

    return machine_code.str();
}

