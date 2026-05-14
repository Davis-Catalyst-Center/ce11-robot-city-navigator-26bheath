#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "CityMap.hpp"

void printResult(const std::string& algorithm,
                 const std::pair<std::vector<std::string>, int>& result);

int main() {
    CityMap city;

    std::cout << "=== George's City Navigation System ===\n\n";

    city.printCity();
    std::cout << "\n";

    std::cout << "--- Route: Downtown -> Airport ---\n\n";

    /* Test all three algorithms on this route */
        auto resultDijkstra = city.dijkstraPath(0, 2);
        printResult("Dijkstra's Algorithm", resultDijkstra);
    
        auto resultAStar = city.aStarPath(0, 2);
        printResult("A* Search Algorithm", resultAStar);
    
        auto resultBFS = city.greedyPath(0, 2);
        printResult("Greedy Search", resultBFS);

    std::cout << "\n";

    std::cout << "--- Route: Downtown -> Suburb South ---\n\n";

    /* Test all three algorithms on this route */
        resultDijkstra = city.dijkstraPath(0, 7);
        printResult("Dijkstra's Algorithm", resultDijkstra);
    
        resultAStar = city.aStarPath(0, 7);
        printResult("A* Search Algorithm", resultAStar);
    
        resultBFS = city.greedyPath(0, 7);
        printResult("Greedy Search", resultBFS);
    std::cout << "\n";

    std::cout << "--- Route: [Your Choice] ---\n\n";

    /* Pick your own start/end pair and test all three algorithms */
        resultDijkstra = city.dijkstraPath(3, 2);
            printResult("Dijkstra's Algorithm", resultDijkstra);
        
            resultAStar = city.aStarPath(3, 2);
            printResult("A* Search Algorithm", resultAStar);
        
            resultBFS = city.greedyPath(3, 2);
            printResult("Greedy Search", resultBFS);
    return 0;
}

void printResult(const std::string& algorithm,
                 const std::pair<std::vector<std::string>, int>& result) {
    std::cout << "[" << algorithm << "]\n";
    if (result.first.empty() || result.second == -1) {
        std::cout << "  No path found.\n";
        return;
    }
    std::cout << "  Path: ";
    for (int i = 0; i < (int)result.first.size(); i++) {
        std::cout << result.first[i];
        if (i < (int)result.first.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n  Total time: " << result.second << " min\n";
}
