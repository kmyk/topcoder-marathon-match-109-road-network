#include <algorithm>
#include <cassert>
#include <climits>
#include <cstdlib>
#include <iostream>
#include <iterator>
#include <map>
#include <numeric>
#include <queue>
#include <set>
#include <sstream>
#include <string>
#include <vector>
#define REP(i, n) for (int i = 0; (i) < (int)(n); ++ (i))
#define REP3(i, m, n) for (int i = (m); (i) < (int)(n); ++ (i))
#define REP_R(i, n) for (int i = int(n) - 1; (i) >= 0; -- (i))
#define REP3R(i, m, n) for (int i = int(n) - 1; (i) >= (int)(m); -- (i))
#define ALL(x) begin(x), end(x)
using ll = long long;
using namespace std;
template <class T> using reversed_priority_queue = priority_queue<T, vector<T>, greater<T> >;


struct union_find_tree {
    vector<int> data;
    union_find_tree() = default;
    explicit union_find_tree(size_t n) : data(n, -1) {}
    bool is_root(int i) { return data[i] < 0; }
    int find_root(int i) { return is_root(i) ? i : (data[i] = find_root(data[i])); }
    int tree_size(int i) { return - data[find_root(i)]; }
    int unite_trees(int i, int j) {
        i = find_root(i); j = find_root(j);
        if (i != j) {
            if (tree_size(i) < tree_size(j)) swap(i,j);
            data[i] += data[j];
            data[j] = i;
        }
        return i;
    }
    bool is_same(int i, int j) { return find_root(i) == find_root(j); }
};


struct connection_t {
    int a, b;
    ll m, p;
};

struct route_t {
    int a, b;
    ll p;
};

vector<bool> find_solution(ll NM, int N, int E, vector<connection_t> const & edges, int R, vector<route_t> const & routes) {
    vector<vector<int> > edges_of(N);
    REP (i, E) {
        edges_of[edges[i].a].push_back(i);
        edges_of[edges[i].b].push_back(i);
    }

    vector<bool> answer(E);
    ll sum_m = 0;
    union_find_tree uft(N);

    while (true) {
        vector<int> selected_path;
        ll selected_sum_m = LLONG_MAX;

        for (auto const & route : routes) if (not uft.is_same(route.a, route.b)) {
            // Dijkstra
            vector<int> parent(N, -1);
            vector<ll> sum_m(N, LLONG_MAX);
            reversed_priority_queue<pair<ll, int> > que;
            sum_m[route.a] = 0;
            que.emplace(0, route.a);
            while (not que.empty()) {
                ll cost_a; int a; tie(cost_a, a) = que.top();
                que.pop();
                if (a == route.b) break;
                if (sum_m[a] < cost_a) continue;
                for (int i : edges_of[a]) {
                    int b = (a ^ edges[i].a ^ edges[i].b);
                    ll cost_b = sum_m[a] + (uft.is_same(a, b) ? 0 : edges[i].m);
                    if (cost_b < sum_m[b]) {
                        sum_m[b] = cost_b;
                        parent[b] = a;
                        que.emplace(cost_b, b);
                    }
                }
            }
            assert (sum_m[route.b] != LLONG_MAX);

            // reconstruct path
            vector<int> path;
            path.push_back(route.b);
            while (parent[path.back()] != -1) {
                path.push_back(parent[path.back()]);
            }
            reverse(ALL(path));
            assert (path.front() == route.a);
            assert (path.back() == route.b);

            if (sum_m[route.b] < selected_sum_m) {
                selected_sum_m = sum_m[route.b];
                selected_path = path;
            }
        }

        if (not selected_path.empty() and sum_m + selected_sum_m <= NM) {
            cerr << "use path:";
            sum_m += selected_sum_m;
            assert (not uft.is_same(selected_path.front(), selected_path.back()));
            REP (i, selected_path.size() - 1) {
                int a = selected_path[i];
                int b = selected_path[i + 1];
                int j = 0;
                while (minmax({ a, b }) != minmax({ edges[edges_of[a][j]].a, edges[edges_of[a][j]].b })) ++ j;
                cerr << " " << edges_of[a][j];
                if (not uft.is_same(a, b)) {
                    answer[edges_of[a][j]] = true;
                    uft.unite_trees(a, b);
                }
            }
            cerr << endl;
            assert (uft.is_same(selected_path.front(), selected_path.back()));
        } else {
            break;
        }
    }
    return answer;
}


class RoadNetwork {
public:
    vector<int> findSolution(int NM, int N, int E, vector<string> edges, int R, vector<string> routes) {
        assert (30 <= N and N <= 1000);
        assert (N - 1 <= E);
        assert (5 <= R and R <= N / 4);

        vector<connection_t> parsed_edges(E);
        REP (i, E) {
            istringstream iss(edges[i]);
            auto & edge = parsed_edges[i];
            iss >> edge.a >> edge.b >> edge.m >> edge.p;
            assert (0 <= edge.a and edge.a < N);
            assert (0 <= edge.b and edge.b < N);
            assert (1 <= edge.m);
            assert (1 <= edge.p);
        }

        vector<route_t> parsed_routes(R);
        REP (i, R) {
            istringstream iss(routes[i]);
            auto & route = parsed_routes[i];
            iss >> route.a >> route.b >> route.p;
            assert (0 <= route.a and route.a < N);
            assert (0 <= route.b and route.b < N);
            assert (1 <= route.p);
        }

        vector<bool> used = find_solution(NM, N, E, parsed_edges, R, parsed_routes);

        vector<int> answer;
        REP (i, E) {
            if (used[i]) {
                answer.push_back(i);
            }
        }
        return answer;
    }
};
