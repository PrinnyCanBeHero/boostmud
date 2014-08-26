#ifndef WORLD_HPP_
#define WORLD_HPP_

#include <map>
#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

#include "room.hpp"

namespace boostMUD
{

namespace world
{

class room_not_found : public std::exception {
public:
	room_not_found(boost::uint32_t id) 
	: std::exception(), _id(id) { }
	virtual const char* what() const noexcept {
		return "The requested room was not found.";
	}
	boost::uint32_t id() const { return _id; }
private:
	const boost::uint32_t _id;
};

class World
{
public:
	World() = default;
	Room& room(boost::uint32_t id);
	void add(std::shared_ptr<Room> room);
private:
	std::map<boost::uint32_t, std::shared_ptr<Room>> _rooms;	
};

}

}

#endif