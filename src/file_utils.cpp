// Copyright 2026
#include "file_utils.h"
#include <fstream>
#include <stdexcept>
#include <sstream>
#include <queue>
#include <climits>
#include <algorithm>

void file_reader(const std::string& file_path, std::vector<std::vector<int>>& dist_colonias, std::vector<std::vector<int>>& max_data, std::vector<std::pair<int, int>>& coordinates) {
    std::ifstream file(file_path, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Could not open file");
    }
    std::string line;
    int n_val = 0;
    int line_count = 0;
    while (std::getline(file, line)) {
        if (line.find_first_not_of(" \t\r\n") == std::string::npos) {
            continue;
        }
        if (line_count == 0) {
            n_val = std::stoi(line);
            dist_colonias.resize(n_val);
            max_data.resize(n_val);
            coordinates.resize(n_val);
            line_count++;
            continue;
        }
        std::stringstream ss(line);
        if (line_count <= n_val) {
            int n;
            while (ss >> n) {
                dist_colonias[line_count - 1].push_back(n);
            }
        } else if (line_count <= 2 * n_val) {
            int n;
            while (ss >> n) {
                max_data[line_count - (n_val + 1)].push_back(n);
            }
        } else {
            std::string token;
            while (ss >> token) {
                std::stringstream ss2(token);
                char open;
                char comma;
                char close;
                int x;
                int y;
                ss2 >> open >> x >> comma >> y >> close;
                coordinates[line_count - (2 * n_val + 1)] = {x, y};
            }
        }
        line_count++;
    }
    file.close();
}

int nearest_node(const std::vector<int>& dists, const std::vector<bool>& visited) {
    int min_val = INT_MAX;
    int min_index = -1;
    for (size_t i = 0; i < visited.size(); i++) {
        if (!visited[i] && dists[i] < min_val) {
            min_index = static_cast<int>(i);
            min_val = dists[i];
        }
    }
    return min_index;
}

std::vector<int> prim(const std::vector<std::vector<int>>& adj) {
    if (adj.empty()) {
        return {};
    }
    size_t n = adj.size();
    std::vector<bool> visited(n, false);
    std::vector<int> distance(n, INT_MAX);
    std::vector<int> path(n, -1);
    distance[0] = 0;
    for (size_t i = 0; i < n; i++) {
        int small = nearest_node(distance, visited);
        if (small == -1) {
            break;
        }
        visited[small] = true;
        for (size_t j = 0; j < n; j++) {
            if (adj[small][j] > 0 && !visited[j] && adj[small][j] < distance[j]) {
                path[j] = small;
                distance[j] = adj[small][j];
            }
        }
    }
    return path;
}

void index_to_letter(const std::vector<int>& cables) {
    for (size_t i = 0; i < cables.size(); i++) {
        if (cables[i] != -1) {
            std::cout << '(' << static_cast<char>('A' + cables[i]) << ',' << static_cast<char>('A' + i) << ")\n";
        }
    }
}

void tsp_backtrack(int current_node, int visited_count, int current_cost, std::vector<int>& current_path, std::vector<int>& best_path, int& min_cost, std::vector<bool>& visited, const std::vector<std::vector<int>>& dists) {
    int n = static_cast<int>(dists.size());
    if (visited_count == n) {
        if (dists[current_node][0] > 0) {
            int total_cost = current_cost + dists[current_node][0];
            if (total_cost < min_cost) {
                min_cost = total_cost;
                best_path = current_path;
            }
        }
        return;
    }
    for (int i = 0; i < n; i++) {
        if (visited[i] || dists[current_node][i] <= 0) {
            continue;
        }
        if (current_cost + dists[current_node][i] >= min_cost) {
            continue;
        }
        visited[i] = true;
        current_path.push_back(i);
        tsp_backtrack(i, visited_count + 1, current_cost + dists[current_node][i], current_path, best_path, min_cost, visited, dists);
        current_path.pop_back();
        visited[i] = false;
    }
}

