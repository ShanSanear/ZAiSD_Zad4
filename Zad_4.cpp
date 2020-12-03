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


    void check_distances(int src_node);

private:
    /// Matrix to be searched
    int **matrix;
    /// How many edges has already been loaded
    int vertex_count;
    int number_of_edges;

    void add_edge(int vertex_u, int vertex_v, int weight);

    void initialize_matrices();

    int **initialize_matrix(int init_value) const;

    int find_node_with_minimum_distance(const int distance[], const bool p_boolean[]) const;
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

void Graph::check_distances(int src_node) {
    int dist[vertex_count];
    bool visited[vertex_count];
    for (int i=0; i < vertex_count; i++) {
        dist[i]=INT_MAX;
        visited[i] = false;
    }
    dist[src_node] = 0;

    for (int ending_vertex=0; ending_vertex < vertex_count; ending_vertex++) {
        std::vector<int> search_path;
        search_path.push_back(src_node);
        int min_distance_node = find_node_with_minimum_distance(dist, visited);
        visited[min_distance_node] = true;
        for (int target_node = 0; target_node < vertex_count; target_node++) {
            if(!visited[target_node] && matrix[min_distance_node][target_node]
               && dist[min_distance_node] + matrix[min_distance_node][target_node] < dist[target_node]) {
                dist[target_node] = dist[min_distance_node] + matrix[min_distance_node][target_node];
                search_path.push_back(min_distance_node);
            }
        }
        printf("Path for %d -> %d\n", src_node + 1, ending_vertex + 1);
        for (int p : search_path) {
            printf("%*d\t", 2, p+1);
        }
        printf("\n");
    }
    printf("Shortest paths:\n");
    for (int i=0; i < vertex_count; i++) {
        if (i != src_node) {
            printf("Source: %d \t Destination: %d \t Minimum cost is: %d\n",
                   src_node + 1, i + 1, dist[i]);
        }
    }
    
}

int Graph::find_node_with_minimum_distance(const int *distance, const bool *visited) const {
    int min = INT_MAX;
    int index;
    for (int v = 0; v < vertex_count; v++) {
        if (!visited[v] && distance[v] <= min) {
            min = distance[v];
            index = v;
        }
    }
    printf("Index: %d\n", index+1);
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