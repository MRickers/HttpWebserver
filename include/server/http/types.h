#pragma once
#include <string>

namespace webserver::types {
    constexpr uint32_t MAX_HEADER_SIZE = 64;
    constexpr char CR = 0x0D;
    constexpr char LF = 0x0A;

    const std::string WINDOWS_ENDINGS = {CR, LF}; 
    const std::string UNIX_ENDINGS = {LF};
    const std::string MAC_ENDINGS = {CR};

    enum class HttpMethod : uint16_t {
		Get,
		Post,
		Update,
		Delete
	};

    enum class AcceptEncoding : uint16_t {
        deflat,
        gzip
    };

    enum class Connection : int16_t {
        keep_alive,
        close
    };

    enum class MIMEType : int16_t {
        ApplicationJson,
        ApplicationXWWWFormUrlEncode,
        ApplicationXhtmlXml,
        ApplicationXml,
        ApplicationPdf,
        ApplicationOctetStream,

        ImageBmp,
        ImageJpeg,
        ImagePng,
        ImageSvg,
        ImageGIF,
        ImageIco,

        FontOtf,    // OpenFonts
        FontEot,    // Microsoft embedded OpenFonts
        FontWoff,   // Web OpenFont format
        FontWoff2,  // Web OpenFont format
        FontTtf,    // True type font


        TextHtml,
        TextPlain,
        TextCss,
        TextJs,

        Unknown
    };

    types::MIMEType findMimeType(const std::string& ending) {
        if(ending.empty()) {
            return types::MIMEType::ApplicationOctetStream;
        }

        std::string end = ending;
        if(const auto pos = ending.find_first_of("."); pos != std::string::npos) {
            end = ending.substr(pos);
        }

        if(end == ".txt") {
            return types::MIMEType::TextPlain;
        } else if(end == ".js") {
            return types::MIMEType::TextJs;
        } else if(end == ".html") {
            return types::MIMEType::TextHtml;
        } else if(end == ".css") {
            return types::MIMEType::TextCss;
        } else if(end == ".json") {
            return types::MIMEType::ApplicationJson;
        } else if(end == ".pdf") {
            return types::MIMEType::ApplicationPdf;
        } else if(end == ".xml") {
            return types::MIMEType::ApplicationXml;
        } else if(end == ".bin") {
            return types::MIMEType::ApplicationOctetStream;
        } else if(end == ".bmp") {
            return types::MIMEType::ImageBmp;
        } else if(end == ".jpg") {
            return types::MIMEType::ImageJpeg;
        } else if(end == ".jpeg") {
            return types::MIMEType::ImageJpeg;
        } else if(end == ".png") {
            return types::MIMEType::ImagePng;
        } else if(end == ".svg") {
            return types::MIMEType::ImageSvg;
        } else if(end == ".gif") {
            return types::MIMEType::ImageGIF;
        } else if(end == ".ico") {
            return types::MIMEType::ImageIco;
        } else if(end == ".otf") {
            return types::MIMEType::FontOtf;
        } else if(end == ".eot") {
            return types::MIMEType::FontEot;
        } else if(end == ".woff") {
            return types::MIMEType::FontWoff;
        } else if(end == ".woff2") {
            return types::MIMEType::FontWoff2;
        } else if(end == ".ttf") {
            return types::MIMEType::FontTtf;
        }  else {
            return types::MIMEType::ApplicationOctetStream;
        }
    }

    

    enum class Charset : int16_t {
        utf8,
        utf16,
        ascii
    };

    /*
    The status code is a three-digit integer, and the first digit identifies the general category of response:

    1xx indicates an informational message only
    2xx indicates success of some kind
    3xx redirects the client to another URL
    4xx indicates an error on the client's part
    5xx indicates an error on the server's part 
    */
    enum class HttpStatusCode : int16_t {
        OK = 200,
        Created = 201,

        MovedPermanently = 301,
        MovedTemporarily = 302,
        SeeOther = 303,

        BadRequest = 400,
        Unauthorized = 401,
        Forbidden = 403,
        NotFound = 404,
        MethodNotAllowed = 405,
        RequestTimeout = 408,
        LengthRequired = 411,
        PayloadTooLarge = 413,
        UriTooLong = 414,
        UnsuportedMIMEType = 415,
        TooManyRequests = 429,
        RequestHeaderFieldsTooLarge = 431,

        InternalError = 500,
        NotImplemented = 501,
        ServerUnavailable = 503,
        HttpVersionNotSupported = 505,
        
    };
}