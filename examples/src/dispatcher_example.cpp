#include <logging/logging.h>
#include <server/server.h>

int main() {
	lLog(lDebug) << "Dispatcher example";

	webserver::Server server;

	server.Run();



	return 0;
}