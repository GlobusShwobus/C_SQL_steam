#pragma once

#include <random>
#include <memory>

namespace ORDO {
	class NumberGenerator {
		std::unique_ptr<std::mt19937> rng;

	public:
		NumberGenerator();
		int getRandInt(int min, int max);
		float getRandFloat(float min, float max);
	};
}

