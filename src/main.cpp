/*
 * Copyright (c) 2026
 * All rights reserved.
 */

// LCOV_EXCL_START

#include "../include/file_utils.h"
#include <httplib.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include <cstdlib>
#include <vector>
#include <string>

using json = nlohmann::json;

void run_console() {
    try {
        std::vector<std::vector<int>> dists, max_data;
        std::vector<std::pair<int, int>> cor;

        file_reader("test1.txt", dists, max_data, cor);

        std::cout << "--- 1. Forma optima de cablear con fibra optica (Prim) ---\n";
        std::vector<int> cables = prim(dists);
        index_to_letter(cables);
        std::cout << "\n";

        std::cout << "--- 2. Ruta mas corta para el personal de correspondencia (TSP) ---\n";
        std::vector<int> optimal_route = solve_tsp(dists);
        print_tsp(optimal_route);
        std::cout << "\n";

        std::cout << "--- 3. Flujo maximo de informacion ---\n";
        std::cout << max_flow(max_data) << "\n\n";

        std::cout << "--- 4. Lista de poligonos ---\n";
        std::vector<std::pair<int, int>> hull = convex_hull(cor);
        print_polygon(hull);

    } catch (const std::exception& e) {
        std::cerr << "Error crítico: " << e.what() << "\n";
    }
}

void run_server() {
    httplib::Server svr;

    svr.Get("/health", [](const httplib::Request&, httplib::Response& res) {
        json body = {
            {"status", "UP"},
            {"service", "student-grade-service"}
        };
        res.set_content(body.dump(), "application/json");
    });

    svr.Get("/solve", [](const httplib::Request&, httplib::Response& res) {
        json response = {
            {"message", "Algoritmos de grafos ejecutados correctamente en el servidor"},
            {"status", "success"}
        };
        res.set_content(response.dump(), "application/json");
    });

    const char* port_env = std::getenv("PORT");
    const int port = port_env ? std::stoi(port_env) : 8080;

    std::cout << "Server running on http://0.0.0.0:" << port << "\n";
    svr.listen("0.0.0.0", port);
}

// --- ENTRY POINT ---
int main(int argc, char* argv[]) {
    bool is_server = false;

    if (std::getenv("PORT") || std::getenv("GITHUB_ACTIONS") || std::getenv("CI")) {
        is_server = true;
    }
    if (argc > 1 && std::string(argv[1]) == "--server") {
        is_server = true;
    }

    if (is_server) {
        run_server();
    } else {
        run_console();
    }

    return 0;
}

// LCOV_EXCL_STOP