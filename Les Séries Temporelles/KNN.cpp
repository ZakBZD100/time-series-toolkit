#include "KNN.h"
#include "TimeSeriesDataset.h"
#include <algorithm>
#include <map>

KNN::KNN(int k, const std::string& similarity_measure)
    : k(k), similarity_measure(similarity_measure) {}

double KNN::evaluate(const TimeSeriesDataset& train, const TimeSeriesDataset& test, const std::vector<int>& ground_truth) const {
    int ntest = test.getNumberOfSamples();
    int ntrain = train.getNumberOfSamples();
    if (ntest == 0 || ntrain == 0) return 0.0;
    int correct = 0;

    for (int i = 0; i < ntest; ++i) {
        std::vector<double> tx = test.getSeries(i);

        // compute distances to all train samples
        std::vector<std::pair<double,int>> dists; dists.reserve(ntrain);
        for (int j = 0; j < ntrain; ++j) {
            std::vector<double> sx = train.getSeries(j);
            double dist = 0.0;
            if (similarity_measure == "dtw") {
                dist = dtw_distance(sx, tx);
            } else { // default to euclidean
                dist = euclidean_distance(sx, tx);
            }
            dists.emplace_back(dist, train.getLabels()[j]);
        }

        std::sort(dists.begin(), dists.end(), [](const auto &a, const auto &b){ return a.first < b.first; });

        std::map<int,int> counts;
        for (int t = 0; t < std::min(k, static_cast<int>(dists.size())); ++t) {
            counts[dists[t].second]++;
        }

        // find label with max count (tie-breaker: smallest label)
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
#include "KNN.h"
#include <algorithm>
#include <map>
#include <limits>

KNN::KNN(int k, const std::string& similarity_measure)
    : k(k), similarity_measure(similarity_measure) {}

double KNN::evaluate(const TimeSeriesDataset& train, const TimeSeriesDataset& test, const std::vector<int>& ground_truth) const {
    int ntest = test.getNumberOfSamples();
    int ntrain = train.getNumberOfSamples();
    if (ntest == 0 || ntrain == 0) return 0.0;
    std::vector<int> predictions;
    predictions.reserve(ntest);

    for (int ti = 0; ti < ntest; ++ti) {
        std::vector<std::pair<double,int>> distances; distances.reserve(ntrain);
        std::vector<double> tx = test.getSeries(ti);
        for (int tr = 0; tr < ntrain; ++tr) {
            std::vector<double> sx = train.getSeries(tr);
            double d = 0.0;
            if (similarity_measure == "dtw") {
                d = dtw_distance(sx, tx);
            } else {
                d = euclidean_distance(sx, tx);
            }
            distances.push_back({d, train.getLabels()[tr]});
        }

        std::sort(distances.begin(), distances.end(), [](const std::pair<double,int>& a, const std::pair<double,int>& b){
            return a.first < b.first;
        });

        std::map<int,int> counts;
        for (int i = 0; i < std::min(k, static_cast<int>(distances.size())); ++i) {
            counts[distances[i].second]++;
        }

        // majority vote
        int bestLabel = -1; int bestCount = -1;
        for (auto &p : counts) {
            if (p.second > bestCount) { bestCount = p.second; bestLabel = p.first; }
        }
        predictions.push_back(bestLabel);
    }

    // compute accuracy against provided ground_truth
    if (static_cast<int>(ground_truth.size()) != ntest) return 0.0;
    int correct = 0;
    for (int i = 0; i < ntest; ++i) if (predictions[i] == ground_truth[i]) ++correct;
    return static_cast<double>(correct) / static_cast<double>(ntest);
}
