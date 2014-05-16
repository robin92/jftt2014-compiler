
#include <iostream>
#include <string>

#include <cstdio>

#include "parser.tab.hh"

int
main(int argc, char **argv)
{
	return yyparse();
}

