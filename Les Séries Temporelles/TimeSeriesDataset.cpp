#include "TimeSeriesDataset.h"
#include <numeric>
#include <cmath>
#include <algorithm>
#include <limits>

TimeSeriesDataset::TimeSeriesDataset(bool znormalize, bool isTrain, int maxLength)
    : znormalize(znormalize), isTrain(isTrain), maxLength(maxLength), numberOfSamples(0) {}

void TimeSeriesDataset::addSample(const std::vector<double>& series, int label) {
    std::vector<double> s = series;
    if (znormalize) {
        s = zNormalizeSeries(series);
    }

    int len = static_cast<int>(s.size());
    if (maxLength == 0) {
        maxLength = len;
    }

    std::vector<double> padded(maxLength, 0.0);
    for (int i = 0; i < std::min(len, maxLength); ++i) {
        padded[i] = s[i];
    }

    data.insert(data.end(), padded.begin(), padded.end());
    labels.push_back(label);
    ++numberOfSamples;
}

void TimeSeriesDataset::addTimeSeries(const std::vector<double>& series, int label) {
    addSample(series, label);
}

void TimeSeriesDataset::addTimeSeries(const std::vector<double>& series) {
    std::vector<double> s = series;
    if (znormalize) s = zNormalizeSeries(series);

    int len = static_cast<int>(s.size());
    if (maxLength == 0) {
        maxLength = len;
    }

    std::vector<double> padded(maxLength, 0.0);
    for (int i = 0; i < std::min(len, maxLength); ++i) padded[i] = s[i];

    data.insert(data.end(), padded.begin(), padded.end());
    ++numberOfSamples;
}

std::vector<double> TimeSeriesDataset::getSeries(int index) const {
    if (maxLength <= 0 || index < 0) return {};
    int start = index * maxLength;
    if (start + maxLength > static_cast<int>(data.size())) return {};
    return std::vector<double>(data.begin() + start, data.begin() + start + maxLength);
}

int TimeSeriesDataset::getNumberOfSamples() const {
    return numberOfSamples;
}

const std::vector<int>& TimeSeriesDataset::getLabels() const {
    return labels;
}

int TimeSeriesDataset::getMaxLength() const {
    return maxLength;
}

std::vector<double> TimeSeriesDataset::zNormalizeSeries(const std::vector<double>& series) const {
    if (series.empty()) return {};
    double sum = std::accumulate(series.begin(), series.end(), 0.0);
    double mean = sum / series.size();
    double sq_sum = 0.0;
    for (double v : series) {
        double d = v - mean;
        sq_sum += d * d;
    }
    double var = sq_sum / series.size();
    double stddev = std::sqrt(var);
    if (stddev == 0.0) stddev = 1.0; // avoid division by zero

    std::vector<double> out;
    out.reserve(series.size());
    for (double v : series) out.push_back((v - mean) / stddev);
    return out;
}

double euclidean_distance(const std::vector<double>& x, const std::vector<double>& y) {
    size_t n = std::min(x.size(), y.size());
    double sum = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double d = x[i] - y[i];
        sum += d * d;
    }
    return std::sqrt(sum);
}

double dtw_distance(const std::vector<double>& x, const std::vector<double>& y) {
    size_t nx = x.size();
    size_t ny = y.size();
    if (nx == 0 || ny == 0) return std::numeric_limits<double>::infinity();

    // use (nx+1) x (ny+1) matrix
    std::vector<double> D((nx + 1) * (ny + 1), std::numeric_limits<double>::infinity());
    auto idx = [&](size_t i, size_t j) { return i * (ny + 1) + j; };
    D[idx(0,0)] = 0.0;

    for (size_t i = 1; i <= nx; ++i) {
        for (size_t j = 1; j <= ny; ++j) {
            double d = (x[i-1] - y[j-1]);
            d = d * d;
            double insertion = D[idx(i-1, j)];
            double deletion = D[idx(i, j-1)];
            double match = D[idx(i-1, j-1)];
            double m = std::min(std::min(insertion, deletion), match);
            D[idx(i,j)] = d + m;
        }
    }

    return std::sqrt(D[idx(nx, ny)]);
}

