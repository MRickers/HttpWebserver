#pragma once
#include <string>
#include <vector>
#include <server/http/types.h>

namespace webserver::http {
    struct Request {
        std::string http_version;
        types::HttpMethod method;
        std::string host;
        std::string user_agent;
        std::string accept_language;
        types::AcceptEncoding accept_encoding;
        types::Connection connection;
        std::string cookie;
        types::MIMEType mime_type;
        uint32_t content_length;
        types::Charset charset;
        std::string url;
        std::vector<std::pair<std::string,std::string>> parameter;
        std::vector<unsigned char> payload;
    };
}