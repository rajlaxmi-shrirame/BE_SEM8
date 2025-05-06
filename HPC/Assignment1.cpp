#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>

using namespace std;

class Graph {
    int V;
    vector<vector<int>> adj;

public:
    Graph(int V) {
        this->V = V;
        adj.resize(V);
    }

    void addEdge(int u, int v) {
        adj[u].push_back(v);
        adj[v].push_back(u); // undirected
    }

    // ---------- Sequential BFS ----------
    void sequentialBFS(int source) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[source] = true;
        q.push(source);

        while (!q.empty()) {
            int u = q.front();
            q.pop();
            cout << u << " ";

            for (int v : adj[u]) {
                if (!visited[v]) {
                    visited[v] = true;
                    q.push(v);
                }
            }
        }
    }

    // ---------- Parallel BFS ----------
    void parallelBFS(int source) {
        vector<bool> visited(V, false);
        queue<int> q;
        visited[source] = true;
        q.push(source);

        while (!q.empty()) {
            vector<int> currentLevel;

            // Dequeue current level
            for (int i = 0; i < q.size(); ++i) {
                int u = q.front();
                q.pop();
                cout << u << " ";
                currentLevel.push_back(u);
            }

#pragma omp parallel for schedule(dynamic)
            for (int i = 0; i < currentLevel.size(); ++i) {
                int u = currentLevel[i];
                for (int v : adj[u]) {
                    if (!visited[v]) {
#pragma omp critical
                        {
                            if (!visited[v]) {
                                visited[v] = true;
                                q.push(v);
                            }
                        }
                    }
                }
            } 
        }
    }

    // ---------- Sequential DFS ----------
    void sequentialDFSUtil(int u, vector<bool>& visited) {
        visited[u] = true;
        cout << u << " ";
        for (int v : adj[u]) {
            if (!visited[v]) {
                sequentialDFSUtil(v, visited);
            }
        }
    }

    void sequentialDFS(int source) {
        vector<bool> visited(V, false);
        sequentialDFSUtil(source, visited);
    }


    void parallelDFSUtil(int u, vector<bool>& visited) {
        bool alreadyVisited;
    
        // Check visited status in critical section
    #pragma omp critical
        {
            alreadyVisited = visited[u];
            if (!visited[u]) {
                visited[u] = true;
                cout << u << " ";
            }
        }
    
        if (alreadyVisited) return;
    
    #pragma omp parallel for schedule(dynamic)
        for (int i = 0; i < adj[u].size(); ++i) {
            int v = adj[u][i];
            bool notVisited;
    #pragma omp critical
            {
                notVisited = !visited[v];
            }
    
            if (notVisited) {
    #pragma omp task
                parallelDFSUtil(v, visited);
            }
        }
    #pragma omp taskwait
    }
    
    void parallelDFS(int source) {
        vector<bool> visited(V, false);
#pragma omp parallel
        {
#pragma omp single
            {
                parallelDFSUtil(source, visited);
            }
        }
    }
};

int main() {
    Graph g(9);
    g.addEdge(0, 1);
    g.addEdge(0, 2);
    g.addEdge(0, 3);
    g.addEdge(1, 4);
    g.addEdge(1, 5);
    g.addEdge(3, 6);
    g.addEdge(4, 7);
    g.addEdge(6, 7);
    g.addEdge(7, 8);

    double start, end;

    cout << "Sequential BFS: ";
    start = omp_get_wtime();
    g.sequentialBFS(0);
    end = omp_get_wtime();
    cout << "\nTime: " << end - start << " seconds\n\n";

    cout << "Parallel BFS: ";
    start = omp_get_wtime();
    g.parallelBFS(0);
    end = omp_get_wtime();
    cout << "\nTime: " << end - start << " seconds\n\n";

    cout << "Sequential DFS: ";
    start = omp_get_wtime();
    g.sequentialDFS(0);
    end = omp_get_wtime();
    cout << "\nTime: " << end - start << " seconds\n\n";

    cout << "Parallel DFS: ";
    start = omp_get_wtime();
    g.parallelDFS(0);
    end = omp_get_wtime();
    cout << "\nTime: " << end - start << " seconds\n\n";

    return 0;
}
