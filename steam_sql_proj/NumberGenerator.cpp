#include "NumberGenerator.h"

namespace ORDO {

	NumberGenerator::NumberGenerator() : rng(std::make_unique<std::mt19937>(std::random_device{}())) {}

	int NumberGenerator::getRandInt(int min, int max)
	{
		return std::uniform_int_distribution<int>(min, max)(*rng);
	}

	float NumberGenerator::getRandFloat(float min, float max)
	{
		return std::uniform_real_distribution<float>(min, max)(*rng);
	}
}