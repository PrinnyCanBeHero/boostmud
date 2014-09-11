#ifndef ROOM_HPP_
#define ROOM_HPP_

#include <string>
#include <vector>

#include <boost/cstdint.hpp>

namespace boostMUD 
{

namespace world 
{

class Exit {
public:
	Exit(const std::string &shortName, const std::string& description, const boost::uint32_t &destination);
	const std::string& shortName() const;
	const std::string& description() const;
	const boost::uint32_t& destination() const;
private:
	const std::string _shortName;
	const std::string _description;
	const boost::uint32_t _destination;

};

class Room {

public:
	boost::uint32_t id() const { return _id; }
	virtual const std::string& description() const = 0;
	virtual std::vector<Exit> exits() const = 0;
	
	virtual ~Room() { }
protected:
	Room(boost::uint32_t id)
	: _id(id) 
	{
	}
	
private:
	const boost::uint32_t _id;

};

}

}

#endif