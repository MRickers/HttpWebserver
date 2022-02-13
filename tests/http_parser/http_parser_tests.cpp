#include <gtest/gtest.h>
#include <util/http_parser.h>
#include <util/types.h>
#include <util/util.h>

using namespace webserver::util;
using namespace webserver::types;

TEST(HttpParser, NoLineEnding) {
	const auto http_header = "GET /hallo.html HTTP/1.1";

	HttpParser parser;

	ASSERT_THROW(parser.Parse(http_header), ServerException);
}

TEST(HttpParser, ParseHeader1) {
	const auto http_header = "GET /hallo.html HTTP/1.1\n\r";

	HttpParser parser;

	ASSERT_NO_THROW(parser.Parse(http_header));
	const auto request = parser.Parse(http_header);

	ASSERT_EQ(request.method, HttpMethod::Get);
	ASSERT_STREQ(request.url.c_str(), "/hallo.html");
	ASSERT_STREQ(request.http_version.c_str(), "1.1");
}

TEST(HttpParser, Parse) {
	const auto http_msg = "GET / HTTP/1.1\n\rHost: localhost:30001\n\rUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0\n\rAccept: */*\n\rAccept-Language: eng\n\rAccept-Encoding: deflate\n\rDNT: 1\n\rConnection: keep-alive\n\rSec-Fetch-Dest: empty\n\rSec-Fetch-Mode: cors\n\rSec-Fetch-Site: same-origin\n\rAccept-Charset: utf-8\n\rAccess-Control-Request-Headers: acrh\n\rAccess-Control-Request-Method: get\n\rContent-Type: application/json\n\rCookie: testcookie\n\rPragma: no-cache\n\rCache-Control: no-cache";
	HttpParser parser;
	
	ASSERT_NO_THROW(parser.Parse(http_msg));
  
}

int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}