#include<bits/stdc++.h>

using namespace std;

const int MAXV = 10002;
struct DFSTree {
    vector<int> V[MAXV];
    int v, e, visit[MAXV], level[MAXV], back[MAXV], child[MAXV];
    bool isRoot[MAXV];

    void setEdge(int u, int v) {
        V[u].push_back(v);
        V[v].push_back(u);
    }

    void makeDFSTree() {
        int count = 1;
        for(int i = 1; i <= v; i++) {
            if(!visit[i]) {
                isRoot[i] = true;
                dfs(i, 1, count);
            }
        }
    }

    int dfs(int here, int l, int& count) {
        level[here] = l;
        visit[here] = count++;

        int b = here;
        for(int next : V[here]) {
            if(!visit[next]) {
                child[here]++;
                int low = dfs(next, l + 1, count);
                if(level[low] < level[b]) b = low;
            }
            else if(level[next] < level[b]) b = next;
        }

        return back[here] = b;
    }

    vector<int> getCutVertex() {
        vector<int> ret;
        for(int i = 1; i <= v; i++)
            if(isCutVertex(i)) ret.push_back(i);

        return ret;
    }

    bool isCutVertex(int v) {
        if(isRoot[v])
            return child[v] >= 2;
        else {
            for(int next : V[v]) {
                if(level[next] < level[v]) continue;
                if(level[next] == level[v] + 1 && level[back[next]] >= level[v]) return true;
            }

            return false;
        }
    }
} T;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    cin >> T.v >> T.e;
    for(int i = 0; i < T.e; i++) {
        int u, v;
        cin >> u >> v;
        T.setEdge(u, v);
    }

    T.makeDFSTree();
    auto answer = T.getCutVertex();

    cout << answer.size() << '\n';
    for(auto ans : answer) cout << ans << ' ';
    cout << '\n';
}
