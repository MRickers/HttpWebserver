#include <handlers/read_handler.h>
#include <logging/logging.h>
#include <factories/socket_factory.h>
#include <util/http_parser.h>
#include <util/util.h>

namespace webserver::multiplexer {
	ReadRequestHandler::ReadRequestHandler(FdQueue queue) : queue_(queue) {

	}

	void ReadRequestHandler::HandleRequest(int fd) {
		try {
			const auto socket = webserver::sock::SocketFactory().Create((int16_t)fd);

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

			const auto [sent_result, sent] = socket->Send(data);

			if (sent_result != webserver::sock::SocketResult::OK) {
				lLog(lError) << "Sending failed " << static_cast<int>(sent_result);
				socket->Close();
				return;
			}
			queue_->Push((int16_t)fd);

		} catch(const webserver::util::ServerException& e) {
			lLog(lError) << e.what();
		}
	}
}