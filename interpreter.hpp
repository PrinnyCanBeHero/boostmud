#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "world.hpp"

#include <boost/optional.hpp>

#include <string>

namespace boostMUD {
namespace util {

/**
 * A command line interpreter.
 */
class Interpreter {
public:
Interpreter(world::World& world);

/**
 * Interpret the given characters. 
 * Optionally return a response to display to the user.
 * Any Telnet control characters should have been stripped out by now.
 */
virtual boost::optional<std::string> interpret(const std::string &input);

/**
 * Return the prompt string for the console.
 */
virtual std::string prompt();

protected:
world::World& world;

std::string buffer;

};

}
}

#endif