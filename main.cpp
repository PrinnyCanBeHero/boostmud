#include "world.hpp"
#include "tcpserver.hpp"

#include <signal.h>

#include <iostream>

using namespace boostMUD::world;
using namespace boostMUD::util;

boost::mutex sleepy_mutex;
boost::condition_variable sleepy_variable;

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

TcpSession::pointer make_echo(boost::asio::io_service& io)
{
	return std::make_shared<EchoSession>(io);
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
	
	TcpServer server(4004, make_echo);
	
	std::cout << "Sleeping..." << std::endl;
	
	//boost::mutex::scoped_lock lock(sleepy_mutex);
	//sleepy_variable.wait(lock);
	
	//std::cout << "Main thread woke up!" << std::endl;
	
	while(!done) {
		boost::this_thread::sleep(boost::posix_time::milliseconds(1000));
	}
	
	std::cout << "Exiting..." << std::endl;
	

return 0;
}

void SigHandler(int signal) {
	if (signal == SIGINT || signal == SIGTERM) {
		std::cout << "Received shutdown sig!" << std::endl;
		//::signal(signal,SIG_IGN);
		//boost::mutex::scoped_lock lock(sleepy_mutex);
		//std::cout << "Sleepy mutex locked!" << std::endl;
		//sleepy_variable.notify_all();
		//std::cout << "Woke up main thread." << std::endl;
		done = true;
	}
}
