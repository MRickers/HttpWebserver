#pragma once
#include <memory>
#include <multiplexer/request_dispatcher.h>
#include <handlers/request_handler.h>
#include <util/blocking_queue.h>
#include <multiplexer/multiplexer.h>

namespace webserver::multiplexer {
	class SelectDispatcher : public webserver::multiplexer::IRequestDispatcher {
		using FDContainer = std::vector<int16_t>;
		using Queue = std::shared_ptr<webserver::util::BlockingQueue<int16_t>>;
	private:
		int16_t accept_socket_;
		FDContainer read_fds_;
		Queue queue_;
		MultiplexerPtr multiplexer_;
		RequestHandlerPtr accept_handler_;
	public:
		SelectDispatcher(MultiplexerPtr multiplexer, RequestHandlerPtr accept_handler, Queue queue);
		virtual void AddClient(int16_t fd) override;
		virtual void RemoveClient(int16_t fd) override;

		virtual void Init(uint16_t port) override;
		virtual std::vector<int16_t> WaitForEvents() override;
	};
}