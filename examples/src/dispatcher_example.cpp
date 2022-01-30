#include <logging/logging.h>
#include <server/server.h>
#include <socket/socket.h>

int main() {
	lLog(lDebug) << "Dispatcher example";

	webserver::Server server;

	try {
		server.Run();

	}
	catch (const webserver::sock::SocketException& e) {
		lLog(lError) << e.what();
	}



	return 0;
}