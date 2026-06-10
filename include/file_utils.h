/*
 * Copyright (c) 2026
 * All rights reserved.
 */

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <utility>

/**
 * \brief Reads the input file and populates the data structures.
 * \param file_path The path to the input file.
 * \param dist_colonias Matrix to store distances.
 * \param max_data Matrix to store max flow capacities.
 * \param coordinates Vector to store coordinates.
 */
void file_reader(const std::string& file_path,
                 std::vector<std::vector<int>>& dist_colonias,
                 std::vector<std::vector<int>>& max_data,
                 std::vector<std::pair<int, int>>& coordinates);

/**
 * \brief Computes the Minimum Spanning Tree using Prim's algorithm.
 * \param adj The adjacency matrix.
 * \return A vector representing the parent of each node in the MST.
 */
std::vector<int> prim(const std::vector<std::vector<int>>& adj);

/**
 * \brief Prints the cables based on the MST parent array.
 * \param cables The parent array from Prim's algorithm.
 */
void index_to_letter(const std::vector<int>& cables);

/**
 * \brief Backtracking helper for TSP.
 * \param current_node The current node.
 * \param visited_count The number of visited nodes.
 * \param current_cost The accumulated cost.
 * \param current_path The current path being evaluated.
 * \param best_path The best path found so far.
 * \param min_cost The minimum cost found so far.
 * \param visited Boolean vector of visited nodes.
 * \param dists Adjacency matrix of distances.
 */
void tsp_backtrack(int current_node, int visited_count, int current_cost,
                   std::vector<int>& current_path, std::vector<int>& best_path,
                   int& min_cost, std::vector<bool>& visited,
                   const std::vector<std::vector<int>>& dists);

/**
 * \brief Solves the Traveling Salesperson Problem.
 * \param dists Adjacency matrix of distances.
 * \return The optimal path.
 */
std::vector<int> solve_tsp(const std::vector<std::vector<int>>& dists);

/**
 * \brief Prints the TSP path.
 * \param best_path The path to print.
 */
void print_tsp(const std::vector<int>& best_path);

/**
 * \brief BFS helper for Max Flow.
 * \param residual_graph The residual graph.
 * \param source The source node.
 * \param sink The sink node.
 * \param parent Parent array to store path.
 * \return True if sink is reachable, false otherwise.
 */
bool bfs(const std::vector<std::vector<int>>& residual_graph, int source, int sink, std::vector<int>& parent);

/**
 * \brief Computes the maximum flow using Edmonds-Karp.
 * \param capacity Capacity matrix.
 * \return The maximum flow value.
 */
int max_flow(const std::vector<std::vector<int>>& capacity);

/**
 * \brief Computes the orientation of three points.
 * \param p First point.
 * \param q Second point.
 * \param r Third point.
 * \return 0 if collinear, 1 if clockwise, 2 if counterclockwise.
 */
int orientation(std::pair<int, int> p, std::pair<int, int> q, std::pair<int, int> r);

/**
 * \brief Computes the convex hull using Graham Scan.
 * \param points The vector of points.
 * \return The points that form the convex hull.
 */
std::vector<std::pair<int, int>> convex_hull(std::vector<std::pair<int, int>> points);

/**
 * \brief Prints the polygon vertices.
 * \param polygon The vertices of the polygon.
 */
void print_polygon(const std::vector<std::pair<int, int>>& polygon);
