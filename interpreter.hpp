#ifndef INTERPRETER_HPP
#define INTERPRETER_HPP

#include "world.hpp"

#include <boost/optional.hpp>

#include <string>

namespace boostMUD {
namespace util {

class Interpreter {
public:
Interpreter(world::World& world);

virtual boost::optional<std::string> interpret(const std::string &input);

virtual std::string prompt();

protected:
world::World& world;

std::string buffer;

};

}
}

#endif