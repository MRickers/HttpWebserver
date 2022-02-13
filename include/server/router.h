#pragma once
#include <unordered_map>
#include <memory>
#include <string>
#include <functional>
#include <server/http/request.h>
#include <server/http/response.h>
#include <util/types.h>

namespace webserver {
	using RequestHandler = std::function<http::HttpResponse(const http::HttpRequest& request)>;
	using RequestHandlerContainer = std::unordered_map<std::string, RequestHandler>;

	class IRouter {
	public:
		~IRouter() {}
		virtual void Register(const types::HttpMethod method, const std::string& url, RequestHandler callback) = 0;
		virtual void ServeAsset(const std::string& url, const std::string& path) = 0;
		virtual RequestHandler GetHandler(const types::HttpMethod method, const std::string& url) const = 0;
	};
	
	using RouterPtr = std::unique_ptr<IRouter>;
}