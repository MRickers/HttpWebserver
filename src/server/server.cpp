#include <server/server.h>
#include <server/router.h>
#include <factories/dispatcher_factory.h>
#include <factories/multiplexer_factory.h>
#include <factories/request_handler_factory.h>
#include <util/blocking_queue.h>

namespace webserver {

	Server::Server(RouterPtr router) : Server(30001, std::move(router)) {

	}

	Server::Server(uint32_t port, RouterPtr router) : router_(std::move(router)), port_(port) {

	}

	void Server::Run() {
		auto queue = std::make_shared<webserver::util::BlockingQueue<int16_t>>();
		auto multiplexer = webserver::multiplexer::MultiplexerFactory{}.Create();
		auto accept_handler = webserver::multiplexer::RequestHandlerFactory{}.CreateAcceptHandler(queue);
		auto dispatcher = multiplexer::DispatcherFactory().Create(std::move(multiplexer), std::move(accept_handler), queue);

		dispatcher->Init(port_);


		dispatcher_->WaitForEvents();
	}
}