#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <list>
#include <limits>

using namespace std;

#define INF numeric_limits<int>::max()

vector<list<tuple<int, int>>> adj;
vector<list<tuple<int, int>>> adjInv;

void dijkstra(int from, vector<long long>& distance, vector<bool>& processed, vector<list<tuple<int, int>>>& edges){
    priority_queue<tuple<int, int>> q;
    distance[from] = 0;

    q.push({0, from});
    while (!q.empty()){
        int a = get<1>(q.top()); q.pop();

        if (processed[a]) continue;

        processed[a] = true;
        for (auto u : edges[a]){
            int b, w; tie(b, w) = u;

            if (distance[a] + w < distance[b]){
                distance[b] = distance[a] + w;
                q.push({-distance[b], b});
            }
        }
    }
}

int main(int argc, char const *argv[]){
    int cases, n, m, k, s, t;

    cin >> cases;

    while (cases > 0){
        cin >> n >> m >> k >> s >> t;

        vector<long long> distanceFromS(n + 1, INF);
        vector<long long> distanceFromT(n + 1, INF);
        vector<bool> processedS(n + 1, false);
        vector<bool> processedT(n + 1, false);
        adj     = vector<list<tuple<int, int>>>(n + 1);
        adjInv  = vector<list<tuple<int, int>>>(n + 1);

        int di, ci, li;
        for (int i = 0; i < m; i++){
            cin >> di >> ci >> li;
            adj[di].push_back({ci, li});
            adjInv[ci].push_back({di, li});
        }
        dijkstra(s, distanceFromS, processedS, adj);
        dijkstra(t, distanceFromT, processedT, adjInv);

        long long res = distanceFromS[t];

        bool wasReached = processedS[t];

        for (int i = 0; i < k; i++){
            cin >> di >> ci >> li;

            long long dist = distanceFromS[di] + li + distanceFromT[ci];

            res = min(res, dist);

            dist = distanceFromS[ci] + li + distanceFromT[di];

            res = min(res, dist);

            wasReached |= (processedS[ci] && processedT[di]) || (processedS[di] && processedT[ci]);
        }

        if (!wasReached) res = -1;

        cout << res << endl;
        cases--;
    }
}