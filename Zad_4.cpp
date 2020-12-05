#include <ctime>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

class Graph {

public:
    /// Constructor
    Graph();

    /// Shows graph's distance_matrix
    void load_graph_matrix_from_stdin();

    /// Deallocates memory
    void deallocate_memory();

    /// Checks for shortest path from source node to any other node
    void check_shortest_path(int src_node);

private:
    /// Matrix with distances
    int **distance_matrix;
    /// Parents matrix
    int **parents_matrix;
    /// How many vertexes there are
    int vertex_count;
    /// How many edges there are
    int number_of_edges;

    /// Add edge to the matrix
    void add_edge(int vertex_u, int vertex_v, int weight);

    /// Process
    bool floyd_warshall_algorithm();

    /// Prints path between source node and target selected during first run
    void print_path(int source_vertex, int target_vertex, bool first_run);
};


void Graph::add_edge(int vertex_u, int vertex_v, int weight) {
    distance_matrix[vertex_u][vertex_v] = weight;
    parents_matrix[vertex_u][vertex_v] = vertex_u;
}

void Graph::load_graph_matrix_from_stdin() {
    std::string line;
    std::vector<std::vector<int>> input_data;
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

void Graph::deallocate_memory() {
    for (int i = 0; i < vertex_count; ++i) {
        delete[] distance_matrix[i];
        delete[] parents_matrix[i];
    }
    delete[] distance_matrix;
    delete[] parents_matrix;
}

bool Graph::floyd_warshall_algorithm() {
    int combined_weight;

    for (int k = 0; k < vertex_count; k++) {
        for (int i = 0; i < vertex_count; i++) {
            for (int j = 0; j < vertex_count; j++) {
                if ((distance_matrix[i][k] == INT_MAX) || (distance_matrix[k][j] == INT_MAX)) {
                    continue;
                }
                combined_weight = distance_matrix[i][k] + distance_matrix[k][j];
                if (distance_matrix[i][j] > combined_weight) {
                    distance_matrix[i][j] = combined_weight;
                    parents_matrix[i][j] = parents_matrix[k][j];
                }
            }
        }
    }
    for (int i = 0; i < vertex_count; i++) {
        if (distance_matrix[i][i] < 0) {
            return false;
        }
    }
    return true;
}

void Graph::print_path(int source_vertex, int target_vertex, bool first_run) {
    if (source_vertex == target_vertex) {
        printf("%d-", source_vertex + 1);
    } else if (parents_matrix[source_vertex][target_vertex] == -1) {
        printf("NIE ISTNIEJE DROGA Z ");
    } else {
        print_path(source_vertex, parents_matrix[source_vertex][target_vertex], false);
        if (first_run) {
            printf("%d", target_vertex + 1);
        } else {
            printf("%d-", target_vertex + 1);
        }
    }
}

void Graph::check_shortest_path(int src_node) {
    if (floyd_warshall_algorithm()) {
        for (int j = 1; j < vertex_count; j++) {
            print_path(src_node, j, true);
            if (distance_matrix[src_node][j] < INT_MAX) {
                printf(" %d\n", distance_matrix[src_node][j]);
            } else {
                printf("%d DO %d\n", src_node+1, j+1);
            }
        }
    }
}


Graph::Graph() {
    std::string line;
    std::getline(std::cin, line);
    int space_index = line.find(' ');
    vertex_count = std::stoi(line.substr(0, space_index));
    number_of_edges = std::stoi(line.substr(space_index + 1, line.size()));
    distance_matrix = new int *[vertex_count];
    parents_matrix = new int *[vertex_count];
    for (int i = 0; i < vertex_count; i++) {
        distance_matrix[i] = new int[vertex_count];
        parents_matrix[i] = new int[vertex_count];
        for (int j = 0; j < vertex_count; j++) {
            distance_matrix[i][j] = INT_MAX;
            parents_matrix[i][j] = -1;
        }
        distance_matrix[i][i] = 0;
    }
}

int main() {
    int number_of_cases;
    std::string line;
    std::getline(std::cin, line);
    number_of_cases = std::stoi(line);
    for (int case_num = 1; case_num <= number_of_cases; case_num++) {
        Graph graph = Graph();
        printf("Graf nr %d\n", case_num);
        graph.load_graph_matrix_from_stdin();

        graph.check_shortest_path(0);
        graph.deallocate_memory();
        if (case_num != number_of_cases - 1) {
            printf("\n");
        }
    }
    return 0;
}