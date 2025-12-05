#pragma once
#include <limits>
#include <cstdint>
#include <utility>

class FasterGoertzel
{
	class Math
	{
		Math() = delete;

	public:
		static constexpr double PI  = 3.14159265358979323846;
		static constexpr double PI2 = PI * 2;

		static constexpr double Cos(double x, int terms = 12)
		{
			x = FMod(x, PI2);

			if (x > PI)  x -= PI2;
			if (x < -PI) x += PI2;

			double result = 0;

			for (int i = 0; i < terms; ++i)
				result += Pow(-1.0, i) * Pow(x, 2 * i) / Factorial(2 * i);

			return result;
		}

		static constexpr double Pow(double x, int n)
		{
			return (n == 0) ? 1.0 : x * Pow(x, n - 1);
		}

		static constexpr double FMod(double x, double y)
		{
			return x - static_cast<int64_t>(x / y) * y;
		}

		static constexpr double Sqrt(double x, double epsilon = 1e-12L)
		{
			if (x < 0)
				return std::numeric_limits<double>::quiet_NaN();

			if (!x || (x == 1))
				return x;

			double prev  = 0.0L;
			double guess = x / 2.0L;

			while (guess != prev)
			{
				prev  = guess;
				guess = (guess + x / guess) / 2;

				if ((guess == prev) || (((guess - prev) < epsilon) && ((prev - guess) < epsilon)))
					break;
			}

			return guess;
		}

		static constexpr double Factorial(int n)
		{
			return (n <= 1) ? 1.0 : (n * Factorial(n - 1));
		}
	};

	FasterGoertzel() = delete;

public:
	template<typename T>
	static constexpr void Calculate(const T* buffer, size_t size, uint32_t sample_rate, uint32_t channel, uint32_t channel_count, double frequency, double& value)
	{
		double values[1]            = {};
		const double frequencies[1] = { frequency };
		Calculate(buffer, size, sample_rate, channel, channel_count, frequencies, values);

		value = values[0];
	}
	template<typename T, size_t S>
	static constexpr void Calculate(const T* buffer, size_t size, uint32_t sample_rate, uint32_t channel, uint32_t channel_count, const double(&frequency)[S], double(&value)[S])
	{
		Calculate(buffer, size, sample_rate, channel, channel_count, frequency, value, std::make_index_sequence<S> {});
	}

private:
	template<typename T, size_t S, size_t ... I>
	static constexpr void Calculate(const T* buffer, size_t size, uint32_t sample_rate, uint32_t channel, uint32_t channel_count, const double(&frequency)[S], double(&value)[S], std::index_sequence<I ...>)
	{
		static_assert(S == sizeof...(I));
		static_assert(std::is_integral<T>::value);

		double q[3][S]  = {};
		double coeff[S] = { (Math::Cos((frequency[I] * Math::PI * 2) / sample_rate) * 2) ... };

		for (size_t i = channel; i < size; i += channel_count)
		{
			((q[0][I] = coeff[I] * q[1][I] - q[2][I] + buffer[i]), ...);
			((q[2][I] = q[1][I]), ...);
			((q[1][I] = q[0][I]), ...);
		}

		((value[I] = Math::Sqrt(q[1][I] * q[1][I] + q[2][I] * q[2][I] - coeff[I] * q[1][I] * q[2][I])), ...);
	}
};
