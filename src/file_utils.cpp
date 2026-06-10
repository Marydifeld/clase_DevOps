#include "file_utils.h"
#include <fstream>
#include <stdexcept>
#include <sstream> 
#include <queue> 
#include <climits>
#include <algorithm>

using namespace std;

void fileReader(const string& FILE_PATH, vector<vector<int>>& dist_colonias, vector<vector<int>>& max_data, vector<pair<int,int>>& coordinates) {
    ifstream file(FILE_PATH, ios::binary);
    if (!file) throw runtime_error("Could not open file");
    string line; 
    int N = 0; 
    int line_count = 0; 
    while (getline(file, line)) {
        if (line_count == 0) {
            N = stoi(line);
            dist_colonias.resize(N); max_data.resize(N); coordinates.resize(N);
            line_count++; continue; 
        } 
        if (line.find_first_not_of(" \t\r\n") == string::npos) continue;
        stringstream ss(line);  
        if (line_count <= N) {
            int n; while (ss >> n) dist_colonias[line_count - 1].push_back(n);
        } else if (line_count <= 2 * N) {
            int n; while (ss >> n) max_data[line_count - (N + 1)].push_back(n);
        } else {
            string token;
            while (ss >> token) {
                stringstream ss2(token);
                char open, comma, close; int x, y;
                ss2 >> open >> x >> comma >> y >> close;
                coordinates[line_count - (2 * N + 1)] = {x, y};
            }
        }
        line_count++; 
    }
    file.close(); 
}

int nearestNode(const vector<int>& dists, const vector<bool>& visited) {
    int min_val = INT_MAX, min_index = -1; 
    for (size_t i = 0; i < visited.size(); i++) {
        if(!visited[i] && dists[i] < min_val) { min_index = static_cast<int>(i); min_val = dists[i]; }
    }
    return min_index; 
}

vector<int> prim(const vector<vector<int>>& adj) {
    if (adj.empty()) return {};
    size_t n = adj.size(); 
    vector<bool> visited(n, false); vector<int> distance(n, INT_MAX); vector<int> path(n, -1); 
    distance[0] = 0;
    for (size_t i = 0; i < n; i++) {
        int small = nearestNode(distance, visited); 
        if (small == -1) break;
        visited[small] = true; 
        for (size_t j = 0; j < n; j++) {
            if (adj[small][j] > 0 && !visited[j] && adj[small][j] < distance[j]) {
                path[j] = small; distance[j] = adj[small][j]; 
            }
        }
    }
    return path; 
}

void indexToLetter(const vector<int>& cables) { 
    for (size_t i = 0; i < cables.size(); i++) {
        if (cables[i] != -1) cout << '(' << static_cast<char>('A' + cables[i]) << ',' << static_cast<char>('A' + i) << ")\n"; 
    }
}

void tspBacktrack(int currentNode, int visitedCount, int currentCost, vector<int>& currentPath, vector<int>& bestPath, int& minCost, vector<bool>& visited, const vector<vector<int>>& dists) {
    int n = static_cast<int>(dists.size());
    if (visitedCount == n) {
        if (dists[currentNode][0] > 0) {
            int totalCost = currentCost + dists[currentNode][0];
            if (totalCost < minCost) { minCost = totalCost; bestPath = currentPath; }
        }
    } else {
        for (int i = 0; i < n; i++) {
            if (!visited[i] && dists[currentNode][i] > 0) {
                if (currentCost + dists[currentNode][i] < minCost) {
                    visited[i] = true; currentPath.push_back(i);
                    tspBacktrack(i, visitedCount + 1, currentCost + dists[currentNode][i], currentPath, bestPath, minCost, visited, dists);
                    currentPath.pop_back(); visited[i] = false;
                }
            }
        }
    }
}

vector<int> solveTSP(const vector<vector<int>>& dists) {
    if (dists.empty()) return {};
    size_t n = dists.size();
    vector<int> currentPath; vector<int> bestPath; vector<bool> visited(n, false);
    int minCost = INT_MAX; 
    visited[0] = true; currentPath.push_back(0);
    tspBacktrack(0, 1, 0, currentPath, bestPath, minCost, visited, dists);
    if (!bestPath.empty()) bestPath.push_back(0);
    return bestPath;
}

void printTSP(const vector<int>& bestPath) {
    size_t size = bestPath.size();
    for (size_t i = 0; i < size; i++) {
        cout << static_cast<char>('A' + bestPath[i]);
        if (i < size - 1) cout << " -> ";
    }
    cout << "\n";
}

bool bfs(const vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent) {
    size_t n = residualGraph.size(); vector<bool> visited(n, false); queue<int> q;
    q.push(source); visited[source] = true; parent[source] = -1;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        for (size_t v = 0; v < n; v++) {
            if (!visited[v] && residualGraph[u][v] > 0) {
                q.push(static_cast<int>(v)); parent[v] = static_cast<int>(u); visited[v] = true;
            }
        }
    }
    return visited[sink];
}

int maxFlow(const vector<vector<int>>& capacity) {
    if (capacity.empty()) return 0;
    int n = static_cast<int>(capacity.size()); int source = 0; int sink = n - 1;
    vector<vector<int>> residualGraph = capacity; vector<int> parent(n); int max_flow = 0;
    while (bfs(residualGraph, source, sink, parent)) {
        int pathFlow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v]; pathFlow = min(pathFlow, residualGraph[u][v]);
        }
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v]; residualGraph[u][v] -= pathFlow; residualGraph[v][u] += pathFlow;
        }
        max_flow += pathFlow;
    }
    return max_flow;
}

int orientation(pair<int, int> p, pair<int, int> q, pair<int, int> r) {
    long long val = static_cast<long long>(q.second - p.second) * (r.first - q.first) - static_cast<long long>(q.first - p.first) * (r.second - q.second);
    if (val == 0) return 0;
    return (val > 0) ? 1 : 2;
}

vector<pair<int, int>> convexHull(vector<pair<int, int>> points) {
    size_t n = points.size(); if (n <= 3) return points;
    sort(points.begin(), points.end()); vector<pair<int, int>> lower;
    for (const auto& p : points) {
        while (lower.size() >= 2) {
            auto q = lower[lower.size() - 1]; auto r = lower[lower.size() - 2];
            long long cross = static_cast<long long>(q.first - r.first) * (p.second - r.second) - static_cast<long long>(q.second - r.second) * (p.first - r.first);
            if (cross > 0) break;
            lower.pop_back();
        }
        lower.push_back(p);
    }
    vector<pair<int, int>> upper;
    for (int i = static_cast<int>(n) - 1; i >= 0; i--) {
        auto p = points[i];
        while (upper.size() >= 2) {
            auto q = upper[upper.size() - 1]; auto r = upper[upper.size() - 2];
            long long cross = static_cast<long long>(q.first - r.first) * (p.second - r.second) - static_cast<long long>(q.second - r.second) * (p.first - r.first);
            if (cross > 0) break;
            upper.pop_back();
        }
        upper.push_back(p);
    }
    lower.pop_back(); upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

void printPolygon(const vector<pair<int, int>>& polygon) {
    for (const auto& p : polygon) { cout << "(" << p.first << "," << p.second << ") "; }
    cout << "\n";
}