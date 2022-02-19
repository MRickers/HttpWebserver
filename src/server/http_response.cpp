#include <server/http/response.h>
#include <logging/logging.h>

namespace webserver::http {
    Response::Response():
    payload_(),
    content_type_(types::MIMEType::TextPlain),
    status_code_(types::HttpStatusCode::OK)
    {

    }

    void Response::Payload(const std::vector<unsigned char>& payload) {
        if(!payload.empty()) {
            std::copy(payload.begin(), payload.end(), std::back_inserter(payload_));
        }
    }

    void Response::Payload(const std::string& payload) {
        Payload(std::vector<unsigned char>{payload.begin(), payload.end()});
    }

    void Response::StatusCode(const types::HttpStatusCode status) {
        status_code_ = status;
    }

    void Response::ContentType(const types::MIMEType type) {
        content_type_ = type;
    }

    std::vector<unsigned char> Response::Format() const {
        return std::vector<unsigned char>{};
    }
}