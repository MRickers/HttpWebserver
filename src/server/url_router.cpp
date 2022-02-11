#include <server/url_router.h>
#include <logging/logging.h>
#include <util/util.h>

namespace webserver {
    	UrlRouter::UrlRouter() : request_get_handlers_(), request_post_handlers_(), request_update_handlers_(), request_delete_handlers_() {

        }

        void UrlRouter::ServeAsset([[maybe_unused]]const std::string& url, [[maybe_unused]]const std::string& path) {

        }

		void UrlRouter::Register([[maybe_unused]]const HttpMethod method, const std::string& url, RequestHandler callback) {
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


		RequestHandler UrlRouter::GetHandler(const HttpMethod method, const std::string& url) const {
            const auto handlers = getHandlers(method);

            if(const auto url_ = handlers.find(url); url_ != handlers.end()) {
                return url_->second;
            }
            std::string msg = "Url: ";
            msg += url + " not found";
            throw webserver::util::ServerException{msg, -1};
        }

        RequestHandlerContainer UrlRouter::getHandlers(const HttpMethod method) const {
            if(method == HttpMethod::Get) {
                return request_get_handlers_;
            } else if(method == HttpMethod::Post) {
                return request_post_handlers_;
            } else if(method == HttpMethod::Update) {
                return request_update_handlers_;
            } else if(method == HttpMethod::Delete) {
                return request_delete_handlers_;
            } else {
                std::string msg = "Method ";
                msg += std::to_string(static_cast<int16_t>(method)) + " invalid";
                lLog(lWarn) << "Http Method " << static_cast<int16_t>(method) << " invalid";
                throw webserver::util::ServerException{msg, -1};
            }
        }
}