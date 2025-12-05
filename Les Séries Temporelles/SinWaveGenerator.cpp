#include "SinWaveGenerator.h"
#include <cstdlib>

SinWaveGenerator::SinWaveGenerator()
    : TimeSeriesGenerator(0), amplitude(1.0), omega(1.0), phase(0.0)
{
    srand(0);
}

SinWaveGenerator::SinWaveGenerator(double amplitude, double omega, double phase, int seed)
    : TimeSeriesGenerator(seed), amplitude(amplitude), omega(omega), phase(phase)
{
    srand(seed);
}

std::vector<double> SinWaveGenerator::generateTimeSeries(int n) {
    std::vector<double> series;
    series.reserve(n);

    for (int i = 0; i < n; ++i) {
        double x = static_cast<double>(i);
        series.push_back(amplitude * std::sin(omega * x + phase));
    }

    return series;
}
