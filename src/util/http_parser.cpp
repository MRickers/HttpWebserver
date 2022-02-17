#include <util/http_parser.h>
#include <util/util.h>
#include <util/types.h>
#include <logging/logging.h>
#include <utility>
#include <iterator>

namespace webserver::util {
    static constexpr uint32_t MAX_HEADER_SIZE = 64;
    static constexpr char CR = 0x0D;
    static constexpr char LF = 0x0A;

    static const std::string WINDOWS_ENDINGS = {CR, LF}; 
    static const std::string UNIX_ENDINGS = {LF};
    static const std::string MAC_ENDINGS = {CR};

    Request HttpParser::Parse(const std::string& data) const {
        // determine line endings
        const std::string line_ending = getLineEnding(data);
        // split string in lines
        auto lines = Split(data, line_ending);

        if(lines.size() > MAX_HEADER_SIZE) {
            std::string msg = "To much header lines: " + std::to_string(lines.size());
            lLog(lError) << msg;
            throw ServerException{msg, -1};
        }
        lLog(lDebug) << "Found " << lines.size() << " lines";

        try {
            Request request;
            if(lines.empty()) {
                lLog(lWarn) << "Lines empty.";
                return request;
            }
            const auto first_line = lines.front();

            lLog(lDebug) << "Parsing first line: " << first_line;
            parseFirstLine(request, first_line);
            lines.erase(lines.begin());

            for(const auto& line : lines) {
                if(parseHeaderLine(request, line)) {
                    parsePayload(request, lines.back());
                    return request;
                }
            }
            return request;
        } catch(const std::out_of_range& e) {
            lLog(lError) << e.what();
            throw ServerException{e.what(), -1};
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
            lLog(lError) << "Invalid line ending";
            throw ServerException{"Invalid line ending", -1};
        }
    }

    void HttpParser::parseFirstLine(Request& request, const std::string& line) const {
        try {
            const auto tokens = Split(line, " ");
            if(tokens.size() != 3) {
                lLog(lError) << "Token size != 3. Http Protocol invalid";
                throw ServerException{"Http Protocol invalid", -1};
            }
            // Method
            if(tokens.at(0).find("GET") != std::string::npos) {
                request.method = types::HttpMethod::Get;
            } else if(tokens.at(0).find("POST")!= std::string::npos || tokens.at(0).find("PUT")!= std::string::npos) {
                request.method = types::HttpMethod::Post;
            } else if(tokens.at(0).find("UPDATE")!= std::string::npos) {
                request.method = types::HttpMethod::Update;
            } else if(tokens.at(0).find("DELETE")!= std::string::npos) {
                request.method = types::HttpMethod::Delete;
            } else {
                lLog(lError) << "No Http Method found.";
                throw ServerException{"No Http Method found.", -1};
            }
            // Url
            auto url = tokens.at(1);
                // Parameter
            if(const auto param_offset = url.find("?"); param_offset != std::string::npos) {
                const auto params = Split(url.substr(param_offset+1), "&");
                url = url.substr(0, param_offset);
                for(const auto& param_str : params) {
                    const auto param = Split(param_str, "=");
                    if(param.size() == 2) {
                        const auto& key = Trim(param.at(0));
                        const auto& value = Trim(param.at(1));

                        if(!key.empty() && !value.empty()) {
                            request.parameter.push_back({param.at(0), param.at(1)});
                        }
                    }
                }
            }
            request.url = url;

            // Http Header
            const auto header_line = tokens.at(2);
            if(header_line.find("HTTP") != std::string::npos) {
                const auto version = header_line.find("/");
                request.http_version = header_line.substr(version+1);
            } else {
                throw ServerException{"No Http Header found", -2};
            }


        } catch(const std::out_of_range& e) {
            lLog(lWarn) << e.what();
            throw ServerException{"Http Parsing failed", -3};
        }
    }

    void HttpParser::parsePayload(Request& request, const std::string& line) const {
        if(!line.empty()) {
            if(const auto line_trimmed = Trim(line); !line_trimmed.empty()) {
                std::copy(line_trimmed.begin(), line_trimmed.end(),  std::back_inserter(request.payload));
            }
        }
    }

