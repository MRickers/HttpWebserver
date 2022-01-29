#include <logging/logging.h>
#include <factories/socket_factory.h>

int main(int argc, char* argv[]) {
	using namespace webserver::sock;

	lLog(lDebug) << "socket example.";

	ISocketTcp::Init();

	auto socket = SocketFactory().Create();
	
	socket->Bind(5000);
	socket->Listen();

	try {
		while (1) {
			const auto new_socket = socket->Accept();
			const auto fd = new_socket->GetFd();
			lLog(lInfo) << "fd=" << fd;
			try {
				while (1) {
					const auto sock = SocketFactory().Create(fd);
					const auto [result, data] = sock->Receive();
					if (result != SocketResult::OK) {
						lLog(lError) << "Receive error " << (int16_t)result << " from " << sock->GetPeerAddress();
						break;
					}
					lLog(lInfo) << "Received data:" << std::endl << std::string{ data.begin(), data.end() };
					lLog(lInfo) << "Echo back";

					{
						const auto [result, sent] = sock->Send(data);
						if (result != SocketResult::OK) {
							lLog(lError) << "Sending failed " << (int16_t)result << " from " << sock->GetPeerAddress();
						}
						lLog(lInfo) << "Sent " << sent << " bytes";
					}
				}
			}
			catch (const SocketException& e) {
				new_socket->Close();
				lLog(lError) << e.what();
			}
		}
	}
	catch (const SocketException& e) {
		lLog(lError) << e.what();
	}


	ISocketTcp::CleanUp();
	return 0;
}