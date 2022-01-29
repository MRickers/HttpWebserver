#include <algorithm>
#include <multiplexer/select_dispatcher.h>
#include <factories/multiplexer_factory.h>
#include <factories/socket_factory.h>
#include <logging/logging.h>

namespace webserver::multiplexer {
	SelectDispatcher::SelectDispatcher(AcceptHandler accept_handler, Queue queue)
		: accept_socket_(), read_fds_(), accept_handler_(std::move(accept_handler)), queue_(queue) {

	}

	void SelectDispatcher::AddClient(int16_t fd) {
		read_fds_.push_back(fd);
	}

	void SelectDispatcher::RemoveClient(int16_t fd) {
		if (const auto found = std::find(read_fds_.begin(), read_fds_.end(), fd); found != read_fds_.end()) {
			read_fds_.erase(found);
		}
	}

	void SelectDispatcher::HandleEvents(uint16_t port) {
		// init socket connection
		webserver::sock::ISocketTcp::Init();

		const auto master_socket = webserver::sock::SocketFactory{}.Create(accept_socket_);
		const auto multiplexer = webserver::multiplexer::MultiplexerFactory().Create();


		master_socket->Bind(port);
		master_socket->Listen();
		read_fds_.push_back(accept_socket_);

		while (true) {
			// sockets aus der queue holen

			const auto queue_size = queue_->Size();
			for (int i = 0; i < queue_size; i++) {
				if (const auto fd = queue_->Pop();fd.has_value()) {
					read_fds_.push_back(fd.value());
				}
			}

			const auto [result, active_fds] = multiplexer->Select(
				read_fds_, 
				std::vector<int16_t>{}, 
				std::vector<int16_t>{});

			for (const auto fd : read_fds_) {
				if (fd == accept_socket_) {
					lLog(lDebug) << "Handling AcceptRequest";
					accept_handler_->HandleRequest(accept_socket_);
				}
				else {
					lLog(lDebug) << "Handling ReadRequest";

				}
			}


		}
	}
}