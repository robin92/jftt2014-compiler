/*
 * Copyright 2014 Rafał Bolanowski
 * All rights reserved.
 *
 * For licensing information please see the LICENSE file.
 */

#ifndef COMPILER_EXPRESSION_HH_
#define COMPILER_EXPRESSION_HH_

#include <tuple>

struct Expression
{
    enum class Operation : std::uint8_t
    {
        ADD = 1,
        SUBTRACT,
        MULTIPLY,
        DIVIDE,
        MODULO,
    };

    enum class Type : std::uint8_t
    {
        UNDEFINED = 0,
        NUMBER,
        IDENTIFIER,
        COMPLEX,
    };

    typedef std::tuple<std::string, std::string, Operation> Complex;

    Type type                {Type::UNDEFINED};
    std::string number        {};
    std::string identifier    {};
    Complex complex            {};
};

#endif  // COMPILER_EXPRESSION_HH_

