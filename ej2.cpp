#include <iostream>
#include <vector>
#include <queue>
#include <climits>

using namespace std;

vector<vector<int>> capacity;
vector<vector<int>> adj;

int bfs(int s, int t, vector<int>& parent) {
    fill(parent.begin(), parent.end(), -1);
    parent[s] = -2;
    queue<pair<int, int>> q;
    q.push({s, INT_MAX});

    while (!q.empty()) {
        int cur = q.front().first;
        int flow = q.front().second;
        q.pop();

        for (int next : adj[cur]) {
            if (parent[next] == -1 && capacity[cur][next] > 0) {
                parent[next] = cur;
                int new_flow = min(flow, capacity[cur][next]);
                if (next == t)
                    return new_flow;
                q.push({next, new_flow});
            }
        }
    }

    return 0;
}

int maxFlow(int s, int t, int n) {
    int flow = 0;
    vector<int> parent(n);

    int new_flow;
    while (new_flow = bfs(s, t, parent)) {
        flow += new_flow;
        int cur = t;
        while (cur != s) {
            int prev = parent[cur];
            capacity[prev][cur] -= new_flow;
            capacity[cur][prev] += new_flow;
            cur = prev;
        }
    }

    return flow;
}

int main() {
    while (true) {
        int n, m,pref,u,v;
        scanf("%d %d", &n, &m);

        if (n == 0 && m == 0)
            break;

        capacity.assign(n + 2, vector<int>(n + 2, 0));
        adj.assign(n + 2, vector<int>());

        for (int i = 0; i < n; i++) {
            scanf("%d", &pref);
            adj[n].push_back(i);
            adj[i].push_back(n+1);
            capacity[n][i] = pref;
            capacity[i][n+1] = 1-pref;
        }

        for (int i = 0; i < m; i++) {
            scanf("%d %d", &u, &v);
            adj[u - 1].push_back(v - 1);
            adj[v - 1].push_back(u - 1);
            capacity[u - 1][v - 1] = 1;
            capacity[v - 1][u - 1] = 1;
        }

        int minDissatisfaction = maxFlow(n,n+1,n+2);
        printf("%d\n", minDissatisfaction);
    }

    return 0;
}
