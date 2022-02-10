#pragma once
#include <server/router.h>

namespace  webserver
{
    class UrlRouter : public IRouter{
    private:
        RequestHandlerContainer request_handlers_;
    public:
        UrlRouter();

        UrlRouter(const UrlRouter&) = delete;
        UrlRouter& operator=(const UrlRouter&) = delete;

        virtual void Register(const HttpMethod method, const std::string& url, RequestHandler callback) override;
        virtual void ServeAsset(const std::string& url, const std::string& path) override;
        virtual RequestHandler GetHandler(const std::string& url) const override;
    };
    
} // namespace  webserver
