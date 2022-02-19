#include <util/datetime.h>
#include <ctime> 
#include <sstream>
#include <iomanip>

namespace webserver::util {
        Datetime::Datetime() :
        point_in_time_(std::chrono::system_clock::now()) {

        }

        std::string Datetime::DateHTTPFormat() const {
            return DateGMT("%a, %d %b %Y %H:%M:%S %Z");
        }

        std::string Datetime::DateLocal(const std::string& fmt) const {
            return datetime_local(point_in_time_, fmt);
        }

        std::string Datetime::DateGMT(const std::string& fmt) const {
            return datetime_gmt(point_in_time_, fmt);
        }

        std::string Datetime::datetime_gmt(const std::chrono::time_point<std::chrono::system_clock>& tp, const std::string& fmt) const {
            std::time_t t = std::chrono::system_clock::to_time_t(tp);
            std::tm tm;
            tm = *gmtime(&t);
            return format(tm, fmt); 
        }

        std::string Datetime::datetime_local(const std::chrono::time_point<std::chrono::system_clock>& tp, const std::string& fmt) const {
            std::time_t t = std::chrono::system_clock::to_time_t(tp);
            std::tm tm;
            tm = *std::localtime(&t);
            return format(tm, fmt);            
        }

        std::string Datetime::format(const std::tm& tm, const std::string& fmt) const {
            std::stringstream s;
            s << std::put_time(&tm, fmt.c_str());
            return s.str();
        }
}