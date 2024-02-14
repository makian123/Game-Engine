#pragma once

#include <cstdint>
#include <chrono>

namespace engine {
	class Time {
	private:
		uint64_t framesPassed = 0;
		float timePassed = 0;
		uint64_t frameTime = 0;	// in ms

		std::chrono::high_resolution_clock::time_point beginTime;
	public:
		Time() = default;

		void Begin();
		void End();

		// Returns frame time in seconds
		float DeltaTime() const { return frameTime / 1000.0f; }

		// Returns frames passed since engine started
		uint64_t FrameCount() const { return framesPassed; }

		// Returns number of seconds since initialized
		float TimePassed() const { return timePassed; }
	};
}