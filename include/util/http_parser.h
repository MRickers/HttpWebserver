#pragma once
#include <string>
#include <vector>
#include <util/types.h>

namespace webserver::util {
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
        types::Charset charset;
        std::string url;
        std::vector<std::pair<std::string,std::string>> parameter;
        std::vector<unsigned char> payload;
    };

    class HttpParser {
    private:
        std::string getLineEnding(const std::string& data) const;
        void parseFirstLine(Request& request, const std::string& line) const;
        void parseHeaderLine(Request& request, const std::string& line) const;
    public:
        Request Parse(const std::string& data) const;

    };
}