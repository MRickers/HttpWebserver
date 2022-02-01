#include <handlers/read_handler.h>
#include <logging/logging.h>
#include <factories/socket_factory.h>

namespace webserver::multiplexer {
	ReadRequestHandler::ReadRequestHandler(FdQueue queue) : queue_(queue) {

	}

	void ReadRequestHandler::HandleRequest(int fd) {
		const auto socket = webserver::sock::SocketFactory().Create((int16_t)fd);

		const auto [recv_result, data] = socket->Receive();

		if (recv_result != webserver::sock::SocketResult::OK) {
			lLog(lDebug) << "Error in receive " << static_cast<int>(recv_result);
			socket->Close();
			return;
		}

		lLog(lInfo) << "Read data: " << std::string{ data.begin(), data.end() };
		lLog(lInfo) << "Echoing . . .";

		const auto [sent_result, sent] = socket->Send(data);

		if (sent_result != webserver::sock::SocketResult::OK) {
			lLog(lError) << "Sending failed " << static_cast<int>(sent_result);
			socket->Close();
			return;
		}
		queue_->Push((int16_t)fd);

	}
}