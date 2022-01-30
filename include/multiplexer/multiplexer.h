#pragma once
#include <vector>
#include <socket/socket.h>

namespace webserver::multiplexer {
	class IMultiplexer {
	public:
		virtual ~IMultiplexer() {}

		virtual webserver::sock::Result<std::vector<int16_t>> Select(
			std::vector<int16_t>& read_fds,
			std::vector<int16_t>& write_fds,
			std::vector<int16_t>& except_fds) const = 0;
	};
}