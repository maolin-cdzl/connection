#pragma once

#include <string>
#include <memory>

namespace echat {
namespace connection {

class Connection {
private:
	int				m_sock;
	std::string		m_ip;
	uint16_t		m_port;
	uint64_t		m_ts_create;
	uint64_t		m_ts_last_read;
	uint64_t		m_ts_last_write;

	Connection(const Connection&) = delete;
	Connection(Connection&&) = delete;
	Connection& operator = (const Connection &) = delete;
	Connection& operator = (Connection &&) = delete;

	Connection();
public:
	static std::shared_ptr<Connection> accept(int listsock,bool nonblock=true);

	~Connection();

	void reset();

	inline int socket() const { return m_sock; }
	inline const std::string& ip() const { return m_ip; }
	inline uint16_t port() const { return m_port; }
	inline uint64_t ts_create() const { return m_ts_create; }
	inline uint64_t last_read() const { return m_ts_last_read; }
	inline uint64_t last_write() const { return m_ts_last_write; }

	int read(void* buf,size_t count);
	int send(const void* msg,size_t len,int flags);
};

}}

