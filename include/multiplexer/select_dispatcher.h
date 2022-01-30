#pragma once
#include <memory>
#include <multiplexer/request_dispatcher.h>
#include <handlers/request_handler.h>
#include <util/blocking_queue.h>

namespace webserver::multiplexer {
	class SelectDispatcher : public webserver::multiplexer::IRequestDispatcher {
		using FDContainer = std::vector<int16_t>;
		using AcceptHandler = std::unique_ptr<webserver::multiplexer::IRequestHandler>;
		using Queue = std::shared_ptr<webserver::util::BlockingQueue<int16_t>>;

	private:
		int16_t accept_socket_;
		FDContainer read_fds_;
		AcceptHandler accept_handler_;
		Queue queue_;
	public:
		SelectDispatcher(AcceptHandler accept_handler, Queue queue);
		virtual void AddClient(int16_t fd) override;
		virtual void RemoveClient(int16_t fd) override;

		virtual void HandleEvents(uint16_t port) override;
	};
}