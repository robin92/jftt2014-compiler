/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#ifndef COMPILER_COMMANDS_HH_
#define COMPILER_COMMANDS_HH_

#include <list>



class Command;



struct Commands
{
	typedef std::list<Command*> CommandList;

	CommandList cmds;

	virtual ~Commands();
};

#endif  // COMPILER_COMMANDS_HH_

