#pragma once
#include <string>
#include <vector>
#include <server/http/types.h>

namespace webserver::http {
    class Response {
    private:
        std::vector<unsigned char> payload_;
        types::MIMEType content_type_;
        types::HttpStatusCode status_code_;
    public:
        Response();

        void Payload(const std::vector<unsigned char>& payload);
        void Payload(const std::string& payload);
        void StatusCode(const types::HttpStatusCode status);
        void ContentType(const types::MIMEType type);

        std::vector<unsigned char> Format() const;
    };
}