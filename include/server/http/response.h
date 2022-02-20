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

        std::string toText(const types::MIMEType type) const;
        std::string toText(const types::HttpStatusCode type) const;
    public:
        Response();

        void Payload(const std::vector<unsigned char>& payload);
        void Payload(const std::string& payload);
        void StatusCode(const types::HttpStatusCode status);
        void ContentType(const types::MIMEType type);

        size_t PayloadSize() const;

        std::vector<unsigned char> Format() const;
    };
}