#pragma once
#include <memory>
#include <handlers/request_handler.h>
#include <util/blocking_queue.h>

namespace webserver::multiplexer {
	using FdQueue = std::shared_ptr<webserver::util::BlockingQueue<int16_t>>;

	class ReadRequestHandler : public IRequestHandler {
	private:
		FdQueue queue_;
	public:
		ReadRequestHandler(FdQueue queue);

		ReadRequestHandler(const ReadRequestHandler&) = delete;
		ReadRequestHandler& operator=(const ReadRequestHandler&) = delete;

		virtual void HandleRequest(int fd) override;

	};
}