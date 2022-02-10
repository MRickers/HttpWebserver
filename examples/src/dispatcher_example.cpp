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
	auto queue = std::make_shared<webserver::util::BlockingQueue<int16_t>>();
	auto multiplexer = webserver::multiplexer::MultiplexerFactory{}.Create();
	auto accept_handler = webserver::multiplexer::RequestHandlerFactory{}.CreateAcceptHandler(queue);
	auto dispatcher = webserver::multiplexer::DispatcherFactory().Create(std::move(multiplexer), std::move(accept_handler), queue);

	const uint32_t port = 30001;
	dispatcher->Init(port);
	webserver::Server server{port, std::move(dispatcher), std::move(router)};

	try {
		server.Run();

	}
	catch (const webserver::sock::SocketException& e) {
		lLog(lError) << e.what();
	}



	return 0;
}