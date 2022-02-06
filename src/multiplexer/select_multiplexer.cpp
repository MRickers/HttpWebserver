#include <algorithm>
#include <functional>
#include <logging/logging.h>
#include <multiplexer/select_multiplexer.h>
#include <util/util.h>

#if _WIN32
#include <WinSock2.h>

static webserver::sock::SocketResult getSocketResult() {
	return webserver::util::convertToResult(WSAGetLastError());
}

#elif __linux__
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

static const int SOCKET_ERROR = -1;

static webserver::sock::SocketResult getSocketResult() {
	return webserver::sock::SocketResult::DEFAULTERROR;
}
#endif

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
		std::vector<int16_t>& read_fds,
		std::vector<int16_t>& write_fds,
		std::vector<int16_t>& except_fds) const {

		if (read_fds.empty() && write_fds.empty() && except_fds.empty()) {
			return webserver::sock::Result<std::vector<int16_t>>{webserver::sock::SocketResult::OK, {}};
		}

		fd_set read_set;
		fd_set write_set;
		fd_set except_set;

		FD_ZERO(&read_set);
		FD_ZERO(&write_set);
		FD_ZERO(&except_set);

		for (const int& fd : read_fds) {
			FD_SET(fd, &read_set);
		}
/*
		for (const int& fd : write_fds) {
			FD_SET(fd, &write_fds);
		}

		for (const int& fd : except_fds) {
			FD_SET(fd, &except_fds);
		}
*/
		// find max fd

		std::vector<int16_t> all_fds{ read_fds.begin(), read_fds.end() };
		all_fds.insert(all_fds.end(), write_fds.begin(), write_fds.end());
		all_fds.insert(all_fds.end(), except_fds.begin(), except_fds.end());

		const auto max_fd = std::max_element(all_fds.begin(), all_fds.end());

		timeval timeout;
		const long timeout_ms = 50;
		timeout.tv_sec = timeout_ms % 1000;
		timeout.tv_usec = timeout_ms * 1000;

		const auto activity = select(*max_fd + 1, &read_set, &write_set, &except_set, &timeout);

		if (activity == SOCKET_ERROR) {
			return webserver::sock::Result<std::vector<int16_t>>{ getSocketResult(), {} };
		}
		else if (activity < 0) {
			return webserver::sock::Result<std::vector<int16_t>>{ webserver::sock::SocketResult::TIMEDOUT, {} };
		}

		std::vector<int16_t> active_fds;

		std::copy_if(
			read_fds.begin(),
			read_fds.end(),
			std::back_inserter(active_fds), std::bind(&isFdSet, std::placeholders::_1, read_set));

		read_fds.erase(std::remove_if(read_fds.begin(), read_fds.end(),
			std::bind(&isFdSet, std::placeholders::_1, read_set)), read_fds.end());

		return webserver::sock::Result<std::vector<int16_t>>{ webserver::sock::SocketResult::OK, active_fds };
	}
}