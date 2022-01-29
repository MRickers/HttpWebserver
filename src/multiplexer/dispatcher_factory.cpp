#include <factories/dispatcher_factory.h>
#include <multiplexer/select_dispatcher.h>
#include <handlers/accept_handler.h>
#include <util/blocking_queue.h>

namespace webserver::multiplexer {
	using FdQueue = std::shared_ptr<webserver::util::BlockingQueue<int16_t>>;

	DispatcherPtr DispatcherFactory::Create() const {
		const auto fd_queue = std::make_shared<webserver::util::BlockingQueue<int16_t>>();
		return std::make_unique<SelectDispatcher>(std::make_unique<AcceptRequestHandler>(fd_queue), fd_queue);
	}
}