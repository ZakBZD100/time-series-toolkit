#pragma once
#include "TimeSeriesGenerator.h"
#include <cmath>
#include <vector>

class SinWaveGenerator : public TimeSeriesGenerator
{
public:
    SinWaveGenerator();
    SinWaveGenerator(double amplitude, double omega, double phase, int seed);
    std::vector<double> generateTimeSeries(int n) override;

private:
    double amplitude;
    double omega;
    double phase;
};

