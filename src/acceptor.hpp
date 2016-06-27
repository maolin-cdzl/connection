#pragma once

#include <string>
#include <memory>
#include "mtutils/thread.hpp"
#include "sockhold.hpp"

namespace echat {
namespace connection {

class Acceptor {
public:
	Acceptor(); 
	~Acceptor();
	Acceptor(const Acceptor&) = delete; 
	Acceptor& operator = (const Acceptor&) = delete;

	int start(const std::string& address,uint16_t port,const std::function<void(const std::shared_ptr<SockHold>&)>& callback);
	void stop();

private:
	void run(const std::string& address,uint16_t port,const std::function<void(const std::shared_ptr<SockHold>&)>& callback,int pipe);
	int listenSocket(const std::string& address,uint16_t port);
	void sendSignal(int pipe,char s);

private:
	std::unique_ptr<mt::Thread>			m_thread;
};

}}

