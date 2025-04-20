#include <iostream>
#include <vector>
#include <limits>
#include <queue>
#include <fstream>
#include <sstream>
#include <iomanip>

using namespace std;

const int INF = 9999;   // Representation for “infinite” cost (no direct link)

/* -------------------------------------------------------------------------
 *  Helper: Print final routing table for a node in DVR simulation
 * ------------------------------------------------------------------------- */
void printDVRTable(int node,
                   const vector<vector<int>>& table,   // Final distance table
                   const vector<vector<int>>& nextHop) // Corresponding next‑hop matrix
{
    cout << "Node " << node << " Routing Table:\n";
    cout << "Dest\tCost\tNext Hop\n";

    for (int i = 0; i < table.size(); ++i) {
        cout << i << "\t" << table[node][i] << "\t";
        if (nextHop[node][i] == -1) cout << "-";
        else                        cout << nextHop[node][i];
        cout << '\n';
    }
    cout << '\n';
}

/* -------------------------------------------------------------------------
 *  Distance Vector Routing (Bellman‑Ford style)
 * ------------------------------------------------------------------------- */
void simulateDVR(const vector<vector<int>>& graph)
{
    int n = graph.size();
    vector<vector<int>> dist = graph;          // Local copy of cost matrix
    vector<vector<int>> nextHop(n, vector<int>(n));

    /* ------------- INITIALIZATION ------------- */
    // If there is a direct edge i‑>j, nextHop = j, otherwise −1
    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            nextHop[i][j] = (graph[i][j] != INF && i != j) ? j : -1;

    /* ------------- ITERATIVE UPDATES ------------- */
    // Standard DV update until no table changes occur
    bool updated;
    do {
        updated = false;
        for (int i = 0; i < n; ++i) {          // For every source node i
            for (int j = 0; j < n; ++j) {      // For every destination j
                for (int k = 0; k < n; ++k) {  // Consider each neighbor k of i
                    // Relaxation: if path i->k->j is cheaper, adopt it
                    if (dist[i][k] + dist[k][j] < dist[i][j]) {
                        dist[i][j]     = dist[i][k] + dist[k][j];
                        nextHop[i][j]  = nextHop[i][k];  // First hop towards k
                        updated        = true;
                    }
                }
            }
        }
    } while (updated);   // Continue iterations until convergence

    /* ------------- OUTPUT ------------- */
    cout << "--- DVR Final Tables ---\n";
    for (int i = 0; i < n; ++i)
        printDVRTable(i, dist, nextHop);
}

/* -------------------------------------------------------------------------
 *  Helper: Print routing table for a node after LSR (Dijkstra)
 * ------------------------------------------------------------------------- */
void printLSRTable(int src,
                   const vector<int>& dist,  // Final distances from src
                   const vector<int>& prev)  // Predecessor array for paths
{
    cout << "Node " << src << " Routing Table:\n";
    cout << "Dest\tCost\tNext Hop\n";

    for (int i = 0; i < dist.size(); ++i) {
        if (i == src) continue;  // Skip self entry

        cout << i << "\t" << dist[i] << "\t";

        /* Determine first hop on the shortest‑path tree */
        int hop = i;
        while (prev[hop] != src && prev[hop] != -1)
            hop = prev[hop];

        cout << (prev[hop] == -1 ? -1 : hop) << '\n';
    }
    cout << '\n';
}

/* -------------------------------------------------------------------------
 *  Link State Routing – run Dijkstra from every source node
 * ------------------------------------------------------------------------- */
void simulateLSR(const vector<vector<int>>& graph)
{
    int n = graph.size();

    for (int src = 0; src < n; ++src) {
        vector<int>  dist(n, INF);   // Shortest distance estimates
        vector<int>  prev(n, -1);    // Predecessor array
        vector<bool> visited(n, false);

        dist[src] = 0;               // Distance to self is 0

        /* ---------- Standard Dijkstra’s algorithm ---------- */
        for (int iter = 0; iter < n; ++iter) {
            int u = -1;
            // Pick the unvisited node with minimum tentative distance
            for (int j = 0; j < n; ++j)
                if (!visited[j] && (u == -1 || dist[j] < dist[u]))
                    u = j;

            if (u == -1 || dist[u] == INF) break;  // Unreachable nodes left

            visited[u] = true;

            // Relax edges from u
            for (int v = 0; v < n; ++v) {
                if (!visited[v] && graph[u][v] != INF) {
                    int alt = dist[u] + graph[u][v];
                    if (alt < dist[v]) {
                        dist[v] = alt;
                        prev[v] = u;
                    }
                }
            }
        }

        printLSRTable(src, dist, prev);
    }
}

/* -------------------------------------------------------------------------
 *  Read adjacency matrix from input file
 * ------------------------------------------------------------------------- */
vector<vector<int>> readGraphFromFile(const string& filename)
{
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << filename << '\n';
        exit(1);
    }

    int n;
    file >> n;                              // Number of nodes
    vector<vector<int>> graph(n, vector<int>(n));

    for (int i = 0; i < n; ++i)
        for (int j = 0; j < n; ++j)
            file >> graph[i][j];

    file.close();
    return graph;
}

/* -------------------------------------------------------------------------
 *  Entry point: expects one argument = input filename
 * ------------------------------------------------------------------------- */
int main(int argc, char *argv[])
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " <input_file>\n";
        return 1;
    }

    string filename = argv[1];
    vector<vector<int>> graph = readGraphFromFile(filename);

    cout << "\n--- Distance Vector Routing Simulation ---\n";
    simulateDVR(graph);

    cout << "\n--- Link State Routing Simulation ---\n";
    simulateLSR(graph);

    return 0;
}
