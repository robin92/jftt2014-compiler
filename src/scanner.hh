#ifndef SCANNER_HH
#define SCANNER_HH
 
#if ! defined(yyFlexLexerOnce)
#include "FlexLexer.h"
#endif
 
#undef  YY_DECL
#define YY_DECL int Scanner::yylex()
 
#include "parser.tab.hh"

class Scanner
    : public yyFlexLexer
{
private:
    yy::parser::semantic_type *yylval;

    int yylex();

public:   
    Scanner()
    {
        yylval = NULL;
    }
    
    virtual int yylex(yy::parser::semantic_type *lval)
    {
        yylval = lval;
        return yylex();
    }
};

#endif

