#include <util/util.h>
#include <logging/logging.h>
#include <algorithm>
#include <fstream>
#include <iterator>

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

    std::vector<unsigned char> ReadFile(const std::string& path) {
        return ReadFile(filesystem::path(path));
    }

	std::vector<unsigned char> ReadFile(const filesystem::path& path) {
        if(!filesystem::exists(path)) {
            const std::string msg = "File " + path.string() + "does not exist.";
            lLog(lError) << "File " << path << " does not exist.";
            throw ServerException{msg, -1};
        }

        std::ifstream ifs(path, std::ios::binary);
        if (!ifs.is_open()) {
            const int syserr = errno;
            lLog(lError) << "Could not open file (" << syserr << ").";
            throw ServerException{"Could not open file", syserr};
        }
        std::vector<unsigned char> vec;
        std::copy(std::istream_iterator<unsigned char>(ifs), std::istream_iterator<unsigned char>(),
        std::back_inserter(vec));
        ifs.close(),

        if (ifs.fail() && !ifs.eof()) {
            const int syserr = errno;        
            throw wcon::Error(STREAM("logical i/o error on " << path), WCIERR_READ_FILE_IO, syserr);
        }

        if (ifs.bad()) {
            const int syserr = errno;        
            throw ServerException{"read or write i/o error on " << path), WCIERR_READ_FILE_IO, syserr);
        }

        if (buffer.empty())
            throw wcon::Error("Empty buffer", WCIERR_READ_FILE_EMPTY);   

        return buffer;

        return vec;
    }

    std::vector<unsigned char> ReadFile(const std::string& path, const size_t size) {
        return ReadFile(filesystem::path(path), size);
    }

	std::vector<unsigned char> ReadFile(const filesystem::path& path, const size_t size) {
        if(size == 0) {
            return {};
        }

        if(!filesystem::exists(path)) {
            const std::string msg = "File " + path.string() + "does not exist.";
            lLog(lError) << "File " << path << " does not exist.";
            throw ServerException{msg, -1};
        }

        std::ifstream ifs(path, std::ios::binary);
        if (!ifs.is_open()) {
            const int syserr = errno;
            lLog(lError) << "Could not open file (" << syserr << ").";
            throw ServerException{"Could not open file", syserr};
        }
        std::vector<unsigned char> vec;
        vec.reserve(size);
        std::copy(std::istream_iterator<unsigned char>(ifs), std::istream_iterator<unsigned char>(),
        std::back_inserter(vec));

        return vec;
    }


}