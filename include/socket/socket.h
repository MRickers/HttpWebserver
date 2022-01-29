#pragma once
#include <string>
#include <vector>
#include <memory>
#include <utility>

namespace webserver::sock {
	enum class SocketResult : int16_t {
		OK = 0,
		NOTINITIALISED,
		NETDOWN,
		NOTCONNECTED,
		NOSOCKET,
		SHUTDOWN,
		CONNABORTED,
		TIMEDOUT,
		CONNRESET,

		DEFAULTERROR
	};

	template<typename T>
	struct Result {
		Result(SocketResult result_, const T& data_) : result(result_), data(data_) {}
		SocketResult result;
		T data;
	};

	class SocketException : public std::exception {
	private:
		const char* m_msg;
		int16_t m_error_code;
		const char* m_info;
	public:
		SocketException(const std::string& msg, int16_t error_code, const std::string& info="") :
			m_msg(msg.c_str()),
			m_error_code(error_code),
			m_info(info.c_str()) {}

		const char* GetMsg() { return m_msg; }
		const char* GetInfo() { return m_info; }
		int16_t GetErrorCode() { return m_error_code; }

	};

	class ISocketTcp {
	public:
		virtual ~ISocketTcp() {}

		static void Init();
		static void CleanUp();
		virtual void Connect(const std::string& host, uint32_t port) = 0;
		virtual void ConnectTimeout(const std::string& host, uint32_t port, uint32_t timeout_ms) = 0;
		virtual void Close() = 0;
		virtual Result<int32_t> Send(const std::vector<char>& data) const = 0;
		virtual Result<int32_t> Send(const std::string& data) const = 0;
		virtual Result<std::vector<char>> Receive() const = 0;

		virtual void Bind(uint32_t port) = 0;
		virtual void Listen() = 0;
		virtual std::unique_ptr<ISocketTcp> Accept() = 0;
		virtual int16_t GetFd() = 0;
		virtual std::string GetPeerAddress() const = 0;
	};
}