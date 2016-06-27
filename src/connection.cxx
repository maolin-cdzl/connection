#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <memory>
#include "etutils/common/time.hpp"

#include "connection.hpp"

namespace echat {
namespace connection {

std::shared_ptr<Connection> Connection::accept(int lsock) {
	struct sockaddr_in addr;
	socklen_t addrlen = sizeof(addr);
	int sock = ::accept(lsock,(struct sockaddr*)&addr,&addrlen);
	if( -1 == sock ) {
		return nullptr;
	}
	char ip[32];
	inet_ntop(addr.sin_family,&(addr.sin_addr),ip,sizeof(ip));
	std::shared_ptr<Connection> conn(new Connection());
	conn->m_sock = sock;
	conn->m_ip = ip;
	conn->m_port = ntohs(addr.sin_port);
	conn->m_ts_create = etutils::clock_time();
	conn->m_ts_last_read = conn->m_ts_create;

	return conn;
}


Connection::Connection() :
	m_sock(-1),
	m_port(0),
	m_ts_create(0),
	m_ts_last_read(0),
	m_ts_last_write(0)
{
}

Connection::~Connection() {
	if( m_sock != -1 ) {
		close(m_sock);
		m_sock = -1;
	}
}

void Connection::reset() {
	m_sock = -1;
	m_ip.clear();
	m_port = 0;
	m_ts_create = m_ts_last_read = m_ts_last_write = 0;
}

int Connection::read(void* buf,size_t count) {
	if( -1 == m_sock ) {
		return -1;
	}
	int r = ::read(m_sock,buf,count);
	if( r > 0 ) {
		m_ts_last_read = etutils::clock_time();
	}
	return r;
}

int Connection::send(const void* msg,size_t len,int flags) {
	if( -1 == m_sock ) {
		return -1;
	}
	int r = ::send(m_sock,msg,len,flags);
	if( r > 0 ) {
		m_ts_last_write = etutils::clock_time();
	}
	return r;
}

}}

