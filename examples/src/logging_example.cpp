#include <logging/logging.h>
#include <thread>
#include <chrono>

void slow() {
	lLog(lInfo) << "Slow function start";
	std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	lLog(lError) << "Slow function end";
}

void fast() {
	for(int i=0;i<5;i++) {
		lLog(lWarn) << "Fast function";
		std::this_thread::sleep_for(std::chrono::milliseconds(200));
	}
}

int main() {
	lLog(lDebug) << "Hello there";
	std::thread t1{ slow };
	std::thread t2{ fast };

	t1.join();
	t2.join();
	return 0;
}