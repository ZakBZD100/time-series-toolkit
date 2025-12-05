#pragma once
#include <vector>

class TimeSeriesDataset {
public:
    TimeSeriesDataset(bool znormalize = true, bool isTrain = true, int maxLength = 0);
    void addSample(const std::vector<double>& series, int label);
    void addTimeSeries(const std::vector<double>& series, int label);
    void addTimeSeries(const std::vector<double>& series);

    std::vector<double> getSeries(int index) const;
    int getNumberOfSamples() const;
    const std::vector<int>& getLabels() const;
    int getMaxLength() const;

    bool znormalize;
    bool isTrain;
    std::vector<double> data;
    std::vector<int> labels;
    int maxLength;
    int numberOfSamples;

private:
    std::vector<double> zNormalizeSeries(const std::vector<double>& series) const;
};

double euclidean_distance(const std::vector<double>& x, const std::vector<double>& y);

double dtw_distance(const std::vector<double>& x, const std::vector<double>& y);
