#include <server/server.h>
#include <factories/dispatcher_factory.h>

namespace webserver {

	Server::Server() : Server(30001) {

	}

	Server::Server(uint32_t port) : router_(), port_(port) {

	}

	void Server::Run() {
		auto dispatcher = multiplexer::DispatcherFactory().Create();

		dispatcher->HandleEvents(port_);
	}
}