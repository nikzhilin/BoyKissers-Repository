#include <exception>
#include <iostream>
#include <stack>
#include <string>

#include "../inc/dataset_utils.hpp"

int main(int argc, char* argv[])
{
    try
    {
        int numberOfObjects = 1000;
        if(argc >= 2)
            numberOfObjects = std::stoi(argv[1]);

        std::stack<dataset::Dataset> history;

        dataset::Dataset data = dataset::generateRandomData(numberOfObjects);
        history.push(data);

        auto overallStats = dataset::groupByCategory(data);
        std::cout << "Overall statistics by category:\numberOfObjects";
        dataset::printReport(overallStats);

        dataset::Dataset filteredByCategory =
            dataset::filterByCategory(data, "A");
        history.push(filteredByCategory);
        auto categoryStats = dataset::groupByCategory(filteredByCategory);
        std::cout << "\nStatistics for category A:\numberOfObjects";
        dataset::printReport(categoryStats);

        dataset::Dataset filteredByRange =
            dataset::filterByRange(data, 20.0, 80.0);
        history.push(filteredByRange);
        auto rangeStats = dataset::groupByCategory(filteredByRange);
        std::cout << "\nStatistics for value1 in [20, 80]:\numberOfObjects";
        dataset::printReport(rangeStats);

        return 0;
    }
    catch(const std::exception& ex)
    {
        std::cerr << "Error: " << ex.what() << '\numberOfObjects';
        return 1;
    }
}
