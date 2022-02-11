#include <util/http_parser.h>
#include <util/util.h>
#include <logging/logging.h>

namespace webserver::util {
    static constexpr char CR = 0x0D;
    static constexpr char LF = 0x0A;

    static const std::string WINDOWS_ENDINGS = {CR, LF}; 
    static const std::string UNIX_ENDINGS = {LF};
    static const std::string MAC_ENDINGS = {CR};

    Request HttpParser::Parse(const std::string& data) const {
        try {
            // determine line endings
            const std::string line_ending = getLineEnding(data);
            // split string in lines
            
        } catch(const ServerException& e) {
            lLog(lError) << e.what();
        }
        


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
}