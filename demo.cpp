#include <array>

#include "FasterDTMF.hpp"
#include "FasterCTCSS.hpp"
#include "FasterGoertzel.hpp"

std::array<int16_t, 0xFFF> iq;

int main(int argc, char* argv[])
{
	char   dtmf;  double dtmf_magnitude;
	double ctcss; double ctcss_magnitude;

	dtmf  = FasterDTMF::Calculate(iq.data(), iq.size(), 11025, 0, 1, &dtmf_magnitude);
	ctcss = FasterCTCSS::Calculate(iq.data(), iq.size(), 11025, 0, 1, &ctcss_magnitude);

	return 0;
}
