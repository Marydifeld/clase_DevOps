#pragma once
#include<iostream>
#include <string>
#include <vector>
#include <utility>

using namespace std;

//Leer input
void fileReader(const string& FILE_PATH, vector<vector<int>>& 
                dist_colonias, vector<vector<int>>& max_data,
                vector<pair<int,int>>& coordinates);

//Parte 1- Prim (Greedy approach)
vector<int> prim(const vector<vector<int>>& adj); 
void indexToLetter(vector<int> cables); 

//Parte 2 - TSP (Backtracking con Branch and Bound)
void tspBacktrack(int currentNode, int visitedCount, int currentCost, vector<int>& currentPath, vector<int>& bestPath, int& minCost, vector<bool>& visited, const vector<vector<int>>& dists);
vector<int> solveTSP(const vector<vector<int>>& dists);
void printTSP(const vector<int>& bestPath);

// Parte 3 - Flujo m�ximo (Edmonds-Karp)
bool bfs(const vector<vector<int>>& residualGraph, int source, int sink, vector<int>& parent);
int maxFlow(const vector<vector<int>>& capacity);

// Parte 4 - Graham Scan (Convex Hull)
int orientation(pair<int, int> p, pair<int, int> q, pair<int, int> r);
vector<pair<int, int>> convexHull(vector<pair<int, int>> points);
void printPolygon(const vector<pair<int, int>>& polygon);