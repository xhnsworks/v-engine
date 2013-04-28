#ifndef TIMER_H
#define TIMER_H
#include "common.h"
#include "etypes.h"
inline double ms_to_s(double ms)
{
	return ms / 1000.0;
}
inline double s_to_ms(double s)
{
	return s * 1000.0;
}
inline double us_to_s(double us)
{
	return us / 1000000.0;
}
inline double s_to_us(double s)
{
	return s * 1000000.0;
}
/**
#ifdef  __GNUC__
__inline__ uint64 perf_counter(void)
{
	uint32 lo, hi;
	// take time stamp counter, rdtscp does serialize by itself, and is much cheaper than using CPUID
	__asm__ __volatile__ (
		"rdtscp" : "=a"(lo), "=d"(hi)
		);
	return ((uint64)lo) | (((uint64)hi) << 32);
}
**/
#if defined(_MSC_VER) || defined(__MINGW32__)

inline uint64 perf_counter(void)
{
	uint64 ret;
	QueryPerformanceCounter((LARGE_INTEGER*)&ret);
	return ret;
}
inline double pref_frequency(void)
{
	LARGE_INTEGER temp;
	QueryPerformanceFrequency((LARGE_INTEGER*)&temp);
	return ((double)temp.QuadPart) / 1000000.0;
}
inline double cale_elapsed_time(uint64 start, uint64 end, double freq)
{
	return ((double)end - (double)start) / freq;
}
struct TimeCheckpoint
{
	uint64 timeStamp;
	double freq;
	TimeCheckpoint()
		: timeStamp(0)
		, freq(0.0)
	{}
	static inline TimeCheckpoint Tick() {
        TimeCheckpoint ret;
		ret.timeStamp = perf_counter();
		ret.freq = pref_frequency();
		return ret;
	}
	static inline double CaleElapsedTime(TimeCheckpoint& prevCheckpoint, TimeCheckpoint& curtCheckpoint) {
		double avFreq = (curtCheckpoint.freq + prevCheckpoint.freq) / 2.0;
        return cale_elapsed_time(prevCheckpoint.timeStamp, curtCheckpoint.timeStamp, avFreq);
	}
};
#endif
#endif
