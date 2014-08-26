#include <stdexcept>

#include <boost/lexical_cast.hpp>

#include "world.hpp"

namespace boostMUD
{

namespace world
{

Room& World::room(boost::uint32_t id) {
if(_rooms.find(id) == _rooms.end())
	throw room_not_found(id);

return *_rooms[id];	

}
void World::add(std::shared_ptr<Room> room) {

_rooms[room->id()] = room;

}

}

}