#pragma once
#include <vector>
#include <string>
#if __GNUG__ == 7
#include <experimental/filesystem>
namespace filesystem = std::experimental::filesystem;
#else
#include <filesystem>
namespace filesystem = std::filesystem;
#endif


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

		virtual const char* what() const noexcept override { return m_msg; }
		const char* GetInfo() { return m_info; }
		int16_t GetErrorCode() { return m_error_code; }

	};

	std::vector<std::string> Split(const std::string& str, const std::string delimeter);
	std::pair<std::string,std::string> SplitOnce(const std::string& str, const std::string delimeter);

	std::string Trim(const std::string& str);

	std::vector<unsigned char> ReadFile(const std::string& path);
	std::vector<unsigned char> ReadFile(const filesystem::path& path);
	std::vector<unsigned char> ReadFile(const std::string& path, const size_t size);
	std::vector<unsigned char> ReadFile(const filesystem::path& path, const size_t size);

}