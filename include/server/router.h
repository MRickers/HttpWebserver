#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>

namespace webserver {
	using RequestHandler = std::function<void()>;
	using RequestHandlerContainer = std::unordered_map<std::string, RequestHandler>;

	enum class HttpMethod : uint16_t {
		Get,
		Post,
		Update,
		Delete
	};

	class IRouter {
	public:
		~IRouter() {}
		virtual void Register(const HttpMethod method, const std::string& url, RequestHandler callback) = 0;
		virtual void ServeAsset(const std::string& url, const std::string& path) = 0;
		virtual RequestHandler GetHandler(const std::string& url) const = 0;
	};
	
	using RouterPtr = std::unique_ptr<IRouter>;
}