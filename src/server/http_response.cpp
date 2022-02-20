#include <server/http/response.h>
#include <server/http/types.h>
#include <util/datetime.h>
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
            payload_.clear();
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
        std::stringstream s;
        util::Datetime datetime{};

        s << "HTTP/1.1 ";
        s << toText(status_code_) << types::WINDOWS_ENDINGS;
        s << "Date: " << datetime.DateHTTPFormat() << types::WINDOWS_ENDINGS;
        s << "Content-Type: " << toText(content_type_) << types::WINDOWS_ENDINGS;
        if(!payload_.empty()) {
            s << "Content-Length: " << payload_.size() << types::WINDOWS_ENDINGS;
        }
        s << types::WINDOWS_ENDINGS;
        const auto header = s.str();

        std::vector<unsigned char> vec{header.cbegin(), header.cend()};
        vec.insert(vec.cend(), payload_.cbegin(), payload_.cend());
        return vec;
    }

    std::string Response::toText(const types::MIMEType type) const {
        if(type == types::MIMEType::ApplicationJson) {
            return "application/json";
        }  else if(type == types::MIMEType::ApplicationOctetStream) {
            return "application/octet-stream";
        }  else if(type == types::MIMEType::ApplicationPdf) {
            return "application/pdf";
        }  else if(type == types::MIMEType::ApplicationXhtmlXml) {
            return "application/xml";
        }  else if(type == types::MIMEType::ApplicationXml) {
            return "application/xml";
        }  else if(type == types::MIMEType::ApplicationXWWWFormUrlEncode) {
            return "application/x-www-form-url-encode";
        }  else if(type == types::MIMEType::FontEot) {
            return "font/eot";
        }  else if(type == types::MIMEType::FontOtf) {
            return "font/otf";
        }  else if(type == types::MIMEType::FontTtf) {
            return "font/ttf";
        }  else if(type == types::MIMEType::FontWoff2) {
            return "font/woof2";
        }  else if(type == types::MIMEType::FontWoff) {
            return "font/woof";
        }  else if(type == types::MIMEType::ImageBmp) {
            return "image/bmp";
        }  else if(type == types::MIMEType::ImageGIF) {
            return "image/gif";
        }  else if(type == types::MIMEType::ImageIco) {
            return "image/ico";
        }  else if(type == types::MIMEType::ImageJpeg) {
            return "image/jpeg";
        }  else if(type == types::MIMEType::ImagePng) {
            return "image/png";
        }  else if(type == types::MIMEType::ImageSvg) {
            return "image/svg";
        }  else if(type == types::MIMEType::TextCss) {
            return "text/css";
        }  else if(type == types::MIMEType::TextHtml) {
            return "text/html";
        }  else if(type == types::MIMEType::TextJs) {
            return "text/javascript";
        }  else if(type == types::MIMEType::TextPlain) {
            return "text/plain";
        }  else if(type == types::MIMEType::Unknown) {
            return "text/plain";
        }  else {
            return "text/plain";
        }
    }

    size_t Response::PayloadSize() const {
        return payload_.size();
    }

    std::string Response::toText(const types::HttpStatusCode status) const {
        if(status == types::HttpStatusCode::OK) {
            return "200 OK";
        }  else if(status == types::HttpStatusCode::Created) {
            return "201 Created";
        }  else if(status == types::HttpStatusCode::MovedPermanently) {
            return "301 Moved Permanently";
        }  else if(status == types::HttpStatusCode::MovedTemporarily) {
            return "302 Moved Temporarily";
        }  else if(status == types::HttpStatusCode::SeeOther) {
            return "303 See Other";
        }  else if(status == types::HttpStatusCode::BadRequest) {
            return "400 BadRequest";
        }  else if(status == types::HttpStatusCode::Unauthorized) {
            return "401 Unauthorized";
        }  else if(status == types::HttpStatusCode::Forbidden) {
            return "403 Forbidden";
        }  else if(status == types::HttpStatusCode::NotFound) {
            return "404 NotFound";
        }  else if(status == types::HttpStatusCode::MethodNotAllowed) {
            return "405 Method Not Allowed";
        }  else if(status == types::HttpStatusCode::RequestTimeout) {
            return "408 Request Timeout";
        }  else if(status == types::HttpStatusCode::LengthRequired) {
            return "411 Length Required";
        }  else if(status == types::HttpStatusCode::PayloadTooLarge) {
            return "413 Payload Too Large";
        }  else if(status == types::HttpStatusCode::UriTooLong) {
            return "414 Uri Too Long";
        }  else if(status == types::HttpStatusCode::UnsuportedMIMEType) {
            return "415 Unsuported MIME Type";
        }  else if(status == types::HttpStatusCode::TooManyRequests) {
            return "429 Too Many Requests";
        }  else if(status == types::HttpStatusCode::RequestHeaderFieldsTooLarge) {
            return "431 Request Header Fields Too Large";
        }  else if(status == types::HttpStatusCode::InternalError) {
            return "500 Internal Error";
        }  else if(status == types::HttpStatusCode::NotImplemented) {
            return "501 Not Implemented";
        }  else if(status == types::HttpStatusCode::ServerUnavailable) {
            return "503 Server Unavailable";
        }  else if(status == types::HttpStatusCode::HttpVersionNotSupported) {
            return "505 Http Version Not Supported";
        }  else {
            return "400 BadRequest";
        }
    }
}