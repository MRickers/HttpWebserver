#include <logging/logging.h>
#include <server/server.h>
#include <socket/socket.h>
#include <factories/dispatcher_factory.h>
#include <factories/multiplexer_factory.h>
#include <factories/request_handler_factory.h>
#include <factories/dispatcher_factory.h>
#include <util/blocking_queue.h>
#include <server/url_router.h>

int main() {
	lLog(lDebug) << "Dispatcher example";
	auto router = std::make_unique<webserver::UrlRouter>();
	
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