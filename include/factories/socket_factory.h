#pragma once
#include <socket/socket.h>
#include <memory>

namespace webserver::sock {
	using ISocketPtr = std::unique_ptr<ISocketTcp>;

	class SocketFactory {
	public:
		ISocketPtr Create() const;
		ISocketPtr Create(int16_t fd) const;
		ISocketPtr Create(const std::string& host, uint32_t port) const;
	};
}