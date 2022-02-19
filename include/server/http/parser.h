#pragma once
#include <string>
#include <vector>
#include <server/http/request.h>
#include <server/http/response.h>

namespace webserver::util {
    class HttpParser {
    private:
        std::string getLineEnding(const std::string& data) const;
        void parseFirstLine(http::Request& request, const std::string& line) const;
        bool parseHeaderLine(http::Request& request, const std::string& line) const;
        void parsePayload(http::Request& request, const std::string& line) const;
    public:
        http::Request Parse(const std::string& data) const;

    };
}