std::vector<int> solve_tsp(const std::vector<std::vector<int>>& dists) {
    if (dists.empty()) {
        return {};
    }
    size_t n = dists.size();
    std::vector<int> current_path;
    std::vector<int> best_path;
    std::vector<bool> visited(n, false);
    int min_cost = INT_MAX;
    visited[0] = true;
    current_path.push_back(0);
    tsp_backtrack(0, 1, 0, current_path, best_path, min_cost, visited, dists);
    if (!best_path.empty()) {
        best_path.push_back(0);
    }
    return best_path;
}

void print_tsp(const std::vector<int>& best_path) {
    size_t size = best_path.size();
    for (size_t i = 0; i < size; i++) {
        std::cout << static_cast<char>('A' + best_path[i]);
        if (i < size - 1) {
            std::cout << " -> ";
        }
    }
    std::cout << "\n";
}

bool bfs(const std::vector<std::vector<int>>& residual_graph, int source, int sink, std::vector<int>& parent) {
    size_t n = residual_graph.size();
    std::vector<bool> visited(n, false);
    std::queue<int> q;
    q.push(source);
    visited[source] = true;
    parent[source] = -1;
    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (size_t v = 0; v < n; v++) {
            if (!visited[v] && residual_graph[u][v] > 0) {
                q.push(static_cast<int>(v));
                parent[v] = static_cast<int>(u);
                visited[v] = true;
            }
        }
    }
    return visited[sink];
}

int max_flow(const std::vector<std::vector<int>>& capacity) {
    if (capacity.empty()) {
        return 0;
    }
    int n = static_cast<int>(capacity.size());
    int source = 0;
    int sink = n - 1;
    std::vector<std::vector<int>> residual_graph = capacity;
    std::vector<int> parent(n);
    int max_flow_val = 0;
    while (bfs(residual_graph, source, sink, parent)) {
        int path_flow = INT_MAX;
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            path_flow = std::min(path_flow, residual_graph[u][v]);
        }
        for (int v = sink; v != source; v = parent[v]) {
            int u = parent[v];
            residual_graph[u][v] -= path_flow;
            residual_graph[v][u] += path_flow;
        }
        max_flow_val += path_flow;
    }
    return max_flow_val;
}

int orientation(std::pair<int, int> p, std::pair<int, int> q, std::pair<int, int> r) {
    long long val1 = static_cast<long long>(q.second - p.second) * (r.first - q.first);
    long long val2 = static_cast<long long>(q.first - p.first) * (r.second - q.second);
    long long val = val1 - val2;
    if (val == 0) {
        return 0;
    }
    return (val > 0) ? 1 : 2;
}

std::vector<std::pair<int, int>> convex_hull(std::vector<std::pair<int, int>> points) {
    size_t n = points.size();
    if (n <= 3) {
        return points;
    }
    std::sort(points.begin(), points.end());
    std::vector<std::pair<int, int>> lower;
    for (const auto& p : points) {
        while (lower.size() >= 2) {
            auto q = lower[lower.size() - 1];
            auto r = lower[lower.size() - 2];
            long long cross1 = static_cast<long long>(q.first - r.first) * (p.second - r.second);
            long long cross2 = static_cast<long long>(q.second - r.second) * (p.first - r.first);
            if ((cross1 - cross2) > 0) {
                break;
            }
            lower.pop_back();
        }
        lower.push_back(p);
    }
    std::vector<std::pair<int, int>> upper;
    for (int i = static_cast<int>(n) - 1; i >= 0; i--) {
        auto p = points[i];
        while (upper.size() >= 2) {
            auto q = upper[upper.size() - 1];
            auto r = upper[upper.size() - 2];
            long long cross1 = static_cast<long long>(q.first - r.first) * (p.second - r.second);
            long long cross2 = static_cast<long long>(q.second - r.second) * (p.first - r.first);
            if ((cross1 - cross2) > 0) {
                break;
            }
            upper.pop_back();
        }
        upper.push_back(p);
    }
    lower.pop_back();
    upper.pop_back();
    lower.insert(lower.end(), upper.begin(), upper.end());
    return lower;
}

void print_polygon(const std::vector<std::pair<int, int>>& polygon) {
    for (const auto& p : polygon) {
        std::cout << "(" << p.first << "," << p.second << ") ";
    }
    std::cout << "\n";
}