/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#ifndef COMPILER_COMMAND_HH_
#define COMPILER_COMMAND_HH_



class Condition;
class Expression;
class Commands;
class ISymbolTable;


struct Command
{
    enum class Type : std::uint8_t
    {
        UNDEFINED = 0,
        ASSIGNMENT,
        IF,
        WHILE,
        READ,
        WRITE,
    };
    
    static std::string str(Type type);
    
    Type type                {Type::UNDEFINED};
    std::string identifier    {};
    Expression *expr        {nullptr};
    Condition *cond            {nullptr};
    Commands *thencmds        {nullptr};
    Commands *elsecmds        {nullptr};
    Commands *docmds        {nullptr};
    
    virtual ~Command();
    
    std::int32_t generate(
            std::ostream& output,
            std::uint32_t* length,
            ISymbolTable* symtbl,
            const std::uint32_t& offset = 0);
    
    std::int32_t operator() (
            std::ostream& output,
            std::uint32_t* length,
            ISymbolTable* symtbl,
            const std::uint32_t& offset = 0);
};

#endif  // COMPILER_COMMAND_HH_

