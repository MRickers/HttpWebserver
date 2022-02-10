#pragma once
#include <memory>
#include <multiplexer/request_dispatcher.h>
#include <multiplexer/multiplexer.h>
#include <handlers/request_handler.h>
#include <util/blocking_queue.h>

namespace webserver::multiplexer {
	using DispatcherPtr = std::unique_ptr<IRequestDispatcher>;
	using Queue = std::shared_ptr<webserver::util::BlockingQueue<int16_t>>;

	class DispatcherFactory {
	public:
		DispatcherPtr Create(MultiplexerPtr multiplexer, RequestHandlerPtr accept_handler, Queue queue) const;
	};
}