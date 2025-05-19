#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

bool is_valid(const int v, const int x) {
    if (x < 0 || x >= v) return false;
    return true;
}

struct Edge {
    int to;
    int weight;

    explicit Edge(const int to, const int weight = 1) : to(to), weight(weight) {}
};

class List {
    int vertex;
    vector<vector<Edge>> graph;

public:
    explicit List(const int v) : vertex(v), graph(v) {};

    ~List() = default;

    void add_edge(const int m, const int n, const bool directed = false, const int weight = 1) {
        if (!is_valid(vertex, m) || !is_valid(vertex, n)) {
            cout << "Error: Vertex is out of range." << endl;
            return;
        }
        graph[m].emplace_back(n, weight);
        if (!directed) graph[n].emplace_back(m, weight);
    }

    int get_vertex() const {
        return vertex;
    }

    void print() const {
        for (int i = 0; i < vertex; i++) {
            cout << i << ": ";
            bool first = true;
            for (const Edge& edge : graph[i]) {
                if (!first) cout << ", ";
                cout << edge.to;
                if (edge.weight != 1) {
                    cout << "(w:" << edge.weight << ")";
                }
                first = false;
            }
            cout << endl;
        }
    }

    void DFS(const int start) const {
        if (!is_valid(vertex, start)) {
            cout << "Error: Start vertex is out of range." << endl;
            return;
        }
        vector<bool> visited(vertex, false);
        function<void(int)> dfs_;
        dfs_ = [&, this] (int v) {
            visited[v] = true;
            cout << v << " ";
            for (const Edge& edge : graph[v]) {
                if (!visited[edge.to]) {
                    dfs_(edge.to);
                }
            }
        };
        dfs_(start);
        cout << endl;
    }

    void BFS(const int start) const {
        if (!is_valid(vertex, start)) {
            cout << "Error: Start vertex is out of range." << endl;
            return;
        }
        vector<bool> visited(vertex, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        cout << start << " ";
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            for (const Edge& edge : graph[current]) {
                if (!visited[edge.to]) {
                    visited[edge.to] = true;
                    q.push(edge.to);
                    cout << edge.to << " ";
                }
            }
        }
        cout << endl;
    }

    void search(const int start) const {
        if (!is_valid(vertex, start)) {
            cout << "Error: Start vertex is out of range." << endl;
            return;
        }
        cout << "Which way?" << endl;
        cout << "1. DFS" << endl;
        cout << "2. BFS" << endl;
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:
                DFS(start);
                break;
            case 2:
                BFS(start);
                break;
            default:
                cout << "Invalid input." << endl;
                break;
        }
    }
};

class Matrix {
    int vertex;
    vector<vector<int>> graph;

public:
    explicit Matrix(const int v) : vertex(v) {
        graph.resize(v, vector<int>(v, 0));
    }

    ~Matrix() = default;

    void add_edge(const int m, const int n, const bool directed = false, const int weight = 1) {
        if (!is_valid(vertex, m) || !is_valid(vertex, n)) {
            cout << "Error: Vertex is out of range." << endl;
            return;
        }
        graph[m][n] = weight;
        if (!directed)
            graph[n][m] = weight;
    }

    int get_vertex() const {
        return vertex;
    }

    void print() const {
        cout << "  ";
        for (int i = 0; i < vertex; i++) {
            cout << i << " ";
        }
        cout << endl;
        for (int i = 0; i < vertex; i++) {
            cout << i << " ";
            for (int j = 0; j < vertex; j++) {
                cout << graph[i][j] << " ";
            }
            cout << endl;
        }
    }

    void DFS(const int start) const {
        if (!is_valid(vertex, start)) {
            cout << "Error: Start vertex is out of range." << endl;
            return;
        }
        vector<bool> visited(vertex, false);
        function<void(int)> dfs_;
        dfs_ = [&, this] (int v) {
            visited[v] = true;
            cout << v << " ";
            for (int i = 0; i < vertex; i++) {
                if (graph[v][i] != 0 && !visited[i]) {
                    dfs_(i);
                }
            }
        };
        dfs_(start);
        cout << endl;
    }

    void BFS(const int start) const {
        if (!is_valid(vertex, start)) {
            cout << "Error: Start vertex is out of range." << endl;
            return;
        }
        vector<bool> visited(vertex, false);
        queue<int> q;
        visited[start] = true;
        q.push(start);
        cout << start << " ";
        while (!q.empty()) {
            int current = q.front();
            q.pop();
            for (int i = 0; i < vertex; i++) {
                if (graph[current][i] != 0 && !visited[i]) {
                    visited[i] = true;
                    q.push(i);
                    cout << i << " ";
                }
            }
        }
        cout << endl;
    }

    void search(const int start) const {
        if (!is_valid(vertex, start)) {
            cout << "Error: Start vertex is out of range." << endl;
            return;
        }
        cout << "Which way?" << endl;
        cout << "1. DFS" << endl;
        cout << "2. BFS" << endl;
        int choice;
        cin >> choice;
        switch (choice) {
            case 1:
                DFS(start);
                break;
            case 2:
                BFS(start);
                break;
            default:
                cout << "Invalid input." << endl;
                break;
        }
    }
};

