#pragma once
#include <memory>
#include <handlers/request_handler.h>
#include <util/blocking_queue.h>

namespace webserver::multiplexer {
	using FdQueue = std::shared_ptr<webserver::util::BlockingQueue<int16_t>>;

	class AcceptRequestHandler : public IRequestHandler {
	private:
		FdQueue queue_;
	public:
		AcceptRequestHandler(FdQueue queue);
		AcceptRequestHandler(const AcceptRequestHandler&) = delete;
		AcceptRequestHandler& operator=(const AcceptRequestHandler&) = delete;

		virtual void HandleRequest(int fd) override;
	};
}