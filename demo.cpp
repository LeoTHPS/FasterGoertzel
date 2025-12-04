#include <array>
#include <algorithm>

#include "FasterGoertzel.hpp"

constexpr char   DTMF_KEY[4][4]        = { { '1', '2', '3', 'A' }, { '4', '5', '6', 'B' }, { '7', '8', '9', 'C' }, { '*', '0', '#', 'D' } };
constexpr double DTMF_FREQUENCY[8]     = { 697, 770, 852, 941, 1209, 1336, 1477, 1633 };
constexpr size_t DTMF_FREQUENCY_COUNT  = sizeof(DTMF_FREQUENCY) / sizeof(double);

constexpr double CTCSS_FREQUENCY[]     = { 67.0, 71.9, 74.4, 77.0, 79.7, 82.5, 85.4, 88.5, 91.5, 94.8, 97.4, 100.0, 103.5, 107.2, 110.9, 114.8, 118.8, 123.0, 127.3, 131.8, 136.5, 141.3, 146.2, 151.4, 156.7, 162.2, 167.9, 173.8, 179.9, 186.2, 192.8, 203.5, 210.7, 218.1, 225.7, 233.6, 241.8, 250.3 };
constexpr size_t CTCSS_FREQUENCY_COUNT = sizeof(CTCSS_FREQUENCY) / sizeof(double);

template<typename T>
constexpr char   FasterDTMF(const T* buffer, size_t size, uint32_t sample_rate, uint32_t channel, uint32_t channel_count, double* magnitude)
{
	double frequency[DTMF_FREQUENCY_COUNT] = {};
	double frequency_max[2]                = { __DBL_MIN__, __DBL_MIN__ };
	size_t frequency_max_i[2]              = { 0, 0 };

	FasterGoertzel::Calculate(buffer, size, sample_rate, channel, channel_count, DTMF_FREQUENCY, frequency);

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

	return DTMF_KEY[frequency_max_i[0]][frequency_max_i[1] - 4];
}
template<typename T>
constexpr double FasterCTCSS(const T* buffer, size_t size, uint32_t sample_rate, uint32_t channel, uint32_t channel_count, double* magnitude)
{
	double frequency[CTCSS_FREQUENCY_COUNT] = {};
	double frequency_max                    = __DBL_MIN__;
	size_t frequency_max_i                  = 0;

	FasterGoertzel::Calculate(buffer, size, sample_rate, channel, channel_count, CTCSS_FREQUENCY, frequency);

	for (size_t i = 0; i < CTCSS_FREQUENCY_COUNT; ++i)
		if (frequency[i] > frequency_max)
		{
			frequency_max   = frequency[i];
			frequency_max_i = i;
		}

	if (magnitude)
		*magnitude = frequency_max;

	return CTCSS_FREQUENCY[frequency_max_i];
}

std::array<int16_t, 0xFFF> iq;

int main(int argc, char* argv[])
{
	char   dtmf;  double dtmf_magnitude;
	double ctcss; double ctcss_magnitude;

	dtmf  = FasterDTMF(iq.data(), iq.size(), 11025, 0, 1, &dtmf_magnitude);
	ctcss = FasterCTCSS(iq.data(), iq.size(), 11025, 0, 1, &ctcss_magnitude);

	return 0;
}
