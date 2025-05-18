#include <functional>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

class List {
    int vertex;
    vector<vector<int>> graph;

public:
    List(int v) : vertex(v), graph(v) {};

    ~List() {};

    void add_edge(int m, int n, bool directed = false) {
        if (m < 0 || m >= vertex || n < 0 || n >= vertex) {
            cout << "Error: Vertex is out of range." << endl;
            return;
        }
        graph[m].push_back(n);
        if (!directed) graph[n].push_back(m);
    }

    int get_vertex() const {
        return vertex;
    }

    void print() const {
        for (int i = 0; i < vertex; i++) {
            cout << i << ": ";
            int k = 1;
            for (int j : graph[i]) {
                if (k++ != 1) cout << ", ";
                cout << j;
            }
            cout << endl;
        }
    }

    void DFS(int start) {
        if (start < 0 || start >= vertex) {
            cout << "Error: Start vertex is out of range." << endl;
            return;
        }
        vector<bool> visited(vertex, false);
        function<void(int)> dfs_;
        dfs_ = [&, this] (int v) {
            visited[v] = true;
            cout << v << " ";
            for (int neighbor : graph[v]) {
                if (!visited[neighbor]) {
                    dfs_(neighbor);
                }
            }
        };
        dfs_(start);
        cout << endl;
    }

    void BFS(int start) {
        if (start < 0 || start >= vertex) {
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
            for (int neighbor : graph[current]) {
                if (!visited[neighbor]) {
                    visited[neighbor] = true;
                    q.push(neighbor);
                    cout << neighbor << " ";
                }
            }
        }
        cout << endl;
    }

    void search(int start) {
        if (start < 0 || start >= vertex) {
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

void test_normal_case() {
    cout << "=== Test 1: Normal Case ===" << endl;
    List graph(6);
    graph.add_edge(0, 1);
    graph.add_edge(0, 2);
    graph.add_edge(1, 3);
    graph.add_edge(1, 4);
    graph.add_edge(2, 5);
    graph.add_edge(3, 4);
    graph.print();
    graph.DFS(0);
    graph.BFS(0);
    cout << endl;
}

void test_single_vertex() {
    cout << "=== Test 2: Boundary Case (Single Vertex, No Edges) ===" << endl;
    List graph(1);
    graph.print();
    graph.DFS(0);
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
    graph.DFS(0);
    graph.BFS(0);
    cout << "Starting from vertex 2:" << endl;
    graph.DFS(2);
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
    graph.DFS(0);
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
    cout << "Input 1 to test DFS:" << endl;
    graph.search(0);
    cout << "You can manually test invalid input (like 3) to verify error handling" << endl;
}

int main() {
    test_normal_case();
    test_single_vertex();
    test_disconnected_graph();
    test_directed_graph();
    test_error_cases();
    interactive_test();
    return 0;
}
