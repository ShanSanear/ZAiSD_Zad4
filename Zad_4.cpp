#include <ctime>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

/// Offset due to starting counting from 0, but both input and output should start from 1
const int NUMBERING_OFFSET = 1;

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

    /// Add edge to the distance matrix with its weight
    void add_edge(int vertex_u, int vertex_v, int weight);

    /// Algorithm used for this task
    void floyd_warshall_algorithm();

    /// Prints path between source node and target selected during first run
    void print_path(int source_vertex, int target_vertex, bool first_run);

    /// Prints shortest paths to all the other nodes from source one
    void print_shortest_paths(int src_node);
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
        add_edge(row[0] - NUMBERING_OFFSET, row[1] - NUMBERING_OFFSET, row[2]);
    }
}

void Graph::floyd_warshall_algorithm() {
    int combined_weight;

    for (int iteration = 0; iteration < vertex_count; iteration++) {
        for (int vertex_u = 0; vertex_u < vertex_count; vertex_u++) {
            for (int vertex_v = 0; vertex_v < vertex_count; vertex_v++) {
                if ((distance_matrix[vertex_u][iteration] == INT_MAX) ||
                    (distance_matrix[iteration][vertex_v] == INT_MAX)) {
                    continue;
                }
                combined_weight = distance_matrix[vertex_u][iteration] + distance_matrix[iteration][vertex_v];
                if (distance_matrix[vertex_u][vertex_v] > combined_weight) {
                    distance_matrix[vertex_u][vertex_v] = combined_weight;
                    parents_matrix[vertex_u][vertex_v] = parents_matrix[iteration][vertex_v];
                }
            }
        }
    }
}

void Graph::print_path(int source_vertex, int target_vertex, bool first_run) {
    if (source_vertex == target_vertex) {
        printf("%d-", source_vertex + NUMBERING_OFFSET);
    } else if (parents_matrix[source_vertex][target_vertex] == -1) {
        printf("NIE ISTNIEJE DROGA Z ");
    } else {
        print_path(source_vertex, parents_matrix[source_vertex][target_vertex], false);
        if (first_run) {
            printf("%d", target_vertex + NUMBERING_OFFSET);
        } else {
            printf("%d-", target_vertex + NUMBERING_OFFSET);
        }
    }
}

void Graph::check_shortest_path(int src_node) {
    floyd_warshall_algorithm();
    print_shortest_paths(src_node);
}

void Graph::print_shortest_paths(int src_node) {
    for (int j = 0; j < vertex_count; j++) {
        if (j == src_node) {
            continue;
        }
        print_path(src_node, j, true);
        if (distance_matrix[src_node][j] < INT_MAX) {
            printf(" %d\n", distance_matrix[src_node][j]);
        } else {
            printf("%d DO %d\n", src_node + NUMBERING_OFFSET, j + NUMBERING_OFFSET);
        }
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