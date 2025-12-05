#include "TimeSeriesGenerator.h"

TimeSeriesGenerator::TimeSeriesGenerator() : seed(0) {}
TimeSeriesGenerator::TimeSeriesGenerator(int seed) : seed(seed) {}

void TimeSeriesGenerator::printTimeSeries(const std::vector<double>& series) {
    for (size_t i = 0; i < series.size(); ++i) {
        std::cout << i << ": " << series[i] << std::endl;
    }
}

