#include<iostream>
#include<vector>
#include<algorithm>
#include<queue>
#define MAXSIZE 6500
#define MAX 100000000

using namespace std;

int t, n, m, total, dir[6][2] = {-1, -1, 0, -1, 0, 1, -1, 1, 1, -1, 1, 1};
char board[100][100];

struct FlowGraph {
    struct Edge {
        int idx;
        int start, end, capacity;
    };

    vector<Edge> E;
    vector<int> V[MAXSIZE];
    int source, sink, level[MAXSIZE], start[MAXSIZE];

    void setEdge(int u, int v, int c) {
        int n = E.size();

        E.push_back({n, u, v, c});
        E.push_back({n + 1, v, u, 0});

        V[u].push_back(n);
        V[v].push_back(n + 1);
    }

    void levelGraph() {
        queue<int> q;
        for(int i = 0; i < MAXSIZE; i++) level[i] = -1;

        level[source] = 0;
        q.push(source);

        while(!q.empty()) {
            int here = q.front(); q.pop();

            for(auto edge : V[here]) {
                int next = E[edge].end;
                if(level[next] == -1 && E[edge].capacity) {
                    level[next] = level[here] + 1;
                    q.push(next);
                }
            }
        }
    }

    long long dfs(int u, int minFlow = MAX) {
        if(u == sink) return minFlow;

        for(; start[u] < V[u].size(); start[u]++) {
            auto edge = E[V[u][start[u]]];
            int next = edge.end;

            if(level[next] == level[u] + 1 && edge.capacity) {
                long long f = dfs(next, min(minFlow, edge.capacity));
                if(f) {
                    E[edge.idx].capacity -= f;
                    E[edge.idx ^ 1].capacity += f;
                    return f;
                }
            }
        }

        return 0;
    }

    long long maxflow() {
        long long ret = 0;
        while(1) {
            levelGraph();
            if(level[sink] == -1) break;
            for(int i = 0; i < MAXSIZE; i++) start[i] = 0;
            while(1) {
                long long flow = dfs(source);
                if(flow == 0) break;
                ret += flow;
            }
        }

        return ret;
    }

    void clear() {
        E.clear();
        for(int i = 0; i < MAXSIZE; i++) V[i].clear();
    }
} G;

int main(){
    for(cin >> t; t > 0; t--) {
        G.clear();
        cin >> n >> m;
        total = n * m;

        G.source = 0;
        G.sink = n * m + 1;

        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j++){
                cin >> board[i][j];

                if(board[i][j] == 'x') total--;
            }


        for(int i = 0; i < n; i++)
            for(int j = 0; j < m; j += 2) {
                if(board[i][j] == 'x') continue;

                int start = i * m + j + 1;

                G.setEdge(0, start, 1);

                for(int k = 0; k < 6; k++) {
                    int nextx = i + dir[k][0];
                    int nexty = j + dir[k][1];
                    if(nextx < 0 || nexty < 0 || nextx >= n || nexty >= m || board[nextx][nexty] == 'x') continue;

                    int end = nextx * m + nexty + 1;
                    G.setEdge(start, end, 1);
                }
            }

        for(int i = 0; i < n; i++)
            for(int j = 1; j < m; j += 2)
                G.setEdge(i * m + j + 1, n * m + 1, 1);

        cout << total - G.maxflow() << '\n';
    }

    return 0;
}
