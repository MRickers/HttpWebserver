#pragma once
#include <functional>
#include <unordered_map>
#include <server/http/request.h>
#include <server/http/response.h>

namespace  webserver
{
    using RequestHandler = std::function<http::Response(const http::Request& request)>;
	using RequestHandlerContainer = std::unordered_map<std::string, RequestHandler>;

    class UrlRouter{
    private:
        RequestHandlerContainer request_get_handlers_;
        RequestHandlerContainer request_post_handlers_;
        RequestHandlerContainer request_update_handlers_;
        RequestHandlerContainer request_delete_handlers_;

        RequestHandlerContainer getHandlers(const types::HttpMethod method) const;
        http::Response serveFile(const filesystem::path& path) const;
    public:
        UrlRouter();

        UrlRouter(const UrlRouter&) = delete;
        UrlRouter& operator=(const UrlRouter&) = delete;

        void Register(const types::HttpMethod method, const std::string& url, RequestHandler callback);
        void ServeAsset(const std::string& url, const std::string& path);
        RequestHandler GetHandler(const types::HttpMethod method, const std::string& url) const;
    };
    
} // namespace  webserver
