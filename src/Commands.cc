
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

