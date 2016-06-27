#pragma once

namespace echat {
namespace connection {

struct ClientHold {
	int				sock;

	ClientHold(int fd);
	~ClientHold();

	void release();

	ClientHold(const ClientHold&) = delete;
	ClientHold& operator = (const ClientHold &) = delete;
};

}}


