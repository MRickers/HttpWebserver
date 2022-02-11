#include <util/util.h>
#include <logging/logging.h>

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
}