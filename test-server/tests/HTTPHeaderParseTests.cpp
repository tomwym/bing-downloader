#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "pure-functions/Parsing.h"

#include <iostream>
#include <string>

using namespace ::testing;
using namespace Naive;

TEST(HTTPTests, SplitStringTest)
{
    {
        std::string basic_string {"GET / HTTP/1.1"};
        auto v = SplitString(basic_string, ' ');
    }
}

TEST(HTTPTESTS, SplitHeaderLineTest)
{
    {
        std::string basic {"Host :  localhost:8080"};
        auto p = SplitStringOnFirstDelimiter(basic, ":");
        std::cout << p.first << '\n' << p.second << '\n';
    }
}