void test_normal_case();
void test_single_vertex();
void test_disconnected_graph();
void test_directed_graph();
void test_error_cases();
void interactive_test();
void test_matrix_implementation();
void test_weighted_list();

int main() {
    test_normal_case();
    test_single_vertex();
    test_disconnected_graph();
    test_directed_graph();
    test_error_cases();
    interactive_test();
    test_matrix_implementation();
    test_weighted_list();
    return 0;
}

void test_normal_case() {
    cout << "=== Test 1: Normal Case (Adjacency List) ===" << endl;
    List graph(6);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    graph.add_edge(2, 5);
    graph.add_edge(3, 4);
    graph.print();
    cout << "DFS from vertex 0: ";
    graph.DFS(0);
    cout << "BFS from vertex 0: ";
    graph.BFS(0);
    cout << endl;
}

void test_single_vertex() {
    cout << "=== Test 2: Boundary Case (Single Vertex, No Edges) ===" << endl;
    List graph(1);
    graph.print();
    cout << "DFS from vertex 0: ";
    graph.DFS(0);
    cout << "BFS from vertex 0: ";
    graph.BFS(0);
    cout << endl;
}

void test_disconnected_graph() {
    cout << "=== Test 3: Disconnected Graph ===" << endl;
    List graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(2, 3);
    graph.print();
    cout << "Starting from vertex 0:" << endl;
    cout << "DFS: ";
    graph.DFS(0);
    cout << "BFS: ";
    graph.BFS(0);
    cout << "Starting from vertex 2:" << endl;
    cout << "DFS: ";
    graph.DFS(2);
    cout << "BFS: ";
    graph.BFS(2);
    cout << endl;
}

void test_directed_graph() {
    cout << "=== Test 4: Directed Graph ===" << endl;
    List graph(4);
    graph.add_edge(0, 1, true);
    graph.add_edge(1, 2, true);
    graph.add_edge(2, 3, true);
    graph.add_edge(3, 1, true);
    graph.print();
    cout << "DFS from vertex 0: ";
    graph.DFS(0);
    cout << "BFS from vertex 0: ";
    graph.BFS(0);
    cout << endl;
}

void test_error_cases() {
    cout << "=== Test 5: Error Handling ===" << endl;
    List graph(3);
    graph.add_edge(0, 1);
    graph.add_edge(1, 2);
    cout << "Testing adding edges with out-of-range vertices:" << endl;
    graph.add_edge(0, 5);
    graph.add_edge(-1, 1);
    cout << "Testing search starting from out-of-range vertices:" << endl;
    graph.DFS(5);
    graph.BFS(-1);
    cout << endl;
}

void interactive_test() {
    cout << "=== Interactive Test ===" << endl;
    List graph(5);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 4);
    graph.print();
    cout << "Testing search method selection functionality:" << endl;
    cout << "The program will prompt for input (1 for DFS, 2 for BFS):" << endl;
    cout << "(Skipping interactive input for automated testing)" << endl;
    cout << "You can manually test invalid input (like 3) to verify error handling" << endl;
    cout << endl;
}

void test_matrix_implementation() {
    cout << "=== Test 6: Matrix Implementation ===" << endl;
    Matrix graph(4);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(2, 3);
    cout << "Adjacency Matrix:" << endl;
    graph.print();
    cout << "DFS from vertex 0: ";
    graph.DFS(0);
    cout << "BFS from vertex 0: ";
    graph.BFS(0);
    cout << endl;
    cout << "Testing with weights:" << endl;
    Matrix weighted_graph(3);
    weighted_graph.add_edge(0, 1, false, 5);
    weighted_graph.add_edge(1, 2, false, 3);
    weighted_graph.add_edge(0, 2, false, 7);
    weighted_graph.print();
    cout << endl;
}


void test_weighted_list() {
    cout << "=== Test 7: Weighted Adjacency List ===" << endl;
    List graph(5);
    graph.add_edge(0, 1, false, 5);
    graph.add_edge(0, 2, false, 3);
    graph.add_edge(1, 3, false, 7);
    graph.add_edge(2, 3, false, 2);
    graph.add_edge(3, 4, false, 4);
    graph.add_edge(1, 4, false, 6);
    cout << "Weighted graph representation:" << endl;
    graph.print();
    cout << "DFS from vertex 0: ";
    graph.DFS(0);
    cout << "BFS from vertex 0: ";
    graph.BFS(0);
    cout << "\nDirected weighted graph:" << endl;
    List directed_graph(4);
    directed_graph.add_edge(0, 1, true, 10);
    directed_graph.add_edge(0, 2, true, 15);
    directed_graph.add_edge(1, 3, true, 20);
    directed_graph.add_edge(2, 3, true, 25);
    directed_graph.print();
    cout << "DFS from vertex 0: ";
    directed_graph.DFS(0);
    cout << "BFS from vertex 0: ";
    directed_graph.BFS(0);
    cout << endl;
}