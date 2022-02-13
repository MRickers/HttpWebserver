#pragma once
#include <server/router.h>

namespace  webserver
{
    class UrlRouter : public IRouter{
    private:
        RequestHandlerContainer request_get_handlers_;
        RequestHandlerContainer request_post_handlers_;
        RequestHandlerContainer request_update_handlers_;
        RequestHandlerContainer request_delete_handlers_;

        RequestHandlerContainer getHandlers(const types::HttpMethod method) const;
    public:
        UrlRouter();

        UrlRouter(const UrlRouter&) = delete;
        UrlRouter& operator=(const UrlRouter&) = delete;

        virtual void Register(const types::HttpMethod method, const std::string& url, RequestHandler callback) override;
        virtual void ServeAsset(const std::string& url, const std::string& path) override;
        virtual RequestHandler GetHandler(const types::HttpMethod method, const std::string& url) const override;
    };
    
} // namespace  webserver
