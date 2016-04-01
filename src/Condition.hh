/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#ifndef COMPILER_CONDTITION_HH_
#define COMPILER_CONDTITION_HH_

struct Condition
{
    enum class Type : std::uint8_t
    {
        UNDEFINED = 0,
        EQ,
        NE,
        LT,
        GT,
        LE,
        GE,
    };

    typedef std::pair<std::string, std::string> IdentifierPair;
    
    Type type            {Type::UNDEFINED};
    IdentifierPair ids    {};
};

#endif  // COMPILER_CONDTITION_HH_

