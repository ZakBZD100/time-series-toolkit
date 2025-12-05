#pragma once
#include <vector>
#include "TimeSeriesGenerator.h"

class StepGenerator : public TimeSeriesGenerator
{
	public:
	StepGenerator();
	explicit StepGenerator(int seed);
	std::vector<double> generateTimeSeries(int n) override;


};

