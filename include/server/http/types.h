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
        TextXml,
        TextCss,
        TextJs,

        Unknown
    };

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