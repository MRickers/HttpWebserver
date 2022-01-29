#include <algorithm>
#include <functional>
#include <WinSock2.h>

#include <logging/logging.h>
#include <multiplexer/select_multiplexer.h>
#include <util/util.h>

namespace webserver::multiplexer {
	static bool isFdSet(int fd, const fd_set& set) {
		if (FD_ISSET(fd, &set)) {
			return true;
		}
		return false;
	}

	SelectMultiplexer::SelectMultiplexer() {

	}

	webserver::sock::Result<std::vector<int16_t>> SelectMultiplexer::Select(
		const std::vector<int16_t>& read_fds,
		const std::vector<int16_t>& write_fds,
		const std::vector<int16_t>& except_fds) const {

		if (read_fds.empty() && write_fds.empty() && except_fds.empty()) {
			return webserver::sock::Result<std::vector<int16_t>>{webserver::sock::SocketResult::OK, {}};
		}

		fd_set read_set;
		fd_set write_set;
		fd_set except_set;

		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		FD_ZERO(&except_set);

		for (const auto& fd : read_fds) {
			FD_SET(fd, &read_set);
		}

		for (const auto& fd : write_fds) {
			FD_SET(fd, &write_fds);
		}

		for (const auto& fd : except_fds) {
			FD_SET(fd, &except_fds);
		}
		// find max fd

		std::vector<int16_t> all_fds{ read_fds.begin(), read_fds.end() };
		all_fds.insert(all_fds.end(), write_fds.begin(), write_fds.end());
		all_fds.insert(all_fds.end(), except_fds.begin(), except_fds.end());

		const auto max_fd = std::max_element(all_fds.begin(), all_fds.end());

		timeval timeout = { 0x00 };
		const long timeout_ms = 50;
		timeout.tv_sec = timeout_ms % 1000;
		timeout.tv_usec = timeout_ms * 1000;

		const auto activity = select(*max_fd + 1, &read_set, &write_set, &except_set, &timeout);

		if (activity == SOCKET_ERROR) {
			return webserver::sock::Result<std::vector<int16_t>>{ webserver::util::convertToResult(WSAGetLastError()), {} };
		}
		else if (activity < 0) {
			return webserver::sock::Result<std::vector<int16_t>>{ webserver::sock::SocketResult::TIMEDOUT, {} };
		}

		std::vector<int16_t> active_fds;

		std::copy_if(
			read_fds.begin(),
			read_fds.end(),
			std::back_inserter(active_fds), std::bind(&isFdSet, std::placeholders::_1, read_set));

		return webserver::sock::Result<std::vector<int16_t>>{ webserver::sock::SocketResult::OK, active_fds };
	}
}