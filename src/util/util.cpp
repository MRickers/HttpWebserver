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
        if(const auto last = Trim(str.substr(start_index)); !last.empty()) {
            vec.push_back(last);
        }

		return vec;
	}

    std::pair<std::string,std::string> SplitOnce(const std::string& str, const std::string delimeter) {
        if(str.empty()) {
            return {};
        }
        const size_t found = str.find(delimeter);
        if(found == std::string::npos) {
            return {};
        }
        return std::make_pair(str.substr(0, found), str.substr(found+1));
    }

    std::string Trim(const std::string& str) {
        const auto front = std::find_if_not(str.begin(), str.end(), [](unsigned char c) {
            return (std::isspace(c) || std::iscntrl(c));
        });

        const auto back = std::find_if_not(str.rbegin(), str.rend(), [](unsigned char c) {
            return (std::isspace(c) || std::iscntrl(c));
        }).base();

        return (back<=front ? std::string() : std::string{front, back});
    }
}