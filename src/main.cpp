#include "util/console.hpp"
#include "util/tcpserver.hpp"
#include "world/world.hpp"

#include <signal.h>

#include <boost/bind.hpp>

#include <iostream>
#include <condition_variable>
#include <chrono>
#include <thread>

using namespace boostMUD::world;
using namespace boostMUD::util;

std::mutex sleepy_mutex;
std::condition_variable sleepy_variable;

bool done = false;

class TestRoom : public Room 
{
public:
	TestRoom(boost::uint32_t id, const std::string &description)
	: Room(id), _description(description) 
	{
	}
	
	virtual const std::string& description() const {
		return _description;
	}
	virtual std::vector<Exit> exits() const {
		return _exits;
	}
	
private:
	const std::string _description;
	std::vector<Exit> _exits;

};

class EchoSession : public TcpSession 
{
public:
	EchoSession(boost::asio::io_service &io)
	: TcpSession(io) { }
	virtual size_t preferredChunkSize() const { return 128u;  }

	virtual void handleIncoming(const ByteArray& bytes) {
		do_write(std::make_shared<ByteArray>(bytes));
	}
};

TcpSession::pointer make_echo(World& world, boost::asio::io_service& io)
{
	return std::make_shared<Console>(world, io);
}

void SigHandler(int signal);

int main(int argc, char* argv[]) {

	World world;
	
	boost::uint32_t id = 1;
	
	world.add(std::make_shared<TestRoom>(id++, "Test room 1"));
	world.add(std::make_shared<TestRoom>(id++, "Test room 2"));
	world.add(std::make_shared<TestRoom>(id++, "Test room 3"));
	
	::signal(SIGINT, SigHandler);
	::signal(SIGTERM, SigHandler);
	
	{
	TcpServer server(4004, boost::bind(make_echo, boost::ref(world), _1));
	
	std::cout << "Sleeping..." << std::endl;
	
	//std::unique_lock<std::mutex> lock(sleepy_mutex);
	//sleepy_variable.wait(lock,[]{return done;});
	
	//std::cout << "Main thread woke up!" << std::endl;
	
	while(!done) {
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	}
	std::cout << "Exiting..." << std::endl;
	

return 0;
}

void SigHandler(int sign) {
	if (sign == SIGINT || sign == SIGTERM) {
		::signal(sign,SIG_IGN);
		std::cout << "Received shutdown sig!" << std::endl;
		
		//std::unique_lock<std::mutex> lock(sleepy_mutex);
		//std::cout << "Sleepy mutex locked!" << std::endl;
		done = true;
		//sleepy_variable.notify_one();
		//std::cout << "Woke up main thread." << std::endl;
		
	}
}
