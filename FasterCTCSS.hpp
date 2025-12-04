#pragma once
#include "FasterGoertzel.hpp"

class FasterCTCSS
{
	static constexpr double FREQUENCY[]     = { 67.0, 71.9, 74.4, 77.0, 79.7, 82.5, 85.4, 88.5, 91.5, 94.8, 97.4, 100.0, 103.5, 107.2, 110.9, 114.8, 118.8, 123.0, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 162.2, 167.9, 173.8, 179.9, 186.2, 192.8, 203.5, 210.7, 218.1, 225.7, 233.6, 241.8, 250.3 };
	static constexpr size_t FREQUENCY_COUNT = sizeof(FREQUENCY) / sizeof(double);

	FasterCTCSS() = delete;

public:
	template<typename T>
	static constexpr double Calculate(const T* buffer, size_t size, uint32_t sample_rate, uint32_t channel, uint32_t channel_count, double* magnitude)
	{
		double frequency[FREQUENCY_COUNT] = {};
		double frequency_max              = __DBL_MIN__;
		size_t frequency_max_i            = 0;

		FasterGoertzel::Calculate(buffer, size, sample_rate, channel, channel_count, FREQUENCY, frequency);

		for (size_t i = 0; i < FREQUENCY_COUNT; ++i)
			if (frequency[i] > frequency_max)
			{
				frequency_max   = frequency[i];
				frequency_max_i = i;
			}

		if (magnitude)
			*magnitude = frequency_max;

		return FREQUENCY[frequency_max_i];
	}
};
