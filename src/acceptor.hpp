#pragma once

#include <string>
#include <memory>
#include "etutils/mt/thread.hpp"
#include "connection.hpp"

namespace echat {
namespace connection {

class Acceptor {
public:
	Acceptor(); 
	~Acceptor();
	Acceptor(const Acceptor&) = delete; 
	Acceptor& operator = (const Acceptor&) = delete;

	int start(const std::string& address,uint16_t port,const std::function<void(const std::shared_ptr<Connection>&)>& callback);
	void stop();

private:
	void run(const std::string& address,uint16_t port,const std::function<void(const std::shared_ptr<Connection>&)>& callback,int pipe);
	int listenSocket(const std::string& address,uint16_t port);
	void sendSignal(int pipe,char s);

private:
	std::unique_ptr<etutils::Thread>			m_thread;
};

}}

