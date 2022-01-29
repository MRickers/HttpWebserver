#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

namespace webserver {
	using RequestHandler = std::function<void()>;
	using RequestHandlerContainer = std::unordered_map<std::string, RequestHandler>;
	class Router {
	private:
		RequestHandlerContainer request_handlers_;
	public:
		Router();
	};
}