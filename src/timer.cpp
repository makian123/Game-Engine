#include "timer.h"

namespace engine {
	void Time::Begin() {
		beginTime = std::chrono::high_resolution_clock::now();
	}
	void Time::End() {
		std::chrono::high_resolution_clock::time_point endTime = std::chrono::high_resolution_clock::now();

		frameTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - beginTime).count();
		timePassed += frameTime / 1000.0f;
		framesPassed++;
	}
}