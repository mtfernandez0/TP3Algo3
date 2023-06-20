#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <list>
#include <limits>
#include <chrono>
#include <fstream>
#include <functional>

using namespace std;

#define INF numeric_limits<int>::max()
#define INFDOUBLE numeric_limits<double>::max()

list<tuple<int, int, int>> kedges;

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

void dijkstraCuadrado(  int from, vector<long long>& distance, vector<bool>& processed, 
                        vector<list<tuple<int, int>>>& edges){
    int toProcess = processed.size() - 1;
    distance[from] = 0;

    while (toProcess > 0){
        int a = 1;

        for (int i = 2; i < distance.size(); i++) 
            if (distance[a] > distance[i] && !processed[i]) a = i;
        
        processed[a] = true; toProcess--;
        for (auto u : edges[a]){
            int b, w; tie(b, w) = u;

            if (distance[a] + w < distance[b]) distance[b] = distance[a] + w;
        }
    }
}

double measure(const function<void()> &f){
    auto start = chrono::high_resolution_clock::now();
    f();
    auto stop = chrono::high_resolution_clock::now();
    chrono::duration<double> diff = stop - start;
    return diff.count();
}

void findBetterPath(vector<long long>& distanceFromS, vector<bool>& processedS, 
                    vector<long long>& distanceFromT, vector<bool>& processedT, 
                    int k, int t){
    long long res = distanceFromS[t];
    bool wasReached = processedS[t];
    int di, ci, li;
    for (auto edge : kedges){
        tie(di, ci, li) = edge;
        long long dist = distanceFromS[di] + li + distanceFromT[ci];
        res = min(res, dist);
        dist = distanceFromS[ci] + li + distanceFromT[di];
        res = min(res, dist);
        wasReached |= (processedS[ci] && processedT[di]) || (processedS[di] && processedT[ci]);
    }
    if (!wasReached) res = -1;
}

int main(){
    ofstream output_file; output_file.open("outputs/densos_dijkstra_cua.csv");
    output_file << "n,time\n";
    
    int start = 1 << 6;
    int limit = 1 << 11;

    for (int i = start; i <= limit; i *= 2) {
        int cases, n, m, k, s, t;
        string file_name = "inputs/input_densos_" + to_string(i);

        cout << file_name << endl;

        ifstream read_file(file_name);
        read_file >> cases >> n >> m >> k >> s >> t;

        double minimo = INFDOUBLE;
        int repeat = 10;

        vector<long long> distanceFromS(n + 1, INF);
        vector<long long> distanceFromT(n + 1, INF);
        vector<bool> processedS(n + 1, false);
        vector<bool> processedT(n + 1, false);
        vector<list<tuple<int, int>>> adj(n + 1);
        vector<list<tuple<int, int>>> adjInv(n + 1);
        kedges = list<tuple<int, int, int>>();

        int di, ci, li;
        for (int j = 0; j < m; j++){
            read_file >> di >> ci >> li;
            adj[di].push_back({ci, li});
            adjInv[ci].push_back({di, li});
        }

        for (int j = 0; j < k; j++){
            read_file >> di >> ci >> li;
            kedges.push_back({di, ci, li});
        }

        read_file.close();

        while (repeat > 0){
            double time = measure([&s, &distanceFromS, &processedS, &adj]() {
                dijkstraCuadrado(s, distanceFromS, processedS, adj);
            });

            time += measure([&t, &distanceFromT, &processedT, &adjInv]() {
                dijkstraCuadrado(t, distanceFromT, processedT, adjInv);
            });

            time += measure([&distanceFromS, &processedS, &distanceFromT, &processedT, &k, &t]() {
                findBetterPath(distanceFromS, processedS, distanceFromT, processedT, k, t);
            });

            minimo = min(minimo, time);

            repeat--;
        }
        output_file << n << "," << minimo << endl;
    }
    output_file.close();
}