
#include <iostream>
#include <string>

#include <cstdio>

#include "parser.tab.hh"
#include "scanner.hh"

int
main(int argc, char **argv)
{
	Scanner s;
	yy::parser *parser = new yy::parser(s);

	return parser->parse();
}

