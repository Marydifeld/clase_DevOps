/*
 * Copyright (c) 2026
 * All rights reserved.
 */

#include <gtest/gtest.h>
#include "../../include/file_utils.h"

using namespace std;

TEST(GraphAlgorithmsTest, FileReaderThrowsException) {
    vector<vector<int>> dists, max_data;
    vector<pair<int, int>> cor;
    EXPECT_THROW(file_reader("archivo_que_no_existe.txt", dists, max_data, cor), std::runtime_error);
}

TEST(GraphAlgorithmsTest, EmptyDataTests) {
    vector<vector<int>> empty_dists;
    EXPECT_TRUE(prim(empty_dists).empty());
    EXPECT_TRUE(solve_tsp(empty_dists).empty());
    EXPECT_EQ(max_flow(empty_dists), 0);
}

TEST(GraphAlgorithmsTest, FileReaderTest) {
    vector<vector<int>> dists, max_data;
    vector<pair<int, int>> cor;
    EXPECT_NO_THROW(file_reader("test1.txt", dists, max_data, cor));
    EXPECT_EQ(dists.size(), 4);
}

TEST(GraphAlgorithmsTest, PrimTest) {
    vector<vector<int>> dists = {{0,16,45,32},{16,0,18,21},{45,18,0,7},{32,21,7,0}};
    vector<int> cables = prim(dists);
    EXPECT_EQ(cables.size(), 4);
    index_to_letter(cables);
}

TEST(GraphAlgorithmsTest, TSPTest) {
    vector<vector<int>> dists = {{0,16,45,32},{16,0,18,21},{45,18,0,7},{32,21,7,0}};
    vector<int> route = solve_tsp(dists);
    EXPECT_EQ(route.size(), 5); 
    print_tsp(route);
}

TEST(GraphAlgorithmsTest, MaxFlowTest) {
    vector<vector<int>> capacity = {{0,48,12,18},{52,0,42,32},{18,46,0,56},{24,36,52,0}};
    EXPECT_GT(max_flow(capacity), 0);
}

TEST(GraphAlgorithmsTest, ConvexHullTest) {
    vector<pair<int, int>> points = {{200,500}, {300,100}, {450,150}, {520,480}};
    vector<pair<int, int>> hull = convex_hull(points);
    EXPECT_GT(hull.size(), 2);
    print_polygon(hull);
}