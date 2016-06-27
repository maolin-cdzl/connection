#include <unistd.h>
#include <memory>

#include "sockhold.hpp"

namespace echat {
namespace connection {

SockHold::SockHold(int fd,const char* a,uint16_t p) :
	sock(fd),
	ip(a),
	port(p)
{
}

SockHold::~SockHold() {
	if( sock != -1 ) {
		close(sock);
		sock = -1;
	}
}

void SockHold::release() {
	sock = -1;
}

}}

