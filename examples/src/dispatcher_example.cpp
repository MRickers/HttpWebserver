#include <logging/logging.h>
#include <server/server.h>
#include <socket/socket.h>
#include <factories/router_factory.h>
#include <factories/dispatcher_factory.h>
#include <factories/multiplexer_factory.h>
#include <factories/request_handler_factory.h>
#include <factories/dispatcher_factory.h>
#include <util/blocking_queue.h>

int main() {
	lLog(lDebug) << "Dispatcher example";
	auto router = webserver::RouterFactory().Create();

	const uint32_t port = 30001;
	webserver::Server server{port, std::move(router)};

	try {
		server.Run();

	}
	catch (const webserver::sock::SocketException& e) {
		lLog(lError) << e.what();
	}



	return 0;
}