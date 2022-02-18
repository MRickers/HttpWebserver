#pragma once
#include <chrono>

namespace webserver::util {
	class Duration {
	private:
		bool started_;
		std::chrono::high_resolution_clock::time_point start_time_;
        std::chrono::high_resolution_clock::time_point stop_time_;
	public:
		Duration();

		void Start();
		void Stop();
		int64_t GetTime() const;
		bool IsStarted() const;
	};
}