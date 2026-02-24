#pragma once

#include <map>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

namespace dataset {

using Record = std::tuple<int, std::string, double, double>;
using Dataset = std::vector<Record>;

Dataset generateRandomData(int n);
Dataset filterByCategory(const Dataset& data, const std::string& category);
Dataset filterByRange(const Dataset& data, double minVal, double maxVal);

std::tuple<double, double, double> calculateStats(const std::vector<double>& values);
std::map<std::string, std::tuple<double, double, double>> groupByCategory(const Dataset& data);

std::vector<double> extractColumn(const Dataset& data, int columnIndex);
double calculateMean(const std::vector<double>& values);
double calculateMedian(std::vector<double> values);
double calculateStdDev(const std::vector<double>& values, double mean);

void printReport(const std::map<std::string, std::tuple<double, double, double>>& stats);

}  // namespace dataset

