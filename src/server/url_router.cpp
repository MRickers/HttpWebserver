#include <server/url_router.h>
#include <logging/logging.h>
#include <util/util.h>
#include <server/http/types.h>

namespace webserver {
    	UrlRouter::UrlRouter() : request_get_handlers_(), request_post_handlers_(), request_update_handlers_(), request_delete_handlers_() {

        }

        void UrlRouter::ServeAsset(const std::string& url, const std::string& path) {
            Register(types::HttpMethod::Get, url, [url, path, this](const http::Request request)->http::Response {
                const auto childpath = request.url.substr(url.length()-1);

                return this->serveFile(filesystem::path(filesystem::current_path() / path / childpath));
            });
        }

        http::Response UrlRouter::serveFile(const filesystem::path& path) const {
            http::Response response;
            try {

                response.ContentType(types::findMimeType(path.extension().string()));
                response.Payload(util::ReadFile(path));

                lLog(lDebug) << "Serving file " << path.string() << " with " << response.PayloadSize();

                response.StatusCode(types::HttpStatusCode::OK);

            } catch(const util::ServerException& e) {
                lLog(lError) << e.what();
                response.StatusCode(types::HttpStatusCode::NotFound);
            }
            return response;
        }

		void UrlRouter::Register(const types::HttpMethod method, const std::string& url, RequestHandler callback) {
            try {
                auto handlers = getHandlers(method);

                if(const auto url_ = handlers.find(url); url_ != handlers.end()) {
                    lLog(lDebug) << "Register url: " << url;
                    const auto [it, success] = handlers.insert({url, callback});
                    if(!success) {
                        lLog(lError) << "Inserting url: " << url << " failed";
                        std::string msg = "Inserting url: ";
                        msg += msg + " failed";
                        throw webserver::util::ServerException{msg, -1};
                    }
                } else {
                    lLog(lWarn) << "Url: " << url << " already registrered";
                    std::string msg = "Url: ";
                    msg += url + " already registered";
                    throw webserver::util::ServerException{msg, -2};
                }
            } catch(const webserver::util::ServerException& e) {
                lLog(lError) << e.what();
            }
        }


		RequestHandler UrlRouter::GetHandler(const types::HttpMethod method, const std::string& url) const {
            const auto handlers = getHandlers(method);

            if(const auto url_ = handlers.find(url); url_ != handlers.end()) {
                return url_->second;
            }
            std::string msg = "Url: ";
            msg += url + " not found";
            throw webserver::util::ServerException{msg, -1};
        }

        RequestHandlerContainer UrlRouter::getHandlers(const types::HttpMethod method) const {
            if(method == types::HttpMethod::Get) {
                return request_get_handlers_;
            } else if(method == types::HttpMethod::Post) {
                return request_post_handlers_;
            } else if(method == types::HttpMethod::Update) {
                return request_update_handlers_;
            } else if(method == types::HttpMethod::Delete) {
                return request_delete_handlers_;
            } else {
                std::string msg = "Method ";
                msg += std::to_string(static_cast<int16_t>(method)) + " invalid";
                lLog(lWarn) << "Http Method " << static_cast<int16_t>(method) << " invalid";
                throw webserver::util::ServerException{msg, -1};
            }
        }
}