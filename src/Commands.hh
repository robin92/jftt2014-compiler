#ifndef COMPILER_COMMANDS_HH_
#define COMPILER_COMMANDS_HH_

#include <list>



class Command;



struct Commands
{
	typedef std::list<Command*> CommandList;
	
	CommandList cmds;	
};

#endif  // COMPILER_COMMANDS_HH_

