#include <handlers/read_handler.h>
#include <logging/logging.h>
#include <factories/socket_factory.h>
#include <server/http/parser.h>
#include <server/http/response.h>
#include <util/util.h>
#include <util/duration.h>

namespace webserver::multiplexer {
	ReadRequestHandler::ReadRequestHandler(FdQueue queue) : queue_(queue) {

	}

	void ReadRequestHandler::HandleRequest(int fd) {
		try {
			const auto socket = webserver::sock::SocketFactory().Create((int16_t)fd);
			auto timer = webserver::util::Duration{};

			const auto [recv_result, data] = socket->Receive();

			if (recv_result != webserver::sock::SocketResult::OK) {
				lLog(lDebug) << "Error in receive " << static_cast<int>(recv_result);
				socket->Close();
				return;
			}

			std::string data_string{ data.begin(), data.end() };

			lLog(lInfo) << "Read data: " << data_string;
			
			const auto parser = webserver::util::HttpParser();

			const auto request = parser.Parse(data_string);

			lLog(lDebug) << "Incoming http request from: " << request.host;
			lLog(lDebug) << "Version: " << request.http_version;
			lLog(lDebug) << "Url: " << request.url;
			for(const auto& param : request.parameter) {
				lLog(lDebug) << param.first << ":" << param.second;
			}
			if(!request.payload.empty()) {
				lLog(lDebug) << "Payload: " << std::endl << std::string{request.payload.begin(), request.payload.end()};
			}
			// find callback 

			lLog(lDebug) << "Creating response";
			webserver::http::Response response;
			response.ContentType(types::MIMEType::ApplicationJson);
			response.StatusCode(types::HttpStatusCode::OK);
			response.Payload("{\"hello\":\"there\"}");

			const auto res = response.Format();

			lLog(lDebug) << "Sending response " << std::endl << std::string{res.cbegin(), res.cend()};
			const auto [sent_result, sent] = socket->Send(res);

			if (sent_result != webserver::sock::SocketResult::OK) {
				lLog(lError) << "Sending failed " << static_cast<int>(sent_result);
				socket->Close();
				return;
			}
			queue_->Push((int16_t)fd);

			timer.Stop();
			const auto dt = timer.GetTime();

			lLog(lInfo) << "Request took " << dt << " ms";

		} catch(const webserver::util::ServerException& e) {
			lLog(lError) << e.what();
		}
	}
}