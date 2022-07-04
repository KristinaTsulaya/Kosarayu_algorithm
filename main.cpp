#include <iostream>
#include <vector>


class Graph {
private:
    struct Edge {
        int to;
    };
    std::vector<std::vector<Edge>> adj;
    size_t vertex_number;
    std::vector<std::string> colors;

public:

    Graph(size_t vertex_number) : vertex_number(vertex_number) {
        adj.resize(vertex_number, std::vector<Edge>());
        colors.resize(vertex_number, "white");
    }

    void add_vertex(int from, int to) {
        adj[from].push_back({to});
    }

    void dfs_sort(int vertex, std::vector<int>& vector) {
        vector.push_back(vertex); /// adding
        colors[vertex] = "gray";
        for (const auto& nei_edge : adj[vertex]) {
            if (colors[nei_edge.to] == "white") {
                dfs_sort(nei_edge.to, vector);
            }
        }
        colors[vertex] = "black";
    }

    std::vector<int> top_sort() {
        std::vector<int> vector;
        vector.reserve(vertex_number);
        dfs_sort(0, vector);
        return vector;
    }

    Graph get_transposed() const {
        Graph tr_graph(vertex_number);
        for (size_t i = 0; i < vertex_number; ++i) {
            for (size_t j = 0; j < adj[i].size(); ++j) {
                tr_graph.add_vertex(adj[i][j].to, i);
            }
        }
        return tr_graph;
    }

    void dfs_div(int vertex, std::vector<int>& vector_scc) {
        vector_scc.push_back(vertex);

        colors[vertex] = "gray";
        for (auto nei : adj[vertex]) {
            if (colors[nei.to] == "white") {
                dfs_div(nei.to, vector_scc);
            }
        }
        colors[vertex] = "black";
    }

    std::vector<std::vector<int>> SCC_division() {
        std::vector<std::vector<int>> result;
        int size = 0;

        std::vector<int> vector_order = top_sort();
        Graph tr_graph = get_transposed();

        for (auto vertex : vector_order) {
            if (tr_graph.colors[vertex] == "white") {
                result.push_back(std::vector<int>());
                tr_graph.dfs_div(vertex, result[size++]);
            }
        }

        result.shrink_to_fit();
        return result;
    }

};



int main() {
    int vertex_number, edge_number;
    std::cin >> vertex_number >> edge_number;
    Graph graph(vertex_number);
    for (size_t i = 0; i < edge_number; ++i) {
        int from, to;
        std::cin >> from >> to;
        graph.add_vertex(from, to);
    }

    for (const auto& vector_scc : graph.SCC_division()) {
        for (auto vertex : vector_scc) {
            std::cout << vertex << ' ';
        }
        std::cout << std::endl;
    }

    return 0;
}
