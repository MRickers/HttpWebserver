#pragma once
#include <memory>
#include <server/router.h>
#include <multiplexer/request_dispatcher.h>
namespace webserver {
	class Server {
	private:
		webserver::multiplexer::DispatcherPtr dispatcher_;
		RouterPtr router_;
		uint32_t port_;
	public:
		Server(RouterPtr router);
		Server(uint32_t port, RouterPtr router);
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;
		void Run();
	};


}