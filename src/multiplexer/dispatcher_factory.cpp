#include <factories/dispatcher_factory.h>
#include <multiplexer/select_dispatcher.h>
#include <handlers/accept_handler.h>
#include <util/blocking_queue.h>

namespace webserver::multiplexer {
	using FdQueue = std::shared_ptr<webserver::util::BlockingQueue<int16_t>>;

	DispatcherPtr DispatcherFactory::Create(MultiplexerPtr multiplexer, RequestHandlerPtr accept_handler, Queue queue) const {
		return std::make_unique<SelectDispatcher>(multiplexer, accept_handler, queue);
	}
}