#include "../inc/dataset_utils.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <random>
#include <stdexcept>

namespace dataset {

Dataset generateRandomData(int n)
{
    if(n < 0)
        throw std::invalid_argument("Size of dataset must be non-negative.");

    Dataset data(static_cast<std::size_t>(n));

    std::vector<std::string> categories = {"A", "B", "C", "D", "E"};

#pragma omp parallel
    {
        std::mt19937 rng(
            std::random_device{}() ^
            static_cast<std::mt19937::result_type>(omp_get_thread_num()));
        std::uniform_int_distribution<int> idDist(1, std::max(1, n));
        std::uniform_int_distribution<std::size_t> categoryDist(
            0, categories.size() - 1);
        std::uniform_real_distribution<double> valueDist(0.0, 100.0);

#pragma omp for
        for(int i = 0; i < n; ++i)
        {
            int id = idDist(rng);
            const std::string& category = categories[categoryDist(rng)];
            double value1 = valueDist(rng);
            double value2 = valueDist(rng);
            data[static_cast<std::size_t>(i)] =
                std::make_tuple(id, category, value1, value2);
        }
    }

    return data;
}

Dataset filterByCategory(const Dataset& data, const std::string& category)
{
    Dataset result;
    result.reserve(data.size());

    for(const auto& record: data)
        if(std::get<1>(record) == category)
            result.push_back(record);

    return result;
}

Dataset filterByRange(const Dataset& data, double minVal, double maxVal)
{
    if(minVal > maxVal)
        throw std::invalid_argument("minVal must be <= maxVal.");

    Dataset result;
    result.reserve(data.size());

    for(const auto& record: data)
    {
        double value1 = std::get<2>(record);
        if(value1 >= minVal && value1 <= maxVal)
            result.push_back(record);
    }

    return result;
}

double calculateMean(const std::vector<double>& values)
{
    if(values.empty())
        throw std::invalid_argument("Cannot compute mean of empty vector.");

    double sum = std::accumulate(values.begin(), values.end(), 0.0);
    return sum / static_cast<double>(values.size());
}

double calculateMedian(std::vector<double> values)
{
    if(values.empty())
        throw std::invalid_argument("Cannot compute median of empty vector.");

    std::sort(values.begin(), values.end());
    std::size_t n = values.size();
    if(n % 2 == 1)
        return values[n / 2];

    return (values[n / 2 - 1] + values[n / 2]) / 2.0;
}

double calculateStdDev(const std::vector<double>& values, double mean)
{
    if(values.empty())
        throw std::invalid_argument(
            "Cannot compute standard deviation of empty vector.");

    double sumSquares = 0.0;
#pragma omp parallel for reduction(+ : sumSquares)
    for(std::size_t i = 0; i < values.size(); ++i)
    {
        double diff = values[i] - mean;
        sumSquares += diff * diff;
    }

    double variance = sumSquares / static_cast<double>(values.size());
    return std::sqrt(variance);
}

std::tuple<double, double, double>
calculateStats(const std::vector<double>& values)
{
    if(values.empty())
        throw std::invalid_argument("Cannot compute stats of empty vector.");

    double mean = calculateMean(values);
    auto [minIt, maxIt] = std::minmax_element(values.begin(), values.end());

    return std::make_tuple(mean, *minIt, *maxIt);
}

std::vector<double> extractColumn(const Dataset& data, int columnIndex)
{
    if(columnIndex != 2 && columnIndex != 3)
        throw std::invalid_argument(
            "extractColumn supports only indices 2 (value1) and 3 (value2).");

    std::vector<double> result;
    result.reserve(data.size());

    for(const auto& record: data)
        if(columnIndex == 2)
            result.push_back(std::get<2>(record));
        else
            result.push_back(std::get<3>(record));

    return result;
}

std::map<std::string, std::tuple<double, double, double>>
groupByCategory(const Dataset& data)
{
    std::unordered_map<std::string, std::vector<double>> groupedValues;

    for(const auto& record: data)
    {
        const std::string& category = std::get<1>(record);
        double value1 = std::get<2>(record);
        groupedValues[category].push_back(value1);
    }

    std::map<std::string, std::tuple<double, double, double>> statsByCategory;
    std::vector<std::pair<const std::string, std::vector<double>*>> entries;
    entries.reserve(groupedValues.size());
    for(auto& [category, values]: groupedValues)
        entries.emplace_back(category, &values);

#pragma omp parallel for
    for(std::size_t i = 0; i < entries.size(); ++i)
    {
        const std::string& category = entries[i].first;
        std::vector<double>& values = *entries[i].second;
        auto stats = calculateStats(values);

#pragma omp critical
        {
            statsByCategory[category] = stats;
        }
    }

    return statsByCategory;
}

void printReport(
    const std::map<std::string, std::tuple<double, double, double>>& stats)
{
    if(stats.empty())
    {
        std::cout << "No data to report.\n";
        return;
    }

    std::cout << std::fixed << std::setprecision(2);
    std::cout << "Category\tMean\tMin\tMax\n";

    for(const auto& [category, triple]: stats)
    {
        double mean{};
        double min{};
        double max{};
        std::tie(mean, min, max) = triple;
        std::cout << category << '\t' << mean << '\t' << min << '\t' << max
                  << '\n';
    }
}

}  // namespace dataset
