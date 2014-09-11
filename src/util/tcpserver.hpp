/*
 * TcpServer.h
 *
 *  Created on: Feb 28, 2013
 *      Author: cbest
 */

#ifndef TcpServer_H_
#define TcpServer_H_

#include <memory>

#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <boost/signals.hpp>
#include <boost/signals2.hpp>

#include "util/tcpstreamhandler.hpp"
#include "util/threadsafequeue.hpp"

namespace boostMUD {
namespace util {

class TcpServer;

/**
 * Children must implement the handleIncoming method.
 */

class TcpSession : public TcpStreamHandler, public std::enable_shared_from_this<TcpSession> {

	friend class TcpServer;

	MultiProducerThreadSafeQueue<std::shared_ptr<ByteArray>> outgoing;

	void scheduleRead();
	void scheduleWrite(std::shared_ptr<TcpSession> s);
	void handle_write(std::shared_ptr<TcpSession> s, std::shared_ptr<ByteArray> d, const boost::system::error_code &e);
	void handle_read(std::shared_ptr<TcpSession> s, std::shared_ptr<ByteArray> d, const boost::system::error_code &e, size_t bytes_transferred);

protected:
	boost::asio::ip::tcp::socket m_socket;

	/**
	 * Post-start initialization
	 */
	virtual void initialize() { }
	/**
	 * How child classes should schedule bytes for writing.
	 */
	void do_write(std::shared_ptr<ByteArray> array);

	virtual boost::asio::ip::tcp::socket& socket() { return m_socket; }

public:
	TcpSession(boost::asio::io_service &io)
	: m_socket(io) { }
	virtual ~TcpSession();
	typedef std::shared_ptr<TcpSession> pointer;

	/**
	 * Starts up this TcpSession.
	 */
	void start();
	/**
	 * Maximum number of bytes to read at a time.
	 * This is not a minimum. Child classes will receive [1,MAX] bytes.
	 */
	virtual size_t preferredChunkSize() const { return 128; }


};

typedef boost::function<TcpSession::pointer (boost::asio::io_service&)> TcpSessionMaker;

class TcpServer {

	boost::asio::io_service m_service;
	boost::asio::io_service::work m_work;
	boost::asio::ip::tcp::acceptor m_acceptor;
	TcpSessionMaker m_creator;
	std::shared_ptr<boost::thread> m_thread;

	/**
	 * Asynchronously wait for an incoming client connection.
	 */
	void start_accept() {
		using boost::asio::ip::tcp;
		TcpSession::pointer s = m_creator(m_acceptor.get_io_service());
		m_acceptor.async_accept(s->socket(),
				boost::bind(&TcpServer::handle_accept,this,s,boost::asio::placeholders::error));
	}

	/**
	 * Handle incoming client connection.
	 */
	void handle_accept(TcpSession::pointer s, const boost::system::error_code& error) {
		if (!error) {
			s->start();
		}

		start_accept();
	}

public:
    /**
	 * Start a server on the specified port. 
	 * The creator is used to create TcpSession objects for new connections.
	 */
	TcpServer(boost::uint16_t port, TcpSessionMaker creator)
	: m_work(m_service),
	  m_acceptor(m_service,
			boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(),
			port)),
	  m_creator(creator)
	{
		start_accept();
		m_thread.reset(new boost::thread(boost::bind(&boost::asio::io_service::run,boost::ref(m_service))));

	}
	virtual ~TcpServer();
};

} 
} 
#endif /* TcpServer_H_ */
