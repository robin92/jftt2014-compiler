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
code::compare_ge(
        const ISymbolTable::Entry& a,
        const ISymbolTable::Entry& b,
        const std::uint32_t offset)
{
    return compare_le(b, a, offset);
}

