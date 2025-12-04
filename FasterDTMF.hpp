#pragma once
#include <algorithm>

#include "FasterGoertzel.hpp"

class FasterDTMF
{
	static constexpr char   KEY[4][4]        = { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' }, { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };
	static constexpr double FREQUENCY[8]     = { 697, 770, 852, 941, 1209, 1336, 1477, 1633 };
	static constexpr size_t FREQUENCY_COUNT  = sizeof(FREQUENCY) / sizeof(double);

	FasterDTMF() = delete;

public:
	template<typename T>
	static constexpr char Calculate(const T* buffer, size_t size, uint32_t sample_rate, uint32_t channel, uint32_t channel_count, double* magnitude)
	{
		double frequency[FREQUENCY_COUNT] = {};
		double frequency_max[2]           = { __DBL_MIN__, __DBL_MIN__ };
		size_t frequency_max_i[2]         = { 0, 0 };

		FasterGoertzel::Calculate(buffer, size, sample_rate, channel, channel_count, FREQUENCY, frequency);

		for (size_t i = 0, j = 4; i < 4; ++i, ++j)
		{
			if (frequency[i] > frequency_max[0])
			{
				frequency_max[0]   = frequency[i];
				frequency_max_i[0] = i;
			}

			if (frequency[j] > frequency_max[1])
			{
				frequency_max[1]   = frequency[j];
				frequency_max_i[1] = j;
			}
		}

		if (magnitude)
			*magnitude = std::min(frequency_max[0], frequency_max[1]);

		return KEY[frequency_max_i[0]][frequency_max_i[1] - 4];
	}
};
