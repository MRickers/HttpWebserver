#include <algorithm>
#include <multiplexer/select_dispatcher.h>
#include <factories/multiplexer_factory.h>
#include <factories/socket_factory.h>
#include <factories/request_handler_factory.h>
#include <logging/logging.h>

namespace webserver::multiplexer {
	SelectDispatcher::SelectDispatcher(MultiplexerPtr multiplexer, RequestHandlerPtr accept_handler, Queue queue)
		: accept_socket_(), 
		read_fds_(), 
		queue_(queue), 
		multiplexer_(std::move(multiplexer)), 
		accept_handler_(std::move(accept_handler)),
		listen_for_events_(false) {

	}

	void SelectDispatcher::AddClient(int16_t fd) {
		read_fds_.push_back(fd);
	}

	void SelectDispatcher::RemoveClient(int16_t fd) {
		if (const auto found = std::find(read_fds_.begin(), read_fds_.end(), fd); found != read_fds_.end()) {
			read_fds_.erase(found);
		}
	}

	void SelectDispatcher::Init(uint16_t port) {
		// init socket connection
		webserver::sock::ISocketTcp::Init();

		const auto master_socket = webserver::sock::SocketFactory{}.Create();

		accept_socket_ = master_socket->GetFd();

		master_socket->Bind(port);
		master_socket->Listen();
		read_fds_.push_back(accept_socket_);

	}

	void SelectDispatcher::WaitForEvents() {
		std::vector<int16_t> write_fds;
		std::vector<int16_t> except_fds;

		listen_for_events_ = true;

		while (listen_for_events_) {
			// sockets aus der queue holen

			const auto queue_size = queue_->Size();
			for (size_t i = 0; i < queue_size; i++) {
				if (const auto fd = queue_->Pop();fd.has_value()) {
					read_fds_.push_back(fd.value());
				}
			}

			const auto [result, active_fds] = multiplexer_->Select(
				read_fds_, 
				write_fds, 
				except_fds);

			for (const auto fd : active_fds) {
				if (fd == accept_socket_) {
					lLog(lDebug) << "Handling AcceptRequest";
					accept_handler_->HandleRequest(accept_socket_);
				}
				else {
					lLog(lDebug) << "Handling ReadRequest";
					const auto read_handler = webserver::multiplexer::RequestHandlerFactory().CreateReadHandler(queue_);

					read_handler->HandleRequest(fd);
				}
			}
		}

		lLog(lDebug) << "Shutting down";
	}
}