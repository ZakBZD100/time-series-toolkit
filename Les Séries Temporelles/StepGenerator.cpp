#include "StepGenerator.h"

StepGenerator::StepGenerator(int seed)
    : TimeSeriesGenerator(seed)
{
    srand(seed);
}

StepGenerator::StepGenerator()
    : TimeSeriesGenerator(0)
{
    srand(0);
}

std::vector<double> StepGenerator::generateTimeSeries(int n) {
    std::vector<double> series;
    series.reserve(n);
    int current = 0;

    for (int i = 0; i < n; ++i) {
        if (rand() % 2 == 0) {
            current = rand() % 101; 
        }
        series.push_back(static_cast<double>(current));
    }

    return series;
}
