#include <handlers/accept_handler.h>
#include <factories/socket_factory.h>
#include <logging/logging.h>

namespace webserver::multiplexer {
	AcceptRequestHandler::AcceptRequestHandler(FdQueue queue) : queue_(queue) {

	}

	void AcceptRequestHandler::HandleRequest(int fd) {
		const auto socket = webserver::sock::SocketFactory().Create(fd);
		const auto new_socket = socket->Accept();

		const auto [recv_result, data] = new_socket->Receive();
		
		if (recv_result == webserver::sock::SocketResult::OK) {
			lLog(lDebug) << "Received " << std::string{ data.begin(), data.end() };
			lLog(lDebug) << "Sending hello message";
			const auto [sent_result, sent] = new_socket->Send("Hello there");
			if (sent_result != webserver::sock::SocketResult::OK) {
				lLog(lDebug) << "Sending failed";
			}
		}
		else {
			lLog(lDebug) << "Receive failed " << static_cast<int16_t>(recv_result);
		}

		lLog(lDebug) << "Adding socket to queue";
		queue_->Push(fd);
		queue_->Push(new_socket->GetFd());
	}
}