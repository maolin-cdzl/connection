#pragma once

#include <string>
#include <memory>
#include "mtutils/thread.hpp"

namespace echat {
namespace connection {

class Validator {
public:
	Validator();
	~Validator();

	int start(const std::function<void(const std::shared_ptr<ClientHold>&)>& callback);
	void stop();

private:
	void run(const std::function<void(const std::shared_ptr<ClientHold>&)>& callback,mt::MQThread* thread,int pipe);

private:
	std::unique_ptr<mt::MQThread>		m_thread;
};

}}


