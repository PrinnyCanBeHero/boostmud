#ifndef CONSOLE_HPP_
#define CONSOLE_HPP_

#include "util/interpreter.hpp"
#include "util/tcpserver.hpp"
#include "world/world.hpp"

#include <vector>

namespace boostMUD {
namespace util {

/**
 * Representation of a client connection.
 */
class Console : public TcpSession {
public:
Console(world::World &world, boost::asio::io_service &io);

/**
 * Sends some message to the user console.
 */
virtual void sendToConsole(const std::string &message);

virtual ~Console();

protected:

/**
 * Handle a Telnet command byte other than DO/DONT/WILL/WONT.
 */
void handleCommand(unsigned char cmd);

/**
 * Handle a Telnet DO/DONT/WILL/WONT command.
 * @note RFC854 says it's valid to reject all options. So we do that.
 */
void handleOption(unsigned char cmd, unsigned char option);

/**
 * Method we must implement to handle received bytes.
 */
virtual void handleIncoming(const ByteArray& bytes);

private:

/**
 * Suppress Telnet Go-Ahead Response.
 */
bool suppress_ga;

/**
 * Echo bytes back.
 */
bool echo;

/**
 * Dispatch user input (if any) to the interpreter. 
 * Also sends interpreter responses to the client.
 */
void interpretInput();

/** 
 * Default handling of input byte.
 */
void consume(unsigned char c);
/**
 * Handling of input byte following IAC.
 */
void iac(unsigned char c);
/**
 * Handling of option byte following WILL/WONT/DO/DONT
 */
void option(unsigned char c);
/**
 * Handling of sub-negotiation bytes.
 */ 
void discard_sub(unsigned char c);

/**
 * Echo a byte back to the client.
 */
void do_echo(unsigned char c);

/**
 * Reference to World object.
 */
world::World &world;

/**
 * Our user input interpreter.
 */
Interpreter interpreter;

/**
 * Function object to store current byte operation.
 */
boost::function<void (unsigned char)> consume_byte;

/**
 * Where we store parsed command bytes. 
 * Probably overkill as a vector.
 */
std::vector<unsigned char> control_buffer;

/**
 * Where we store potential user input bytes.
 */
std::vector<unsigned char> command_buffer;

};

}
}

#endif