#pragma once
#include <string>

namespace webserver::util {
    struct Request {
        
    };

    class HttpParser {
    private:
        std::string getLineEnding(const std::string& data) const;
    public:
        Request Parse(const std::string& data) const;

    };
}