#pragma once
#include <factories/socket_factory.h>
#include <WinSock2.h>
#include <iphlpapi.h>
#include <Windows.h>
#include <Ws2tcpip.h>

namespace webserver::sock {
	using socket_t = SOCKET;

	class WindowsSocketTcp : public ISocketTcp {
	private:
		socket_t socket_;
		uint32_t port_;
		std::string host_;
	public:
		WindowsSocketTcp();
		WindowsSocketTcp(const std::string& host, uint32_t port);
		WindowsSocketTcp(socket_t sock, const std::string& host, uint32_t port);

		virtual void Connect(const std::string& host, uint32_t port) override;
		virtual void ConnectTimeout(const std::string& host, uint32_t port, uint32_t timeout_ms) override;
		virtual void Close() override;
		virtual Result<int32_t> Send(const std::vector<char>& data) const override;
		virtual Result<int32_t> Send(const std::string& data) const override;
		virtual Result<std::vector<char>> Receive() const override;

		virtual void Bind(uint32_t port) override;
		virtual void Listen() override;
		virtual std::unique_ptr<ISocketTcp> Accept() override;
		virtual int16_t GetFd() override;
		virtual std::string GetPeerAddress() const override;
	};
}