#include <gtest/gtest.h>
#include <server/http/parser.h>
#include <util/util.h>

using namespace webserver::util;
using namespace webserver::types;

TEST(HttpParser, NoLineEnding) {
	const auto http_header = "GET /hallo.html HTTP/1.1";

	HttpParser parser;

	ASSERT_THROW(parser.Parse(http_header), ServerException);
}

TEST(HttpParser, NoMethod) {
	const auto http_header = "/hallo.html HTTP/1.1\n\r";

	HttpParser parser;

	ASSERT_THROW(parser.Parse(http_header), ServerException);
}

TEST(HttpParser, NoHttpVersion) {
	const auto http_header = "GET /hallo.html /1.1\n\r";

	HttpParser parser;

	ASSERT_THROW(parser.Parse(http_header), ServerException);
}

TEST(HttpParser, NoUrl) {
	const auto http_header = "GET HTTP/1.1\n\r";

	HttpParser parser;

	ASSERT_THROW(parser.Parse(http_header), ServerException);
}

TEST(HttpParser, Trash) {
	const auto http_header = "hello abcd cdef \n\r";

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

TEST(HttpParser, ParseHeader2) {
	const auto http_header = "POST /path/to/html HTTP/1.2\n\r";

	HttpParser parser;

	ASSERT_NO_THROW(parser.Parse(http_header));
	const auto request = parser.Parse(http_header);

	ASSERT_EQ(request.method, HttpMethod::Post);
	ASSERT_STREQ(request.url.c_str(), "/path/to/html");
	ASSERT_STREQ(request.http_version.c_str(), "1.2");
}

TEST(HttpParser, ParseHeader3) {
	const auto http_header = "PUT /path/to/index.html HTTP/2.0\n\r";

	HttpParser parser;

	ASSERT_NO_THROW(parser.Parse(http_header));
	const auto request = parser.Parse(http_header);

	ASSERT_EQ(request.method, HttpMethod::Post);
	ASSERT_STREQ(request.url.c_str(), "/path/to/index.html");
	ASSERT_STREQ(request.http_version.c_str(), "2.0");
}

TEST(HttpParser, ParseHeader4) {
	const auto http_header = "DELETE /path/to/index.html HTTP/2.0\n\r";

	HttpParser parser;

	ASSERT_NO_THROW(parser.Parse(http_header));
	const auto request = parser.Parse(http_header);

	ASSERT_EQ(request.method, HttpMethod::Delete);
	ASSERT_STREQ(request.url.c_str(), "/path/to/index.html");
	ASSERT_STREQ(request.http_version.c_str(), "2.0");
}

TEST(HttpParser, ParseHeader5) {
	const auto http_header = "UPDATE /path/to/index.html HTTP/2.0\n\r";

	HttpParser parser;

	ASSERT_NO_THROW(parser.Parse(http_header));
	const auto request = parser.Parse(http_header);

	ASSERT_EQ(request.method, HttpMethod::Update);
	ASSERT_STREQ(request.url.c_str(), "/path/to/index.html");
	ASSERT_STREQ(request.http_version.c_str(), "2.0");
}

TEST(HttpParser, ParseHeader6) {
	const auto http_header = "GET /path/to/index.html?hello=there&general=kenobi HTTP/1.1\n\r";

	HttpParser parser;

	ASSERT_NO_THROW(parser.Parse(http_header));
	const auto request = parser.Parse(http_header);

	ASSERT_EQ(request.method, HttpMethod::Get);
	ASSERT_STREQ(request.url.c_str(), "/path/to/index.html");
	ASSERT_STREQ(request.http_version.c_str(), "1.1");
	ASSERT_STREQ(request.parameter.at(0).first.c_str(), "hello");
	ASSERT_STREQ(request.parameter.at(0).second.c_str(), "there");
	ASSERT_STREQ(request.parameter.at(1).first.c_str(), "general");
	ASSERT_STREQ(request.parameter.at(1).second.c_str(), "kenobi");
}

TEST(HttpParser, ParseHeader7) {
	const auto http_header = "GET /path/to/index.html?dsa=das0=D=AD=SA=D00==$3%&%0===00==0ewi HTTP/1.1\n\r";

	HttpParser parser;

	ASSERT_NO_THROW(parser.Parse(http_header));
	const auto request = parser.Parse(http_header);

	ASSERT_EQ(request.method, HttpMethod::Get);
	ASSERT_STREQ(request.url.c_str(), "/path/to/index.html");
	ASSERT_STREQ(request.http_version.c_str(), "1.1");
}

TEST(HttpParser, ParseHeader8) {
	using namespace webserver;
	const auto http_header = "GET /path/to/index.html?dsa= \t& =D=AD=SA=D00==$3%&%0===00==0ewi HTTP/1.1\n\r";

	HttpParser parser;

	ASSERT_THROW(parser.Parse(http_header), util::ServerException);
}

TEST(HttpParser, ParseHeader9) {
	using namespace webserver;
	const auto http_header = "GET /path/to/index.html?dsa=\t&\t=D=AD=SA=D00==$3%&hello=there HTTP/1.1\n\r";

	HttpParser parser;

	ASSERT_NO_THROW(parser.Parse(http_header));
	const auto request = parser.Parse(http_header);
	ASSERT_STREQ(request.parameter.at(0).first.c_str(), "hello");
	ASSERT_STREQ(request.parameter.at(0).second.c_str(), "there");
}

TEST(HttpParser, Parse1) {
	using namespace webserver;

	const auto http_msg = "GET / HTTP/1.1\n\rHost: localhost:30001\n\rUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0\n\rAccept: */*\n\rAccept-Language: en\n\rAccept-Encoding: deflate\n\rDNT: 1\n\rConnection: keep-alive\n\rSec-Fetch-Dest: empty\n\rSec-Fetch-Mode: cors\n\rSec-Fetch-Site: same-origin\n\rAccess-Control-Request-Headers: acrh\n\rAccess-Control-Request-Method: get\n\rContent-Type: application/json;charset=UTF-8\n\rCookie: testcookie\n\rPragma: no-cache\n\rCache-Control: no-cache";
	HttpParser parser;
	
	ASSERT_NO_THROW(parser.Parse(http_msg));
	const auto request = parser.Parse(http_msg);

	ASSERT_EQ(request.method, HttpMethod::Get);
	ASSERT_STREQ(request.url.c_str(), "/");
	ASSERT_STREQ(request.http_version.c_str(), "1.1");

	ASSERT_EQ(request.accept_encoding, types::AcceptEncoding::deflat);
	ASSERT_EQ(request.charset, types::Charset::utf8);
	ASSERT_EQ(request.mime_type, types::MIMEType::ApplicationJson);
	ASSERT_EQ(request.connection, types::Connection::keep_alive);
	ASSERT_STREQ(request.host.c_str(), "localhost:30001");
	ASSERT_STREQ(request.user_agent.c_str(), "Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0");
	ASSERT_STREQ(request.accept_language.c_str(), "en");
}

TEST(HttpParser, Parse2) {
	using namespace webserver;

	const auto http_msg = "GET / HTTP/1.1\n\rHost: localhost:30001\n\rUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0\n\rContent-Type: application/json;charset=ASCII\n\rContent-Length: 17\n\r\n\r{'hello':'there'}";
	HttpParser parser;
	
	ASSERT_THROW(parser.Parse(http_msg), util::ServerException);
}

TEST(HttpParser, Parse3) {
	using namespace webserver;

	const auto http_msg = "GET / HTTP/1.1\r\nHost: localhost:30001\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0\r\nContent-Type: application/json;charset=ASCII\r\nContent-Length: 17\r\n\r\n{'hello':'there'}";
	HttpParser parser;
	
	ASSERT_NO_THROW(parser.Parse(http_msg));
	const auto request = parser.Parse(http_msg);

	std::string payload{request.payload.begin(), request.payload.end()};
	ASSERT_STREQ("{'hello':'there'}", payload.c_str());
	ASSERT_EQ(17, request.content_length);
}


TEST(HttpParser, Parse4) {
	using namespace webserver;

	const auto http_msg = "GET / HTTP/1.1\r\nHost: localhost:30001\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:97.0) Gecko/20100101 Firefox/97.0\r\nContent-Type: text/plain;charset=ASCII\r\nContent-Length: 17\r\n\r\nObi-Wan 'Ben' Kenobi is a fictional character in the Star Wars franchise. Within the original trilogy, Obi-Wan is a Jedi Master as a supporting character and is portrayed by English actor Alec Guinness. In the later-released prequel trilogy, a younger version of the character serves as one of the two main protagonists, alongside Anakin Skywalker, and is portrayed by Scottish actor Ewan McGregor. In the original trilogy, he is a mentor to Luke Skywalker, to whom he introduces the ways of the Jedi. After sacrificing himself in a duel against Darth Vader, Obi-Wan guides Luke through the Force in his fight against the Galactic Empire. In the prequel trilogy, set decades earlier, he is initially a Padawan (apprentice) to Jedi Master Qui-Gon Jinn, and later mentor and friend of Luke's father Anakin, who falls to the dark side of the Force and becomes Vader. The character briefly appears in the sequel trilogy as a disembodied voice, speaking to protagonist Rey. He is frequently featured as a main character in various other Star Wars media.";
	HttpParser parser;
	
	ASSERT_NO_THROW(parser.Parse(http_msg));
	const auto request = parser.Parse(http_msg);

	std::string payload{request.payload.begin(), request.payload.end()};
	ASSERT_STREQ("Obi-Wan 'Ben' Kenobi is a fictional character in the Star Wars franchise. Within the original trilogy, Obi-Wan is a Jedi Master as a supporting character and is portrayed by English actor Alec Guinness. In the later-released prequel trilogy, a younger version of the character serves as one of the two main protagonists, alongside Anakin Skywalker, and is portrayed by Scottish actor Ewan McGregor. In the original trilogy, he is a mentor to Luke Skywalker, to whom he introduces the ways of the Jedi. After sacrificing himself in a duel against Darth Vader, Obi-Wan guides Luke through the Force in his fight against the Galactic Empire. In the prequel trilogy, set decades earlier, he is initially a Padawan (apprentice) to Jedi Master Qui-Gon Jinn, and later mentor and friend of Luke's father Anakin, who falls to the dark side of the Force and becomes Vader. The character briefly appears in the sequel trilogy as a disembodied voice, speaking to protagonist Rey. He is frequently featured as a main character in various other Star Wars media.", payload.c_str());
	ASSERT_EQ(17, request.content_length);
}

int main(int argc, char* argv[]) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}