#pragma once

namespace webserver::types {
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
}