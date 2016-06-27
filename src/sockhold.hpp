#pragma once

#include <string>

namespace echat {
namespace connection {

struct SockHold {
	int				sock;
	std::string		ip;
	uint16_t		port;

	SockHold(int fd,const char* a,uint16_t p);
	~SockHold();

	void release();

	SockHold(const SockHold&) = delete;
	SockHold& operator = (const SockHold &) = delete;
};

}}

