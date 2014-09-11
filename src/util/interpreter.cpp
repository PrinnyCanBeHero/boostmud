#include "interpreter.hpp"

#include <iostream>

namespace boostMUD {
namespace util {

Interpreter::Interpreter(world::World& world) 
: world(world) 
{
}

boost::optional<std::string> Interpreter::interpret(const std::string &input)
{
	std::string result;
	
	for(auto c : input) 
	{
		if (c > 127)
		{
			std::cerr << "CONTROL: " << (int)(unsigned char)c << std::endl;
		}
		else if (c == '\r') {
			if(!buffer.empty()) {
				result += "\nReceived: " + buffer + '\n';
				buffer.clear();
			}
			result += prompt();
		}
		else if( c == '\n') {
			
		}
		else {
			buffer.push_back(c);
		}
	}

	if(result.empty())
		return boost::none;

	return result;

}

std::string Interpreter::prompt()
{
	return "\r\n> ";
}

}
}