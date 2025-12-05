#pragma once
#include <vector>
#include <iostream>
#include <random>
#include <stdio.h>

class TimeSeriesGenerator {
public:
	TimeSeriesGenerator();
	explicit TimeSeriesGenerator(int seed);
	virtual ~TimeSeriesGenerator() = default;

	virtual std::vector<double> generateTimeSeries(int n) = 0;

	static void printTimeSeries(const std::vector<double>& series);

private:
	int seed;
};



