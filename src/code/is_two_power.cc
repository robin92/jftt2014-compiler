/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <algorithm>
#include <iostream>
#include <string>

#include <gmpxx.h>

#include "code.hh"



using namespace code::cmd;

bool
code::helper::is_two_power(std::uint64_t *power, const std::string& decnum)
{
    mpz_class num(decnum);

    std::string binnum = num.get_str(2);
    std::uint64_t ones = std::count(binnum.begin(), binnum.end(), '1');
    if (ones != 1) return false;

    if (power) *power = binnum.length() - (std::uint64_t) binnum.find_first_of('1') - 1;

    return true;
}

