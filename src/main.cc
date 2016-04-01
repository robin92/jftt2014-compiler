/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#include <iostream>
#include <string>

#include <cstdio>

#include "parser.tab.hh"

int
main(int argc, char **argv)
{
    return yyparse();
}

