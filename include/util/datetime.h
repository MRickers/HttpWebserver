#include <chrono>
#include <string>

namespace webserver::util {
    class Datetime {
    private:
        std::chrono::system_clock::time_point point_in_time_;

        std::string format(const std::tm& tm, const std::string& fmt) const;
        std::string datetime_local(const std::chrono::time_point<std::chrono::system_clock>& tm, const std::string& fmt) const;
        std::string datetime_gmt(const std::chrono::time_point<std::chrono::system_clock>& tm, const std::string& fmt) const;
    public:
        Datetime();

        std::string DateLocal(const std::string& fmt) const;
        std::string DateGMT(const std::string& fmt) const;
        std::string DateHTTPFormat() const;
        std::string Time() const;
        std::string DateTime() const;
    };
}