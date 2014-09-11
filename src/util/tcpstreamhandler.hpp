/*
 * TcpStreamHandler.h
 *
 *  Created on: Mar 4, 2013
 *      Author: cbest
 */

#ifndef TCPSTREAMHANDLER_H_
#define TCPSTREAMHANDLER_H_

#include "util/bytearray.hpp"

namespace boostMUD {
namespace util {

/**
 * Interface to implement to handle incoming bytes from a TCP stream.
 */
class TcpStreamHandler {
public:
	virtual ~TcpStreamHandler() { }
	/**
	 * The preferred buffer size to attempt to read from an incoming stream.
	 * NOTE this acts as a maximum size, not as a minimum for a read.
	 * Calls to handleIncoming will have something between 1 and this value bytes passed in.
	 */
	virtual size_t preferredChunkSize() const = 0;
	/**
	 * Method that will be called when bytes have been received.
	 */
	virtual void handleIncoming(const ByteArray& bytes) = 0;

};

}
}




#endif /* TCPSTREAMHANDLER_H_ */
