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

#include "code.hh"



using namespace code::cmd;

std::string
code::helper::pad_left(std::uint32_t *length, const std::uint32_t& offset)
{
	std::ostringstream machine_code;
	
	machine_code
			<< ZERO << "\n"							//	d := 1
			<< INC << "\n"
			<< STORE << " " << 2 << "\n"
			<< LOAD << " " << 0 << "\n"				//	while a > b do
			<< SUB << " " << 1 << "\n"
			<< JZ << " " << offset + 13 << "\n"
			<< LOAD << " " << 1 << "\n"				//		b := b << 1;
			<< SHL << "\n"
			<< STORE << " " << 1 << "\n"
			<< LOAD << " " << 2 << "\n"				//		d := d << 1;
			<< SHL << "\n"
			<< STORE << " " << 2 << "\n"
			<< JUMP << " " << offset + 3 << "\n";	//	end

	std::string tmp = machine_code.str();
	if (length) *length = std::count(tmp.begin(), tmp.end(), '\n');

	return tmp;
}

