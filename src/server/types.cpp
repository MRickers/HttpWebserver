#include <server/http/types.h>

namespace webserver::types {
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
}