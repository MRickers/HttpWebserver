#pragma once
#include <socket/socket.h>

namespace webserver::util {
    class ServerException : public std::exception {
	private:
		const char* m_msg;
		int16_t m_error_code;
		const char* m_info;
	public:
		ServerException(const std::string& msg, int16_t error_code, const std::string& info="") :
			m_msg(msg.c_str()),
			m_error_code(error_code),
			m_info(info.c_str()) {}

		const char* GetMsg() { return m_msg; }
		const char* GetInfo() { return m_info; }
		int16_t GetErrorCode() { return m_error_code; }

	};

    /// <summary>
    /// Convert WSA Windows error code to SocketResult
    /// </summary>
    /// <param name="wsa_error"></param>
    /// <returns></returns>
    webserver::sock::SocketResult convertToResult(int wsa_error);


}