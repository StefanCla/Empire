#pragma once
#include "Headers/Precomp.h"

class Random
{
public:
	Random(const Random& r) = delete;
	Random& operator=(const Random& r) = delete;

	static Random* Instance();

	void SetSeed(uint32_t seed);
	float FloatZeroOne();
	float FloatRange(float lower, float upper);
	int IntRange(int lower, int upper);
	int IntRangePeakMiddle(int lower, int upper);

private:
	Random();
	~Random();

	uint32_t x32 = 1;

	uint32_t XorShift32();
};