    bool HttpParser::parseHeaderLine(Request& request, const std::string& line) const {
        try {
            if(line.empty()) {
                return true;
            }

            const auto header = SplitOnce(line, ":");
            
            // payload line incoming
            if(header.first.empty()) {
                return false;
            }

            const auto key = Trim(header.first);
            const auto value = Trim(header.second);

            if(key.find("Host") != std::string::npos) {
                request.host = value;
            } else if(key.find("User-Agent") != std::string::npos) {
                request.user_agent = value;
            } else if(key.find("Accept-Language") != std::string::npos) {
                request.accept_language = value;
            } else if(key.find("Accept-Charset") != std::string::npos) { 
                if(value.find("utf-8") != std::string::npos) {
                    request.charset = types::Charset::utf8;
                } else if(value.find("utf-16") != std::string::npos) {
                    request.charset = types::Charset::utf16;
                } else if(value.find("ascii") != std::string::npos) {
                    request.charset = types::Charset::ascii;
                } else {
                    request.charset = types::Charset::ascii;
                }
            } else if(key.find("Accept-Encoding") != std::string::npos) {
                if(value.find("gzip") != std::string::npos) {
                    request.accept_encoding = types::AcceptEncoding::gzip;
                } else if(value.find("deflat") != std::string::npos) {
                    request.accept_encoding = types::AcceptEncoding::deflat;
                } else {
                    request.accept_encoding = types::AcceptEncoding::deflat;
                }
            } else if(key.find("Connection") != std::string::npos) {
                if(value.find("keep-alive") != std::string::npos) {
                    request.connection = types::Connection::keep_alive;
                } else if(value.find("deflat") != std::string::npos) {
                    request.connection = types::Connection::close;
                } else {
                    request.connection = types::Connection::keep_alive;
                }
            } else if(key.find("Content-Type") != std::string::npos) {
                if(value.find("application/json") != std::string::npos) {
                    request.mime_type = types::MIMEType::ApplicationJson;
                } else if(value.find("application/pdf") != std::string::npos) {
                    request.mime_type = types::MIMEType::ApplicationPdf;
                } else if(value.find("application/pxml") != std::string::npos) {
                    request.mime_type = types::MIMEType::ApplicationXhtmlXml;
                } else if(value.find("application/octet-stream") != std::string::npos) {
                    request.mime_type = types::MIMEType::ApplicationOctetStream;
                } else if(value.find("application/x-www-form-urlencoded") != std::string::npos) {
                    request.mime_type = types::MIMEType::ApplicationXWWWFormUrlEncode;
                } else if(value.find("text/plain") != std::string::npos) {
                    request.mime_type = types::MIMEType::TextPlain;
                } else if(value.find("text/javascript") != std::string::npos) {
                    request.mime_type = types::MIMEType::TextJs;
                } else if(value.find("text/xml") != std::string::npos) {
                    request.mime_type = types::MIMEType::TextXml;
                } else if(value.find("text/css") != std::string::npos) {
                    request.mime_type = types::MIMEType::TextCss;
                } else if(value.find("text/html") != std::string::npos) {
                    request.mime_type = types::MIMEType::TextHtml;
                } else if(value.find("image/bmp") != std::string::npos) {
                    request.mime_type = types::MIMEType::ImageBmp;
                } else if(value.find("image/jpeg") != std::string::npos) {
                    request.mime_type = types::MIMEType::ImageJpeg;
                } else if(value.find("image/png") != std::string::npos) {
                    request.mime_type = types::MIMEType::ImagePng;
                } else if(value.find("image/svg+xml") != std::string::npos) {
                    request.mime_type = types::MIMEType::ImageSvg;
                } else if(value.find("image/gif") != std::string::npos) {
                    request.mime_type = types::MIMEType::ImageGIF;
                } else if(value.find("image/vnd.microsoft.icon") != std::string::npos) {
                    request.mime_type = types::MIMEType::ImageIco;
                } else if(value.find("font/otf") != std::string::npos) {
                    request.mime_type = types::MIMEType::FontOtf;
                }else if(value.find("font/eot") != std::string::npos) {
                    request.mime_type = types::MIMEType::FontEot;
                }else if(value.find("font/woff") != std::string::npos) {
                    request.mime_type = types::MIMEType::FontWoff;
                }else if(value.find("font/woff2") != std::string::npos) {
                    request.mime_type = types::MIMEType::FontWoff2;
                }else if(value.find("font/ttf") != std::string::npos) {
                    request.mime_type = types::MIMEType::FontTtf;
                } else {
                    request.mime_type = types::MIMEType::TextPlain;
                }

                if(const auto charset = value.find("charset"); charset != std::string::npos) {
                    if(value.find("UTF-8", charset) != std::string::npos) {
                        request.charset = types::Charset::utf8;
                    } else if(value.find("UTF-16", charset) != std::string::npos) {
                        request.charset = types::Charset::utf16;
                    } else if(value.find("ASCII", charset) != std::string::npos) {
                        request.charset = types::Charset::ascii;
                    } else {
                        request.charset = types::Charset::ascii;
                    }
                } else {
                    request.charset = types::Charset::ascii;
                }
            } else if(key.find("Content-Length") != std::string::npos) {
                try {
                    request.content_length = std::stoul(value);
                } catch(const std::invalid_argument& e) {
                    lLog(lInfo) << "Could not convert Content-Length: " << value << ". Setting default 0";
                    request.content_length = 0;
                } catch(const std::out_of_range& e) {
                    lLog(lInfo) << "Content-Length overflow. Setting default 0";
                    request.content_length = 0;
                }
            } else {
                lLog(lInfo) << "Header not known: " << key << ":" << value;
            }
        } catch(const std::out_of_range& e) {
            lLog(lWarn) << e.what();
            throw ServerException{"Invalid Header", -1};
        }
        return false;
    }
}