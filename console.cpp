#include "console.hpp"

#include <memory>
#include <vector>

#include <cstdlib>

namespace {
const unsigned char GO_AHEAD_CHAR = 249;
const unsigned char IAC_CHAR = 255;

const unsigned char GO_AHEAD_MSG[] = { IAC_CHAR, GO_AHEAD_CHAR };
const boostMUD::util::ByteArray GO_AHEAD(GO_AHEAD_MSG, sizeof(GO_AHEAD_MSG));

}

namespace command {

const unsigned char SE   = 240;

const unsigned char EC   = 247;

const unsigned char SB   = 250;

const unsigned char WILL = 251;
const unsigned char WONT = 252;
const unsigned char DO   = 253;
const unsigned char DONT = 254;

}

namespace option {

#ifdef ECHO
#undef ECHO
#endif

const unsigned char ECHO = 1;
const unsigned char SUPPRESS_GA = 3;


}

namespace {

inline bool in_option_range(unsigned char c) {
	return (c >= command::WILL <= command::DONT);
}

}

namespace boostMUD {
namespace util {
Console::Console(world::World &world, boost::asio::io_service &io) 
: TcpSession(io), suppress_ga(false), echo(false), world(world), interpreter(world)
{

sendToConsole(world.welcomeMsg() + "\n\n" + interpreter.prompt());

consume_byte = boost::bind(&Console::consume,this,_1);

}

void Console::sendToConsole(const std::string &message) {

auto output = std::make_shared<ByteArray>(reinterpret_cast<const unsigned char*>(message.c_str()), message.size());

if(!suppress_ga) {
	output = std::make_shared<ByteArray>(*output, GO_AHEAD);
}

do_write(output);

}

void Console::handleCommand(unsigned char cmd) {
	
}

Console::~Console() {
}

void Console::handleOption(unsigned char cmd, unsigned char option)
{
	if(cmd == command::DO) {
		auto reply = std::make_shared<ByteArray>(3);
		reply->data()[0] = IAC_CHAR;
		if(option == option::SUPPRESS_GA) {
			reply->data()[1] = command::WILL;
			suppress_ga = true;
			//std::cout << "Informing client we will suppress GA." << std::endl;
		}
		else if(option == option::ECHO) {
			reply->data()[1] = command::WILL;
			echo = true;
			//std::cout << "Informing client we will echo." << std::endl;
		}
		else {
			reply->data()[1] = command::WONT;
			//std::cout << "Informing client we will not do option: " << (int)option << std::endl;
		}
		reply->data()[2] = option;
		
		do_write(reply);
	}
	if(cmd == command::DONT) {
		if(option == option::SUPPRESS_GA && suppress_ga) {
			auto reply = std::make_shared<ByteArray>(3);
			reply->data()[0] = IAC_CHAR;
			reply->data()[1] = command::WONT;
			reply->data()[2] = option::SUPPRESS_GA;
			suppress_ga = false;
			std::cout << "Informing client we won't suppress GA." << std::endl;
			do_write(reply);
		}
		else if(option == option::ECHO && echo) {
			auto reply = std::make_shared<ByteArray>(3);
			reply->data()[0] = IAC_CHAR;
			reply->data()[1] = command::WONT;
			reply->data()[2] = option::ECHO;
			echo = false;
			std::cout << "Informing client we won't echo." << std::endl;
			do_write(reply);
		}
	}
}

void Console::interpretInput()
{
	if(!command_buffer.empty()) {
		boost::optional<std::string> output = interpreter.interpret(std::string(reinterpret_cast<char*>(&command_buffer[0]), command_buffer.size()));
		
		if(output) {
			sendToConsole(*output);
		}
		command_buffer.clear();
	}
}

void Console::consume(unsigned char c) {
	if(c == IAC_CHAR) {
		interpretInput();
		control_buffer.push_back(IAC_CHAR);
		consume_byte = boost::bind(&Console::iac,this,_1);
	}
	else if(c == command::SB) {
		consume_byte = boost::bind(&Console::discard_sub,this,_1);
	}
	else if(c == 0) {
		interpretInput();
	}
	else {
		if(echo) {
			do_echo(c);
		}
		command_buffer.push_back(c);
	}
}

void Console::iac(unsigned char c) {
	control_buffer.push_back(c);
	if (in_option_range(c)) {
		consume_byte = boost::bind(&Console::option,this,_1);
	}
	else {
		handleCommand(control_buffer.back());
		consume_byte = boost::bind(&Console::consume,this,_1);
		control_buffer.clear();
	}
}

void Console::option(unsigned char c) {
	handleOption(control_buffer.back(), c);
	consume_byte = boost::bind(&Console::consume,this,_1);
	control_buffer.clear();
}

void Console::discard_sub(unsigned char c) {
	if(c == command::SE) {
		consume_byte = boost::bind(&Console::consume,this,_1);
	}
}

void Console::do_echo(unsigned char c) {
	
	do_write(std::make_shared<ByteArray>(&c,1));

}

void Console::handleIncoming(const ByteArray& bytes) {
	std::vector<unsigned char> msg(control_buffer.size() + bytes.size());
		
	if (!control_buffer.empty())
	{
		memcpy(&msg[0], &control_buffer[0], control_buffer.size());
	}
	memcpy(&msg[0]+control_buffer.size(), bytes.ptr(), bytes.size());
	
	control_buffer.clear();
	
	for (auto c : msg) {
		consume_byte(c);
	}
	
	interpretInput();
	
}

}
}