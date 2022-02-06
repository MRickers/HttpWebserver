#pragma once
#include <socket/socket.h>
#include <factories/socket_factory.h>

namespace webserver::sock {
	using socket_t = int;

	class LinuxSocketTcp : public ISocketTcp {
	private:
		socket_t socket_;
		uint32_t port_;
		std::string host_;
	public:
		LinuxSocketTcp();
		LinuxSocketTcp(const std::string& host, uint32_t port);
		LinuxSocketTcp(socket_t sock, const std::string& host, uint32_t port);

		virtual void Connect(const std::string& host, uint32_t port) override;
		virtual void ConnectTimeout(const std::string& host, uint32_t port, uint32_t timeout_ms) override;
		virtual void Close() override;
		virtual Result<int32_t> Send(const std::vector<char>& data) const override;
		virtual Result<int32_t> Send(const std::string& data) const override;
		virtual Result<std::vector<char>> Receive() const override;

		virtual void Bind(uint32_t port) override;
		virtual void Listen() override;
		virtual std::unique_ptr<ISocketTcp> Accept() override;
		virtual int16_t GetFd() const override;
		virtual void SetFd(int16_t fd) override;
		virtual std::string GetPeerAddress() const override;
	};
}