#include "GaussianGenerator.h"

GaussianGenerator::GaussianGenerator()
    : TimeSeriesGenerator(0), moyenne(0.0), ecartType(1.0) { srand(0); }

GaussianGenerator::GaussianGenerator(double moyenne, double ecartType  , int seed)
    : TimeSeriesGenerator(seed), moyenne(moyenne), ecartType(ecartType) { srand(seed); }

double GaussianGenerator::boxMuller() {
    double u1 = (double)rand() / RAND_MAX;
    double u2 = (double)rand() / RAND_MAX;

    return sqrt(-2.0 * log(u1)) * cos(2.0 * M_PI * u2);
}

std::vector<double> GaussianGenerator::generateTimeSeries(int n) {
    std::vector<double> series;
    series.reserve(n);

    for (int i = 0; i < n; ++i) {
        double z = boxMuller();
        series.push_back(moyenne + z * ecartType);
    }

    return series;
}