#include <handlers/accept_handler.h>
#include <factories/socket_factory.h>
#include <logging/logging.h>

namespace webserver::multiplexer {
	AcceptRequestHandler::AcceptRequestHandler(FdQueue queue) : queue_(queue) {

	}

	void AcceptRequestHandler::HandleRequest(int fd) {
		const auto socket = webserver::sock::SocketFactory().Create((int16_t)fd);
		const auto new_socket = socket->Accept();
		
		lLog(lDebug) << "Sending hello message";
		const auto [sent_result, sent] = new_socket->Send("Hello there");
		if (sent_result != webserver::sock::SocketResult::OK) {
			lLog(lDebug) << "Sending failed";
		}

		lLog(lDebug) << "Adding socket to queue";
		queue_->Push((int16_t)fd);
		queue_->Push(new_socket->GetFd());
	}
}