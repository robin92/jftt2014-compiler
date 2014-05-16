
%option yyclass="Scanner"
%option noyywrap
%option c++
%option yylineno

%{
    #include <iostream>
    #include <string>
    #include <sstream>
  
    #include <cstdio>
    
    #include "scanner.hh"
    #include "parser.tab.hh"
    
    //
    // imports from bison parser
    //
    
    extern std::ostringstream onpNotation;
%}



%%



%%

