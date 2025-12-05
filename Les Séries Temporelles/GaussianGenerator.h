#pragma once
#define _USE_MATH_DEFINES
#include <vector>
#include <cmath>
#include "TimeSeriesGenerator.h"
#include <cstdlib>

class GaussianGenerator : public TimeSeriesGenerator
{
public:
	GaussianGenerator();
	GaussianGenerator(double moyenne, double ecartType, int seed);
	std::vector<double> generateTimeSeries(int n) override;

private:
	double moyenne;
	double ecartType;
	double boxMuller();
};