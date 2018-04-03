#pragma once

#include <Windows.h>

namespace o2
{
	// ------------------------------------
	// Timer class, using for counting time
	// ------------------------------------
	class Timer
	{
	public:
		// Default constructor
		Timer();

		// Destructor
		~Timer();

		// Resets time
		void Reset();

		// Returns time in seconds from last Reset() call
		float GetTime();

		// Returns time in seconds from last Reset() or GetElapsedTime() call
		float GetDeltaTime();

	protected:
		LONGLONG      mLastElapsedTime;
		LARGE_INTEGER mFrequency;
		LARGE_INTEGER mStartTime;
	};

	class ScopeTimer
	{
	public:
		const char* id = "";
		Timer timer;
		
		ScopeTimer() {}
		ScopeTimer(const char* id):id(id) {}
		~ScopeTimer() { Check(); }

		void Check() { printf("%s - %.4f sec\n", id, timer.GetDeltaTime()); }
	};
}
