#include "CityMap.hpp"
#include <iostream>
#include <cmath>
#include <algorithm>
#include <queue>
#include <limits>

CityMap::CityMap() {
    locations.resize(8);

    locations[0].name = "Downtown";
    locations[0].x = 4; locations[0].y = 4;
    locations[0].neighbors = {{1, 8}, {3, 15}, {6, 12}};

    locations[1].name = "Harbor";
    locations[1].x = 4; locations[1].y = 0;
    locations[1].neighbors = {{0, 8}, {2, 20}, {4, 10}};

    locations[2].name = "Airport";
    locations[2].x = 10; locations[2].y = 0;
    locations[2].neighbors = {{1, 20}, {4, 5}, {7, 18}};

    locations[3].name = "University";
    locations[3].x = 0; locations[3].y = 6;
    locations[3].neighbors = {{0, 15}, {5, 9}, {6, 7}};

    locations[4].name = "Industrial";
    locations[4].x = 9; locations[4].y = 1;
    locations[4].neighbors = {{1, 10}, {2, 5}, {7, 8}};

    locations[5].name = "Medical Center";
    locations[5].x = 2; locations[5].y = 9;
    locations[5].neighbors = {{3, 9}, {6, 11}, {7, 14}};

    locations[6].name = "Suburb North";
    locations[6].x = 1; locations[6].y = 5;
    locations[6].neighbors = {{0, 12}, {3, 7}, {5, 11}};

    locations[7].name = "Suburb South";
    locations[7].x = 8; locations[7].y = 8;
    locations[7].neighbors = {{2, 18}, {4, 8}, {5, 14}};
}

void CityMap::printCity() const {
    std::cout << "City Locations:\n";
    for (int i = 0; i < (int)locations.size(); i++) {
        std::cout << "  [" << i << "] " << locations[i].name << "\n";
        std::cout << "       neighbors: ";
        for (int j = 0; j < (int)locations[i].neighbors.size(); j++) {
            auto [idx, time] = locations[i].neighbors[j];
            std::cout << locations[idx].name << "(" << time << ")";
            if (j < (int)locations[i].neighbors.size() - 1) std::cout << ", ";
        }
        std::cout << "\n";
    }
}

int CityMap::heuristic(Location from, Location to) const {
    int result = sqrt( (to.x-from.x)^2 + (to.y-from.y)^2 );
    return result;
}

 std::vector<std::string> CityMap::reconstructPath(const std::vector<int>& prev, int start, int end) const{
    std::vector<int> path;
    int current = end;
    path.push_back(current);

    while (current != start) {
        if (prev[current] == -1)
            return {};
        current = prev[current];
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());

    std::vector<std::string> pathNames;
    for (int idx : path) {
        pathNames.push_back(locations[idx].name);
    }

    return pathNames;
 }

 std::pair<std::vector<std::string>, int> CityMap::greedyPath(int start, int end){
    if (start == end) {
        return {{locations[start].name}, 0};
    }

    int n = static_cast<int>(locations.size());
    std::vector<int> prev(n, -1);
    std::vector<bool> visited(n, false);

    using P = std::pair<int,int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    pq.emplace(heuristic(locations[start], locations[end]), start);

    while (!pq.empty()) {
        auto [h, node] = pq.top();
        pq.pop();

        if (visited[node]) continue;
        visited[node] = true;

        if (node == end) break;

        for (const auto& neighbor : locations[node].neighbors) {
            int neiIndex = neighbor.first;
            int travelTime = neighbor.second;

            if (!visited[neiIndex]) {
                if (prev[neiIndex] == -1) {
                    prev[neiIndex] = node;
                }
                pq.emplace(heuristic(locations[neiIndex], locations[end]), neiIndex);
            }
        }
    }

    if (prev[end] == -1) {
        return {{}, -1};
    }

    std::vector<std::string> path = reconstructPath(prev, start, end);

    int totalCost = 0;
    for (size_t i = 0; i + 1 < path.size(); ++i) {
        int fromIdx = -1, toIdx = -1;
        for (int j = 0; j < n; ++j) {
            if (locations[j].name == path[i]) fromIdx = j;
            if (locations[j].name == path[i+1]) toIdx = j;
        }
        if (fromIdx == -1 || toIdx == -1) continue;

        for (const auto& neighbor : locations[fromIdx].neighbors) {
            if (neighbor.first == toIdx) {
                totalCost += neighbor.second;
                break;
            }
        }
    }

    return {path, totalCost};
 }

 std::pair<std::vector<std::string>, int> CityMap::dijkstraPath(int start, int end){
if (start == end) {
        return {{locations[start].name}, 0};
    }

    int n = static_cast<int>(locations.size());
    std::vector<int> prev(n, -1);
    std::vector<int> dist(n, std::numeric_limits<int>::max());
    std::vector<bool> visited(n, false);

    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> pq;

    dist[start] = 0;
    pq.emplace(0, start);

    while (!pq.empty()) {
        auto [currentDist, node] = pq.top();
        pq.pop();

        if (visited[node]) continue;
        visited[node] = true;

        if (node == end) break;

        for (const auto& neighbor : locations[node].neighbors) {
            int neiIndex = neighbor.first;
            int travelTime = neighbor.second;

            if (visited[neiIndex]) continue;

            int newDist = currentDist + travelTime;
            if (newDist < dist[neiIndex]) {
                dist[neiIndex] = newDist;
                prev[neiIndex] = node;
                pq.emplace(newDist, neiIndex);
            }
        }
    }

    if (prev[end] == -1) {
        return {{}, -1};
    }

    std::vector<std::string> path = reconstructPath(prev, start, end);

    int totalCost = dist[end];

    return {path, totalCost};
 }

 std::pair<std::vector<std::string>, int> CityMap::aStarPath(int start, int end){
if (start == end) {
        return {{locations[start].name}, 0};
    }

    int n = static_cast<int>(locations.size());
    std::vector<int> prev(n, -1);
    std::vector<int> gScore(n, std::numeric_limits<int>::max());
    std::vector<int> fScore(n, std::numeric_limits<int>::max());
    std::vector<bool> visited(n, false);

    using P = std::pair<int, int>;
    std::priority_queue<P, std::vector<P>, std::greater<P>> openSet;

    gScore[start] = 0;
    fScore[start] = heuristic(locations[start], locations[end]);
    openSet.emplace(fScore[start], start);

    while (!openSet.empty()) {
        auto [currentF, current] = openSet.top();
        openSet.pop();

        if (visited[current]) continue;
        visited[current] = true;

        if (current == end) break;

        for (const auto& neighbor : locations[current].neighbors) {
            int neiIndex = neighbor.first;
            int travelTime = neighbor.second;

            if (visited[neiIndex]) continue;

            int tentative_gScore = gScore[current] + travelTime;
            if (tentative_gScore < gScore[neiIndex]) {
                prev[neiIndex] = current;
                gScore[neiIndex] = tentative_gScore;
                fScore[neiIndex] = tentative_gScore + heuristic(locations[neiIndex], locations[end]);
                openSet.emplace(fScore[neiIndex], neiIndex);
            }
        }
    }

    if (prev[end] == -1) {
        return {{}, -1};
    }

    std::vector<std::string> path = reconstructPath(prev, start, end);

    int totalCost = gScore[end];

    return {path, totalCost};
 }