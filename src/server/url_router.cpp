#include <server/url_router.h>
#include <logging/logging.h>
#include <util/util.h>

namespace webserver {
    	UrlRouter::UrlRouter() : request_handlers_() {

        }

		void UrlRouter::Register(const std::string& url, RequestHandler callback) {
            if(const auto url_ = request_handlers_.find(url); url_ != request_handlers_.end()) {
                lLog(lDebug) << "Register url: " << url;
                const auto [it, success] = request_handlers_.insert({url, callback});
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
        }

		RequestHandler UrlRouter::GetHandler(const std::string& url) const {
            if(const auto url_ = request_handlers_.find(url); url_ != request_handlers_.end()) {
                return url_->second;
            }
            std::string msg = "Url: ";
            msg += url + " not found";
            throw webserver::util::ServerException{msg, -1};
        }
}