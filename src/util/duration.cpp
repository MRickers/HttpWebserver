#include <util/duration.h>

namespace webserver::util {
	Duration::Duration() : 
		started_(true),
		start_time_(std::chrono::high_resolution_clock::now()),
		stop_time_() {}

	void Duration::Start() {
		started_ = true;
		start_time_ = std::chrono::high_resolution_clock::now();
	}

	void Duration::Stop() {
		started_ = false;
		stop_time_ = std::chrono::high_resolution_clock::now();
	}

	int64_t Duration::GetTime() const {
		const auto duration = stop_time_ - start_time_;
		return std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
	}

	bool Duration::IsStarted() const {
		return started_;
	}
}