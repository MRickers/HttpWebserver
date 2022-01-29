#pragma once
#include <memory>

namespace webserver {
	class Router {};

	using RouterPtr = std::shared_ptr<Router>;

	class Server {
	private:
		RouterPtr router_;
		uint32_t port_;
	public:
		Server();
		Server(uint32_t port);
		Server(const Server&) = delete;
		Server& operator=(const Server&) = delete;
		void Run();
	};


}