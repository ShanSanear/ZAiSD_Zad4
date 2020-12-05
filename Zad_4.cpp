#include <ctime>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>

const bool DEBUG = true;

#define noop() ({;});

class Graph {

public:
    /// Constructor
    Graph();

    /// Shows graph's distance_matrix
    void show_graph_matrix();

    void load_graph_matrix_from_stdin();


    void deallocate_memory();

    void check_distances(int src_node);

private:
    /// Matrix to be searched
    int **distance_matrix;
    /// Parents matrix
    int **parents_matrix;
    /// How many edges has already been loaded
    int vertex_count;
    int number_of_edges;

    void add_edge(int vertex_u, int vertex_v, int weight);

    void print_paths_to_other_nodes(int dist[], int parent[]);

    void print_target_paths(int *parent, int j);

    int minDistance(const int *dist, const bool *sptSet);

    bool FloydWarshall();

    void FWPath(int i, int j);
};


void Graph::add_edge(int vertex_u, int vertex_v, int weight) {
    distance_matrix[vertex_u][vertex_v] = weight;
    parents_matrix[vertex_u][vertex_v] = vertex_u;
    if (DEBUG) {
        printf("Adding edge: %d -> %d : %d\n", vertex_u, vertex_v, weight);
    }
}

void Graph::show_graph_matrix() {
    int i, j;
    printf("DISTANCE GRAPH:\n");
    for (i = 0; i < vertex_count; i++) {
        for (j = 0; j < vertex_count; j++) {
            printf("%*d ", 2, distance_matrix[i][j] == INT_MAX ? -1 : distance_matrix[i][j]);
        }
        printf("\n");
    }
    printf("PARENTS GRAPH:\n");
    for (i = 0; i < vertex_count; i++) {
        for (j = 0; j < vertex_count; j++) {
            printf("%*d ", 2, parents_matrix[i][j]);
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

void Graph::deallocate_memory() {
    for (int i = 0; i < vertex_count; ++i) {
        delete[] distance_matrix[i];
    }
    delete[] distance_matrix;
}

bool Graph::FloydWarshall() {
    int i, j, k, w;

    for (k = 0; k < vertex_count; k++) {
        for (i = 0; i < vertex_count; i++) {
            for (j = 0; j < vertex_count; j++) {
                if ((distance_matrix[i][k] == INT_MAX) || (distance_matrix[k][j] == INT_MAX)) {
                    continue;
                }
                w = distance_matrix[i][k] + distance_matrix[k][j];
                if (distance_matrix[i][j] > w) {
                    distance_matrix[i][j] = w;
                    parents_matrix[i][j] = parents_matrix[k][j];
                }
            }
        }
    }
    for (i = 0; i < vertex_count; i++) {
        if (distance_matrix[i][i] < 0) {
            return false;
        } // Ujemny cykl
    }
    return true;
}

// Rekurencyjna procedura odtwarzania minimalnej
// ścieżki z macierzy poprzedników p
//----------------------------------------------
void Graph::FWPath(int i, int j) {
    if (i == j) {
        printf("%d==%d\n", i, j);
        printf("%d ", i);
    } else if (parents_matrix[i][j] == -1) {
        printf("NO PATH");
    } else {
        FWPath(i, parents_matrix[i][j]);
        printf(" ");
    }
}

void Graph::check_distances(int src_node) {
    if (FloydWarshall()) {
        for (int i = 0; i < vertex_count; i++) {
            for (int j = 0; j < vertex_count; j++) {
                printf("%d-%d: ", i, j);
                FWPath(i, j);
                if (distance_matrix[i][j] < INT_MAX) {
                    printf("$%d", distance_matrix[i][j]);
                }
                printf("\n");
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
    for (int case_num = 0; case_num < number_of_cases; case_num++) {
        Graph graph = Graph();
        graph.load_graph_matrix_from_stdin();

        graph.check_distances(0);
        if (DEBUG) {
            graph.show_graph_matrix();
        }
        graph.deallocate_memory();
        if (case_num != number_of_cases - 1) {
            printf("\n");
        }
    }
    return 0;
}