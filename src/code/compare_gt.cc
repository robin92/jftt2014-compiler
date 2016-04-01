/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <iostream>
#include <sstream>
#include <string>

#include "config.hh"
#include "code.hh"



using namespace code::cmd;

std::string
code::compare_gt(
    const ISymbolTable::Entry& a,
    const ISymbolTable::Entry& b,
    const std::uint32_t offset)
{    
    std::ostringstream machine_code;
    
    bool bIsZero = b.has_value and (std::int32_t) b.value.find_first_not_of('0') == -1;                
    if (F_COND_GT_ZERO and bIsZero)
    {
        // optymalizacja: x > 0
        std::cerr << ">> optymalizacja: > 0\n";
        
        machine_code
                << LOAD << " " << a.current_addr << "\n"
                << JG << " " << offset + 4 << "\n"            //     if a > 0
                << ZERO << "\n"                                //         false
                << JUMP << " " << offset + 5 << "\n"
                << INC << "\n";                                //         true
    }
    else
    {
        machine_code << compare_lt(b, a, offset);
    }
    
    return machine_code.str();
}

