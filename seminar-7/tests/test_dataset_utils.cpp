#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_approx.hpp>

#include <string>
#include <tuple>
#include <vector>

#include "../inc/dataset_utils.hpp"

TEST_CASE("filterByCategory returns only requested category")
{
    dataset::Dataset data = {
        dataset::Record{1, "A", 10.0, 1.0},
        dataset::Record{2, "B", 20.0, 2.0},
        dataset::Record{3, "A", 30.0, 3.0},
    };

    auto filtered = dataset::filterByCategory(data, "A");

    REQUIRE(filtered.size() == 2);
    for(const auto& record : filtered)
    {
        REQUIRE(std::get<1>(record) == "A");
    }
}

TEST_CASE("filterByRange filters by value1 in inclusive range")
{
    dataset::Dataset data = {
        dataset::Record{1, "A", 10.0, 1.0},
        dataset::Record{2, "A", 20.0, 2.0},
        dataset::Record{3, "A", 30.0, 3.0},
    };

    auto filtered = dataset::filterByRange(data, 15.0, 25.0);

    REQUIRE(filtered.size() == 1);
    REQUIRE(std::get<0>(filtered[0]) == 2);
}

TEST_CASE("calculateMean, calculateMedian and calculateStdDev work correctly")
{
    std::vector<double> values = {1.0, 2.0, 3.0, 4.0};

    double mean = dataset::calculateMean(values);
    double median = dataset::calculateMedian(values);
    double stddev = dataset::calculateStdDev(values, mean);

    REQUIRE(mean == Catch::Approx(2.5));
    REQUIRE(median == Catch::Approx(2.5));
    REQUIRE(stddev == Catch::Approx(1.1180).margin(0.0005));
}

TEST_CASE("extractColumn returns correct column for value1 and value2")
{
    dataset::Dataset data = {
        dataset::Record{1, "A", 10.0, 100.0},
        dataset::Record{2, "B", 20.0, 200.0},
    };

    auto col1 = dataset::extractColumn(data, 2);
    auto col2 = dataset::extractColumn(data, 3);

    REQUIRE(col1.size() == 2);
    REQUIRE(col2.size() == 2);

    REQUIRE(col1[0] == Catch::Approx(10.0));
    REQUIRE(col1[1] == Catch::Approx(20.0));
    REQUIRE(col2[0] == Catch::Approx(100.0));
    REQUIRE(col2[1] == Catch::Approx(200.0));
}

TEST_CASE("groupByCategory aggregates mean, min and max by category")
{
    dataset::Dataset data = {
        dataset::Record{1, "A", 10.0, 1.0},
        dataset::Record{2, "A", 30.0, 2.0},
        dataset::Record{3, "B", 5.0, 3.0},
    };

    auto stats = dataset::groupByCategory(data);

    REQUIRE(stats.size() == 2);

    {
        auto it = stats.find("A");
        REQUIRE(it != stats.end());
        double mean{}, min{}, max{};
        std::tie(mean, min, max) = it->second;
        REQUIRE(mean == Catch::Approx(20.0));
        REQUIRE(min == Catch::Approx(10.0));
        REQUIRE(max == Catch::Approx(30.0));
    }

    {
        auto it = stats.find("B");
        REQUIRE(it != stats.end());
        double mean{}, min{}, max{};
        std::tie(mean, min, max) = it->second;
        REQUIRE(mean == Catch::Approx(5.0));
        REQUIRE(min == Catch::Approx(5.0));
        REQUIRE(max == Catch::Approx(5.0));
    }
}

