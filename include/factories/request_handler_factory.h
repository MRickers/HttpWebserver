#pragma once
#include <memory>
#include <handlers/request_handler.h>
#include <util/blocking_queue.h>

namespace webserver::multiplexer{
	using HandlerPtr = std::unique_ptr<IRequestHandler>;
	using FdQueue = std::shared_ptr<webserver::util::BlockingQueue<int16_t>>;

	class RequestHandlerFactory {
	public:
		static HandlerPtr CreateAcceptHandler(FdQueue queue);
		static HandlerPtr CreateReadHandler(FdQueue queue);
	};
}