#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ev.h>
#include "acceptor.hpp"


namespace echat {
namespace connection {

static void onPipeReadable(struct ev_loop* loop,ev_io* w,int revents);
static void onListenSocketReadable(struct ev_loop* loop,ev_io* w,int revents);

Acceptor::Acceptor() {
}

Acceptor::~Acceptor() {
	stop();
}

int Acceptor::start(const std::string& address,uint16_t port,const std::function<void(const std::shared_ptr<SockHold>&)>& callback) {
	if( m_thread != nullptr ) {
		return -1;
	}

	m_thread.reset(new mt::Thread(std::bind(&Acceptor::run,this,address,port,callback,std::placeholders::_1)));
	if( -1 == m_thread->start() ) {
		m_thread.reset();
		return -1;
	} else {
		return 0;
	}
}

void Acceptor::stop() {
	if( m_thread != nullptr ) {
		m_thread->stop();
		m_thread.reset();
	}
}

void Acceptor::run(const std::string& address,uint16_t port,const std::function<void(const std::shared_ptr<SockHold>&)>& callback,int pipe) {
	int sock = listenSocket(address,port);
	if( -1 == sock ) {
		sendSignal(pipe,-1);
		return;
	}
	std::function<void(const std::shared_ptr<SockHold>&)> cb(callback);

	struct ev_loop* loop = ev_loop_new(0);
	ev_io wp; // pipe watcher
	ev_io wl; // listen watcher

	ev_io_init(&wp,onPipeReadable,pipe,EV_READ);
	ev_io_start(loop,&wp);
	ev_io_init(&wl,onListenSocketReadable,sock,EV_READ);
	wl.data = &cb;
	ev_io_start(loop,&wl);

	sendSignal(pipe,0);

	ev_run(0);

	ev_loop_destroy(loop);
	close(sock);
}

int Acceptor::listenSocket(const std::string& address,uint16_t port) {
	struct sockaddr_in addr;
	memset(&addr,0,sizeof(addr));
	addr.sin_family = AF_INET;

	if( address.empty() ) {
		addr.sin_addr.s_addr = INADDR_ANY;
	} else if( 0 == inet_aton(address.c_str(),&(addr.sin_addr)) ) {
		return -1;
	}
	addr.sin_port = htons(port);

	int sock = socket(AF_INET,SOCK_STREAM | SOCK_NONBLOCK,0);
	if( -1 == sock ) {
		return -1;
	}
	if( -1 == bind(sock,(struct sockaddr*)&addr,sizeof(addr)) ) {
		close(sock);
		return -1;
	}
	if( -1 == listen(sock,1000) ) {
		close(sock);
		return -1;
	}
	return sock;
}

void sendSignal(int pipe,char s) {
	write(pipe,&s,1);
}


static void onPipeReadable(struct ev_loop* loop,ev_io* w,int revents) {
	(void)w; (void)revents;
	ev_break(loop,EVBREAK_ALL);
}

static void onListenSocketReadable(struct ev_loop* loop,ev_io* w,int revents) {
	(void)loop;
	if( revents & EV_READ ) {
		struct sockaddr_in addr;
		socklen_t addrlen = sizeof(addr);
		int sock = accept(w->fd,(struct sockaddr*)&addr,&addrlen);
		if( -1 != sock ) {
			char ip[32];
			inet_ntop(addr.sin_family,&(addr.sin_addr),ip,sizeof(ip));
			uint16_t port = ntohs(addr.sin_port);

			std::function<void(const std::shared_ptr<SockHold>&)>* cb = (std::function<void(const std::shared_ptr<SockHold>&)>*)w->data;
			(*cb)(std::shared_ptr<SockHold>(new SockHold(sock,ip,port)));
		}
	}
}
}}
