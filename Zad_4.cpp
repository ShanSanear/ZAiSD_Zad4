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

int **initialize_matrix(int num_of_vectors);

class Graph {

public:
    /// Constructor
    Graph();

    /// Shows graph's matrix
    void show_graph_matrix();

    void load_graph_matrix_from_stdin();


    void deallocate_memory();


    void check_distances(int src);

private:
    /// Matrix to be searched
    int **matrix;
    /// How many edges has already been loaded
    int vertex_count;
    int number_of_edges;

    void add_edge(int vertex_u, int vertex_v, int weight);

    void initialize_matrices();

    int **initialize_matrix(int init_value) const;

    int minimum_distance(const int p_int[], const bool p_boolean[]);
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
    std::getline(std::cin, line);
    int space_index = line.find(' ');
    vertex_count = std::stoi(line.substr(0, space_index));
    number_of_edges = std::stoi(line.substr(space_index + 1, line.size()));
    initialize_matrices();
    for (int i = 0; i < number_of_edges; i++) {
        std::vector<int> row;
        std::getline(std::cin, line);
        std::stringstream basic_stringstream(line);
        std::string provided_number;
        while (std::getline(basic_stringstream, provided_number, ' ')) {
            int number = std::stoi(provided_number);
            row.push_back(number);
        }
        add_edge(row[0] - 1, row[1] - 1, row[2]);
    }
}

void Graph::initialize_matrices() {
    matrix = initialize_matrix(0);
}

int **Graph::initialize_matrix(int init_value) const {
    int **matrix_to_initialize = new int *[vertex_count];
    for (int i = 0; i < vertex_count; ++i) {
        matrix_to_initialize[i] = new int[vertex_count];
        for (int j = 0; j < vertex_count; j++) {
            matrix_to_initialize[i][j] = init_value;
        }
    }
    return matrix_to_initialize;
}

void Graph::deallocate_memory() {
    for (int i = 0; i < vertex_count; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void Graph::check_distances(int src) {
    int dist[vertex_count];
    bool visited[vertex_count];
    for (int i=0; i < vertex_count; i++) {
        dist[i]=INT_MAX;
        visited[i] = false;
    }
    dist[src] = 0;
    for (int j=1; j < vertex_count; j++) {
        int u = minimum_distance(dist, visited);
        visited[u] = true;
        for (int v = 0; v < vertex_count; v++) {
            if(!visited[v] && matrix[u][v] && dist[u]+matrix[u][v] < dist[v]) {
                dist[v] = dist[u]+matrix[u][v];
            }
        }
    }
    printf("Shortest paths:\n");
    for (int i=0; i < vertex_count; i++) {
        if (i != src) {
            printf("Source: %d \t Destination: %d \t Minimum cost is: %d\n",
                   src+1, i+1, dist[i]);
        }
    }
    
}

int Graph::minimum_distance(const int *dist, const bool *visited) {
    int min = INT_MAX;
    int index;
    for (int v = 0; v < vertex_count; v++) {
        if (!visited[v] && dist[v] <= min) {
            min = dist[v];
            index = v;
        }
    }
    return index;
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
        graph.check_distances(0);
        graph.deallocate_memory();
        if (case_num != number_of_cases - 1) {
            printf("\n");
        }
    }
    return 0;
}