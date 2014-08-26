#include "world.hpp"

using namespace boostMUD::world;

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

int main(int argc, char* argv[]) {

	World world;
	
	boost::uint32_t id = 1;
	
	world.add(std::make_shared<TestRoom>(id++, "Test room 1"));
	world.add(std::make_shared<TestRoom>(id++, "Test room 2"));
	world.add(std::make_shared<TestRoom>(id++, "Test room 3"));
	

return 0;
}
