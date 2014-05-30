/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <iostream>

#include "Commands.hh"
#include "Command.hh"



Commands::~Commands()
{
	for (Command* cmd : cmds)
	{
		if (cmd) delete cmd;
		cmd = nullptr;
	}
}

