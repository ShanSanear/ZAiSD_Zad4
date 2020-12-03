#include <ctime>
#include <omp.h>
#include <vector>
#include <fstream>
#include <filesystem>
#include <iterator>
#include <iostream>
#include <chrono>
#include <sstream>
#include <queue>
#include <stack>
#include <algorithm>

const bool DEBUG = true;

class Graph {

public:
    /// Constructor
    Graph();

    /// Shows graph's matrix
    void show_graph_matrix();

    void load_graph_matrix_from_stdin();


    void deallocate_memory();

private:
    /// Matrix to be searched
    int **matrix;
    /// Distances matrix
    int **matrix_distances;
    /// How many edges has already been loaded
    int vertex_count = 0;
    int number_of_edges = 0;

    void add_edge(int vertex_u, int vertex_v, int weight);


    void initialize_matrix(int num_of_vectors);

};


void Graph::add_edge(int vertex_u, int vertex_v, int weight) {
    matrix[vertex_u][vertex_v] = weight;
    if (DEBUG) {
        printf("Adding edge: %d -> %d : %d\n", vertex_u, vertex_v, weight);
    }
}

void Graph::show_graph_matrix() {
    int i, j;
    for (i = 0; i < vertex_count; i++) {
        for (j = 0; j < vertex_count; j++) {
            printf("%*d ", 2, matrix[i][j]);
        }
        printf("\n");
    }
}

void Graph::load_graph_matrix_from_stdin() {
    std::string line;
    std::vector<std::vector<int>> input_data;
    int num_of_vectors;
    std::getline(std::cin, line);
    int space_index = line.find(' ');
    num_of_vectors = std::stoi(line.substr(0, space_index));
    number_of_edges = std::stoi(line.substr(space_index + 1, line.size()));
    initialize_matrix(num_of_vectors);
    for (int i = 0; i < number_of_edges; i++) {
        std::vector<int> row;
        std::getline(std::cin, line);
        std::stringstream basic_stringstream(line);
        std::string provided_number;
        while (std::getline(basic_stringstream, provided_number, ' ')) {
            int number = std::stoi(provided_number);
            row.push_back(number);
        }
        add_edge(row[0]-1, row[1]-1, row[2]);
    }
}

void Graph::initialize_matrix(int num_of_vectors) {
    matrix = new int *[num_of_vectors];
    for (int i = 0; i < num_of_vectors; ++i) {
        matrix[i] = new int[num_of_vectors];
        for (int j = 0; j < num_of_vectors; j++) {
            matrix[i][j] = 0;
        }
    }
    vertex_count = num_of_vectors;
}

void Graph::deallocate_memory() {
    for (int i = 0; i < vertex_count; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}


Graph::Graph() = default;


int main() {
    int number_of_cases;
    std::string line;
    std::getline(std::cin, line);
    number_of_cases = std::stoi(line);
    for (int case_num = 0; case_num < number_of_cases; case_num++) {
        Graph graph = Graph();
        graph.load_graph_matrix_from_stdin();
        if (DEBUG) {
            graph.show_graph_matrix();
        }
        graph.deallocate_memory();
        if (case_num != number_of_cases-1) {
            printf("\n");
        }
    }
    return 0;
}