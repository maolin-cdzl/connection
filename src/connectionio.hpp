#pragma once

#include <ev.h>
#include <google/protobuf/message.h>
#include "connection.hpp"

namespace echat {
namespace connection {

class ConnectionIO {
public:
	typedef std::function<void(const std::shared_ptr<google::protobuf::Message>&)> readable_callback_t;
	typedef std::function<void(const ConnectionIO*)>	broken_callback_t;

	ConnectionIO(const std::shared_ptr<Connection>& conn,struct ev_loop* loop,const readable_callback_t& readable,const broken_callback_t& broken);
	~ConnectionIO();

	void send(const google::protobuf::Message& msg);
	inline const std::shared_ptr<Connection>& connection() const {
		return m_connect;
	}
private:
	ConnectionIO(const ConnectionIO&) = delete;
	ConnectionIO(ConnectionIO&&) = delete;
	ConnectionIO& operator = (const ConnectionIO&) = delete;
	ConnectionIO& operator = (ConnectionIO&&) = delete;

	static void ioAdapter(struct ev_loop* loop,ev_io* w,int revent);
	void handleReadable();
	void handleWritable();
	void handleError();
	void stop();
private:
	struct ev_loop*					m_loop;
	ev_io							m_io_r;
	ev_io							m_io_w;
	std::shared_ptr<Connection>		m_connect;
	readable_callback_t				m_readable;
	broken_callback_t				m_broken;
	std::string						m_readbuf;
	std::string						m_writebuf;
};

}}

