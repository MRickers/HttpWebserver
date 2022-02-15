#include <util/util.h>
#include <logging/logging.h>
#include <algorithm>
namespace webserver::util {
    std::vector<std::string> Split(const std::string& str, const std::string delimeter) {
        if(str.empty()) {
            return {};
        }

		std::vector<std::string> vec;

        size_t found = str.find(delimeter);
        size_t start_index = 0;
		while(found!=std::string::npos) {
            const auto delta = found - start_index;
            vec.push_back(str.substr(start_index, delta));
            start_index = found+delimeter.size();
			found = str.find(delimeter, found+delimeter.size());
		}
        vec.push_back(str.substr(start_index));
		return vec;
	}

    std::string Trim(const std::string& str) {
        const auto front = std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
            return (std::isspace(c) || std::iscntrl(c));
        });

        const auto back = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
            return (std::isspace(c) || std::iscntrl(c));
        });

        return std::string{front, back.base()};
    }
}