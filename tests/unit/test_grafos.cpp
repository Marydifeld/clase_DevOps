#include <gtest/gtest.h>
#include "../../include/file_utils.h" 

TEST(GraphAlgorithmsTest, FileReaderThrowsException) {
    vector<vector<int>> dists, max_data;
    vector<pair<int, int>> cor;
    EXPECT_THROW(fileReader("archivo_que_no_existe.txt", dists, max_data, cor), std::runtime_error);
}

// Prueba con datos vacíos para aumentar cobertura de seguridad
TEST(GraphAlgorithmsTest, EmptyDataTests) {
    vector<vector<int>> empty_dists;
    EXPECT_TRUE(prim(empty_dists).empty());
    EXPECT_TRUE(solveTSP(empty_dists).empty());
    EXPECT_EQ(maxFlow(empty_dists), 0);
}

TEST(GraphAlgorithmsTest, FileReaderTest) {
    vector<vector<int>> dists, max_data;
    vector<pair<int, int>> cor;
    EXPECT_NO_THROW(fileReader("test1.txt", dists, max_data, cor));
    EXPECT_EQ(dists.size(), 4);
}

TEST(GraphAlgorithmsTest, PrimTest) {
    vector<vector<int>> dists = {{0,16,45,32},{16,0,18,21},{45,18,0,7},{32,21,7,0}};
    vector<int> cables = prim(dists);
    EXPECT_EQ(cables.size(), 4);
    indexToLetter(cables);
}

TEST(GraphAlgorithmsTest, TSPTest) {
    vector<vector<int>> dists = {{0,16,45,32},{16,0,18,21},{45,18,0,7},{32,21,7,0}};
    vector<int> route = solveTSP(dists);
    EXPECT_EQ(route.size(), 5); 
    printTSP(route);
}

TEST(GraphAlgorithmsTest, MaxFlowTest) {
    vector<vector<int>> capacity = {{0,48,12,18},{52,0,42,32},{18,46,0,56},{24,36,52,0}};
    EXPECT_GT(maxFlow(capacity), 0);
}

TEST(GraphAlgorithmsTest, ConvexHullTest) {
    vector<pair<int, int>> points = {{200,500}, {300,100}, {450,150}, {520,480}};
    vector<pair<int, int>> hull = convexHull(points);
    EXPECT_GT(hull.size(), 2);
    printPolygon(hull);
}