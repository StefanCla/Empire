#include "Headers/Random.h"

Random* Random::Instance()
{
	static Random* s_Instance = new Random();
	return s_Instance;
}

Random::Random()
{}

Random::~Random()
{}

void Random::SetSeed(uint32_t seed)
{
	x32 = seed;
}

float Random::FloatZeroOne()
{
	float result = static_cast<float>(XorShift32()) / static_cast<float>(UINT32_MAX);
	assert(result >= 0.0f && result <= 1.0f && "Result is not within specified range!\n");
	return result;
}

float Random::FloatRange(float lower, float upper)
{
	float rand = static_cast<float>(XorShift32()) / static_cast<float>(UINT32_MAX);
	float diff = upper - lower;
	float result = lower + rand * diff;
	assert(result >= lower && result <= upper && "Result is not within specified range!\n");
	return result;
}

int Random::IntRange(int lower, int upper)
{
	int result = static_cast<int>((XorShift32() % (upper - lower)) + lower);
	assert(result >= lower && result <= upper && "Result is out of specified range!\n");
	return result;
}

int Random::IntRangePeakMiddle(int lower, int upper)
{
	int result = static_cast<int>((FloatRange(lower, upper + 1) + FloatRange(lower, upper + 1)) * 0.5f);
	assert(result >= lower && result <= upper && "Result is out of specified range!\n");
	return result;
}

uint32_t Random::XorShift32()
{
	x32 ^= x32 << 13;
	x32 ^= x32 >> 17;
	x32 ^= x32 << 5;

	return x32;
}
