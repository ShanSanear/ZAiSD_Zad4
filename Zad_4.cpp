#include <ctime>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

const bool DEBUG = false;

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

    void print_paths_to_other_nodes(int dist[], int parent[]);

    void print_target_paths(int *parent, int j);

    int minDistance(const int *dist, const bool *sptSet);
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

int Graph::minDistance(const int dist[],
                       const bool sptSet[]) {

    // Initialize min value
    int min = INT_MAX, min_index;

    for (int v = 0; v < vertex_count; v++) {
        if (!sptSet[v] && dist[v] <= min) {
            min = dist[v], min_index = v;
        }
    }
    return min_index;
}

void Graph::deallocate_memory() {
    for (int i = 0; i < vertex_count; ++i) {
        delete[] matrix[i];
    }
    delete[] matrix;
}

void Graph::check_distances(int src_node) {
    int dist[vertex_count];
    bool sptSet[vertex_count];
    int parent[vertex_count];
    for (int i = 0; i < vertex_count; i++) {
        if (i == src_node) {
            parent[i] = -1;
        } else {
            parent[i] = INT_MAX;
        }
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
    dist[src_node] = 0;
    for (int count = 0; count < vertex_count - 1; count++) {
        int u = minDistance(dist, sptSet);

        // Mark the picked vertex
        // as processed
        sptSet[u] = true;

        for (int v = 0; v < vertex_count; v++)

            if (!sptSet[v] && matrix[u][v] &&
                dist[u] + matrix[u][v] < dist[v]) {
                parent[v] = u;
                dist[v] = dist[u] + matrix[u][v];
            }
    }
    print_paths_to_other_nodes(dist, parent);
}

Graph::Graph() {
    matrix = new int *[vertex_count];
    for (int i = 0; i < vertex_count; ++i) {
        matrix[i] = new int[vertex_count];
        for (int j = 0; j < vertex_count; j++) {
            matrix[i][j] = 0;
        }
    }
    std::string line;
    std::getline(std::cin, line);
    int space_index = line.find(' ');
    vertex_count = std::stoi(line.substr(0, space_index));
    number_of_edges = std::stoi(line.substr(space_index + 1, line.size()));
}

void Graph::print_paths_to_other_nodes(int *dist, int *parent) {
    int src = 0;
    for (int target_node = 1; target_node < vertex_count; target_node++) {
        if (parent[target_node] == INT_MAX) {
            printf("NIE ISTNIEJE DROGA Z %d DO %d\n", src+1, target_node + 1);
            continue;
        }
        printf("%d", src + 1);
        print_target_paths(parent, target_node);
        printf(" %d\n", dist[target_node]);
    }


}

void Graph::print_target_paths(int *parent, int j) {

    if (parent[j] == -1) {
        printf("Is source: %d\n", j);
        return;
    }

    print_target_paths(parent, parent[j]);

    printf("-%d", j + 1);
};


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