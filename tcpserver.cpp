/*
 * TcpServer.cpp
 *
 *  Created on: Feb 28, 2013
 *      Author: cbest
 */

#include "tcpserver.hpp"
#include <iostream>

namespace boostMUD {
namespace util {

TcpServer::~TcpServer() {
	// TODO Auto-generated destructor stub
}

TcpSession::~TcpSession() {
}

void TcpSession::scheduleRead() {
	std::shared_ptr<ByteArray> d = std::make_shared<ByteArray>(preferredChunkSize());

	socket().async_read_some(
			boost::asio::buffer(d->data(), d->size()),
			boost::bind(&TcpSession::handle_read,this,shared_from_this(),d,_1,_2)
			);

}
void TcpSession::scheduleWrite(std::shared_ptr<TcpSession> s) {

	if (outgoing.dataAvailable()) {
		std::shared_ptr<ByteArray> d = outgoing.getData();

		boost::asio::async_write(socket(),
					boost::asio::buffer(d->data(), d->size()),
					boost::bind(&TcpSession::handle_write,this,s,d,_1)
					);
	}
	else {
		boost::this_thread::sleep(boost::posix_time::milliseconds(1));
		socket().get_io_service().post(boost::bind(&TcpSession::scheduleWrite,this,s));
	}


}
void TcpSession::handle_write(std::shared_ptr<TcpSession> s, std::shared_ptr<ByteArray> d, const boost::system::error_code &error) {
	if (!error) {
		scheduleWrite(s);
	}
}
void TcpSession::handle_read(std::shared_ptr<TcpSession> s, std::shared_ptr<ByteArray> d, const boost::system::error_code &error, size_t bytes_transferred) {

	if (!error) {
		ByteArray read(reinterpret_cast<const unsigned char*>(d->data()),bytes_transferred);

		handleIncoming(read);

		scheduleRead();
	}
}

void TcpSession::do_write(std::shared_ptr<ByteArray> array) {

	outgoing.addData(array);

}

void TcpSession::start() {

	initialize();
	scheduleRead();
	scheduleWrite(shared_from_this());

}



} 
} 
