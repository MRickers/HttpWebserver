#include <util/http_parser.h>
#include <util/util.h>
#include <util/types.h>
#include <logging/logging.h>

namespace webserver::util {
    static constexpr char CR = 0x0D;
    static constexpr char LF = 0x0A;

    static const std::string WINDOWS_ENDINGS = {CR, LF}; 
    static const std::string UNIX_ENDINGS = {LF};
    static const std::string MAC_ENDINGS = {CR};

    Request HttpParser::Parse(const std::string& data) const {
        // determine line endings
        const std::string line_ending = getLineEnding(data);
        // split string in lines
        const auto lines = Split(data, line_ending);
        Request request;
        bool first_line = false;

        lLog(lDebug) << "Parsing Http Request";
        for(const auto& line : lines) {
            lLog(lDebug) << line;

            if(!first_line) {
                // parse first line
                parseFirstLine(request, line);
                first_line = true;
                lLog(lDebug) << "Method=" << static_cast<int>(request.method) << " url=" << request.url << " http= " << request.http_version;
            }

        }

        return request;
    }

    std::string HttpParser::getLineEnding(const std::string& data) const {
        if(data.find(WINDOWS_ENDINGS) != std::string::npos) {
            return WINDOWS_ENDINGS;
        } else if(data.find(UNIX_ENDINGS) != std::string::npos) {
            return UNIX_ENDINGS;
        } else if(data.find(MAC_ENDINGS) != std::string::npos) {
            return MAC_ENDINGS;
        } else {
            throw ServerException{"Invalid line ending", -1};
        }
    }

    void  HttpParser::parseFirstLine(Request& request, const std::string& line) const {
        const auto items = Split(line, " ");
        lLog(lDebug) << "Size " << items.size();
        lLog(lDebug) << items.at(0) << " | " << items.at(1) << " | " << items.at(2);
        try {
            // Method
            if(items.at(0).find("GET")) {
                request.method = types::HttpMethod::Get;
            } else if(items.at(0).find("POST") || items.at(0).find("PUT")) {
                request.method = types::HttpMethod::Post;
            } else if(items.at(0).find("UPDATE")) {
                request.method = types::HttpMethod::Update;
            } else if(items.at(0).find("DELETE")) {
                request.method = types::HttpMethod::Delete;
            } else {
                throw ServerException{"No Http Method found.", -1};
            }

            lLog(lDebug) << "Url " << items.at(1);
            // Url
            if(!items.at(1).empty()) {
                request.url = items.at(1);
            }

            const auto header_line = items.at(2);
            lLog(lDebug) << "Header " << header_line;
            // Http Header
            if(header_line.find("HTTP") != std::string::npos) {
                lLog(lDebug) << "HTTP found";
                const auto version = header_line.find("/");
                lLog(lDebug) << "version=" << version;
                request.http_version = header_line.substr(version+1);
            } else {
                throw ServerException{"No Http Header found", -2};
            }
        } catch(const std::out_of_range& e) {
            lLog(lWarn) << e.what();
            throw ServerException{"Http Parsing failed", -3};
        }
    }
}