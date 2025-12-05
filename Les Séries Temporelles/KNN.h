#pragma once
#include <string>
#include <vector>
#include "TimeSeriesDataset.h"
#include <algorithm>
#include <map>

class KNN {
public:
    KNN(int k, const std::string& similarity_measure)
        : k(k), similarity_measure(similarity_measure) {}

    double evaluate(const TimeSeriesDataset& train, const TimeSeriesDataset& test, const std::vector<int>& ground_truth) const {
        int ntest = test.getNumberOfSamples();
        int ntrain = train.getNumberOfSamples();
        if (ntest == 0 || ntrain == 0) return 0.0;
        int correct = 0;

        for (int i = 0; i < ntest; ++i) {
            std::vector<double> tx = test.getSeries(i);

            std::vector<std::pair<double,int>> dists; dists.reserve(ntrain);
            for (int j = 0; j < ntrain; ++j) {
                std::vector<double> sx = train.getSeries(j);
                double dist = 0.0;
                if (similarity_measure == "dtw") {
                    dist = dtw_distance(sx, tx);
                } else {
                    dist = euclidean_distance(sx, tx);
                }
                dists.emplace_back(dist, train.getLabels()[j]);
            }

            std::sort(dists.begin(), dists.end(), [](const auto &a, const auto &b){ return a.first < b.first; });

            std::map<int,int> counts;
            for (int t = 0; t < std::min(k, static_cast<int>(dists.size())); ++t) {
                counts[dists[t].second]++;
            }

            int bestLabel = -1;
            int bestCount = -1;
            for (const auto &p : counts) {
                if (p.second > bestCount || (p.second == bestCount && (bestLabel == -1 || p.first < bestLabel))) {
                    bestLabel = p.first;
                    bestCount = p.second;
                }
            }

            if (i < static_cast<int>(ground_truth.size()) && bestLabel == ground_truth[i]) ++correct;
        }

        return static_cast<double>(correct) / static_cast<double>(ground_truth.size());
    }

private:
    int k;
    std::string similarity_measure;
};
