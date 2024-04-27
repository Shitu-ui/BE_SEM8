#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>

using namespace std;

class Solution {
public:
    vector<int> dfsOfGraph(int V, vector<int> adj[], int startNode) {
        vector<int> storeDfs;
        vector<int> vis(V + 1, 0);
        dfs(startNode, vis, adj, storeDfs);
        return storeDfs;
    }

    void dfs(int node, vector<int>& vis, vector<int> adj[], vector<int>& storeDfs) {
        storeDfs.push_back(node);
        vis[node] = 1;
        for (auto it : adj[node]) {
            if (!vis[it]) {
                dfs(it, vis, adj, storeDfs);
            }
        }
    }

    vector<int> bfsOfGraph(int V, vector<int> adj[], int startNode) {
        vector<int> bfs;
        vector<int> vis(V + 1, 0);
        queue<int> q;
        q.push(startNode);
        vis[startNode] = 1;
        while (!q.empty()) {
            int node = q.front();
            q.pop();
            bfs.push_back(node);
            for (auto it : adj[node]) {
                if (!vis[it]) {
                    q.push(it);
                    vis[it] = 1;
                }
            }
        }
        return bfs;
    }
};

void addEdge(vector<int> adj[], int u, int v) {
    adj[u].push_back(v);
    adj[v].push_back(u);
}

int main() {
    int nodes, edges;
    cout << "Enter the number of nodes: ";
    cin >> nodes;
    cout << "Enter the number of edges: ";
    cin >> edges;

    vector<int> adj[nodes + 1];

    for (int i = 0; i < edges; i++) {
        int a, b;
        cout << "Enter node pair (a, b): ";
        cin >> a >> b;
        addEdge(adj, a, b);
    }

    int startNode;
    cout << "Enter the starting node for DFS and BFS traversals: ";
    cin >> startNode;

    Solution obj;
    vector<int> df, ans;

    int threads;
    cout << "Enter the number of threads: ";
    cin >> threads;

    double start_time;
    start_time = omp_get_wtime();
    omp_set_num_threads(threads);
#pragma omp parallel sections
    {
#pragma omp section
        {
            df = obj.dfsOfGraph(nodes, adj, startNode);
        }
#pragma omp section
        {
            ans = obj.bfsOfGraph(nodes, adj, startNode);
        }
    }
    double parallel_time = omp_get_wtime() - start_time;

    cout << "DFS Traversal: ";
    for (auto it : df) cout << it << " ";
    cout << endl;

    cout << "BFS Traversal: ";
    for (auto it : ans) cout << it << " ";
    cout << endl;

    cout << "Parallel Time: " << parallel_time << " seconds" << endl;

    cout << "DFS Sequential Time: ";
    start_time = omp_get_wtime();
    df = obj.dfsOfGraph(nodes, adj, startNode);
    double dfs_seq_time = omp_get_wtime() - start_time;
    cout << dfs_seq_time << " seconds" << endl;

    cout << "BFS Sequential Time: ";
    start_time = omp_get_wtime();
    ans = obj.bfsOfGraph(nodes, adj, startNode);
    double bfs_seq_time = omp_get_wtime() - start_time;
    cout << bfs_seq_time << " seconds" << endl;

    cout << "DFS Parallel Time: ";
    start_time = omp_get_wtime();
    df = obj.dfsOfGraph(nodes, adj, startNode);
    double dfs_par_time = omp_get_wtime() - start_time;
    cout << dfs_par_time << " seconds" << endl;

    cout << "BFS Parallel Time: ";
    start_time = omp_get_wtime();
    ans = obj.bfsOfGraph(nodes, adj, startNode);
    double bfs_par_time = omp_get_wtime() - start_time;
    cout << bfs_par_time << " seconds" << endl;

    return 0;
}


