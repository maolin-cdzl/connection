#include <g3log/g3log.hpp>
#include "connectionio.hpp"


namespace echat {
namespace connection {

ConnectionIO::ConnectionIO(const std::shared_ptr<Connection>& conn,struct ev_loop* loop,const readable_callback_t& readable,const broken_callback_t& broken) :
	m_loop(loop),
	m_connect(conn),
	m_readable(readable),
	m_broken(broken)
{
	ev_io_init(&m_io_w,&ConnectionIO::ioAdapter,m_connect->socket(),EV_WRITE);
	m_io_w.data = this;

	ev_io_init(&m_io_r,&ConnectionIO::ioAdapter,m_connect->socket(),EV_READ);
	m_io_r.data = this;
	ev_io_start(m_loop,&m_io_r);
}

ConnectionIO::~ConnectionIO() {
	stop();
}

void ConnectionIO::stop() {
	if( ev_is_active(&m_io_w) ) {
		ev_io_stop(m_loop,&m_io_w);
	}
	if( ev_is_active(&m_io_r) ) {
		ev_io_stop(m_loop,&m_io_r);
	}
}

void ConnectionIO::send(const google::protobuf::Message& msg) {
	// TODO
	if( m_writebuf.empty() ) {
		if( ! msg.SerializeToString(&m_writebuf) ) {
			LOG(FATAL) << "Can not serialize msg: " << msg.GetTypeName();
			return;
		}
	} else {
		std::string buf = msg.SerializeAsString();
		if( buf.empty() ) {
		}
	}
}

void ConnectionIO::handleReadable() {
	char buf[512];
	int len = m_connect->read(buf,sizeof(buf));
	if( 0 == len ) {
		stop();
		m_broken(this);
		return;
	}
	//TODO
	if( m_readbuf.empty() ) {
	} else {
	}
}

void ConnectionIO::handleWritable() {
}

void ConnectionIO::handleError() {
	LOG(FATAL) << "ConnectionIO handle error,this should not be happened";
}

void ConnectionIO::ioAdapter(struct ev_loop* loop,ev_io* w,int revent) {
	(void)loop;
	ConnectionIO* self = (ConnectionIO*)(w->data);
	if( revent & EV_READ ) {
		self->handleReadable();
	} else if( revent & EV_WRITE ) {
		self->handleWritable();
	} else if( revent & EV_ERROR ) {
		self->handleError();
	}
}

}}

