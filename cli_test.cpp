#include <iostream>
#include <sstream>
#include <cassert>

#include "utils.hpp"

void testClearTerminal()
{
    // redirect cout to a stringstream
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf(ss.rdbuf());

    // clear the terminal
    clearTerminal();

    // restore cout
    std::cout.rdbuf(oldCout);

    // check that the terminal was cleared by looking for the clear screen escape sequence
#ifdef _WIN32
    assert(ss.str() == "");
#else
    assert(ss.str() == "\033[H\033[2J");
#endif
}

void testSetTextColor()
{
    // redirect cout to a stringstream
    std::stringstream ss;
    std::streambuf *oldCout = std::cout.rdbuf(ss.rdbuf());

    // set the text color to red
    setTextColor(1);
    std::cout << "hello";

    // restore cout
    std::cout.rdbuf(oldCout);

    // check that the text color was set to red by looking for the set text color escape sequence
#ifdef _WIN32
    assert(ss.str() == "");
#else
    assert(ss.str() == "\033[38;5;1mhello");
#endif
}

testClearTerminal();
testSetTextColor();
