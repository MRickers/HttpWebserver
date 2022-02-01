#include <factories/request_handler_factory.h>
#include <handlers/accept_handler.h>
#include <handlers/read_handler.h>

namespace webserver::multiplexer {

	HandlerPtr RequestHandlerFactory::CreateAcceptHandler(FdQueue queue) {
		return std::make_unique<AcceptRequestHandler>(queue);
	}

	HandlerPtr RequestHandlerFactory::CreateReadHandler(FdQueue queue) {
		return std::make_unique<ReadRequestHandler>(queue);
	}
}