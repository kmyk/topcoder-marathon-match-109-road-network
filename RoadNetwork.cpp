#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
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

struct parameters {
    ll NM;
    int N;
    int E;
    vector<connection_t> edges;
    int R;
    vector<route_t> routes;

    vector<vector<int> > edges_of;
    vector<vector<int> > routes_of;

    parameters(ll NM_, int N_, int E_, vector<connection_t> const & edges_, int R_, vector<route_t> const & routes_)
            : NM(NM_), N(N_), E(E_), edges(edges_), R(R_), routes(routes_) {
        for (auto & edge : edges) {
            if (edge.a > edge.b) {
                swap(edge.a, edge.b);
            }
        }

        for (auto & route : routes) {
            if (route.a > route.b) {
                swap(route.a, route.b);
            }
        }

        edges_of.resize(N);
        REP (i, E) {
            edges_of[edges[i].a].push_back(i);
            edges_of[edges[i].b].push_back(i);
        }

        routes_of.resize(N);
        REP (i, R) {
            routes_of[routes[i].a].push_back(i);
            routes_of[routes[i].b].push_back(i);
        }
    }

    int find_edge(int a, int b) {
        if (a > b) swap(a, b);
        assert (not edges_of[a].empty());
        int j = edges_of[a].size() - 1;
        while (j >= 0 and (a != edges[edges_of[a][j]].a or b != edges[edges_of[a][j]].b)) -- j;
        return edges_of[a][j];
    }
};

struct solution {
    const parameters param;

    // the state of solution
    vector<bool> used;
    ll used_sum_p;
    ll used_sum_m;
    union_find_tree used_uft;

    solution(parameters const & param_)
            : param(param_) {
        auto const & N = param.N;
        auto const & E = param.E;
        used.resize(E);
        used_sum_p = 0;
        used_sum_m = 0;
        used_uft = union_find_tree(N);
    }

    void use(int i) {
        auto const & NM = param.NM;
        auto const & E = param.E;
        auto const & edges = param.edges;
        assert (0 <= i and i <= E);
        if (used[i]) return;
        assert (used_sum_m + edges[i].m <= NM);

        used[i] = true;
        used_sum_p += edges[i].p;
        used_sum_m += edges[i].m;
        used_uft.unite_trees(edges[i].a, edges[i].b);
    }

    vector<int> get_answer() const {
        auto const & E = param.E;
        vector<int> answer;
        REP (i, E) {
            if (used[i]) {
                answer.push_back(i);
            }
        }
        return answer;
    }

    tuple<int, vector<int>, vector<vector<int> > > get_compressed_graph() {
        auto const & N = param.N;
        int size = 0;
        vector<int> component_of(N, -1);
        vector<vector<int> > vertices_of;
        REP (a, N) {
            int b = used_uft.find_root(a);
            if (component_of[b] == -1) {
                component_of[b] = (size ++);
                vertices_of.emplace_back();
            }
            component_of[a] = component_of[b];
            vertices_of[component_of[a]].push_back(a);
        }
        return make_tuple(size, component_of, vertices_of);
    }
};

tuple<vector<int>, ll, double> find_nice_path_for_route(parameters const & param, route_t const & route, int size, vector<int> const & component_of, vector<vector<int> > const & vertices_of) {
    auto const & edges = param.edges;
    auto const & edges_of = param.edges_of;

    if (component_of[route.a] == component_of[route.b]) {
        return make_tuple(vector<int>(), 0, - INFINITY);
    }

    // Dijkstra
    vector<int> parent(size, -1);
    vector<int> path_edge(size, -1);
    vector<ll> sum_p(size, LLONG_MIN);
    vector<ll> sum_m(size, LLONG_MAX);
    reversed_priority_queue<pair<ll, int> > que;
    sum_p[component_of[route.a]] = 0;
    sum_m[component_of[route.a]] = 0;
    que.emplace(0, component_of[route.a]);
    while (not que.empty()) {
        ll sum_m_x; int x; tie(sum_m_x, x) = que.top();
        que.pop();
        if (x == component_of[route.b]) break;
        if (sum_m[x] < sum_m_x) continue;

        for (int a : vertices_of[x]) {
            for (int i : edges_of[a]) {
                int b = (a ^ edges[i].a ^ edges[i].b);
                int y = component_of[b];
                if (x == y) continue;
                ll sum_m_y = sum_m[x] + edges[i].m;
                if (sum_m_y < sum_m[y]) {
                    sum_m[y] = sum_m_y;
                    sum_p[y] = sum_p[x] + edges[i].p;
                    parent[y] = x;
                    path_edge[y] = i;
                    que.emplace(sum_m_y, y);
                }
            }
        }
    }
    assert (sum_m[component_of[route.b]] != LLONG_MAX);

    // reconstruct path
    vector<int> path;
    for (int x = component_of[route.b]; path_edge[x] != -1; x = parent[x]) {
        path.push_back(path_edge[x]);
    }
    reverse(ALL(path));

    double value = (double)(sum_p[component_of[route.b]] + route.p) / sum_m[component_of[route.b]];

    return make_tuple(path, sum_m[component_of[route.b]], value);
}

vector<int> find_solution(ll NM, int N, int E, vector<connection_t> const & edges, int R, vector<route_t> const & routes) {
    parameters param(NM, N, E, edges, R, routes);
    solution sln(param);

    while (true) {
        int size; vector<int> component_of; vector<vector<int> > vertices_of;
        tie(size, component_of, vertices_of) = sln.get_compressed_graph();

        vector<int> selected_path;
        double selected_value = - INFINITY;

        for (auto const & route : routes) {
            vector<int> path; ll sum_m; double value;
            tie(path, sum_m, value) = find_nice_path_for_route(param, route, size, component_of, vertices_of);

            if (sln.used_sum_m + sum_m > NM) {
                continue;
            }
            if (selected_value < value) {
                selected_value = value;
                selected_path = path;
            }
        }

        if (not selected_path.empty()) {
            cerr << "use path:";
            for (int i : selected_path) {
                cerr << " " << i;
                assert (not sln.used_uft.is_same(edges[i].a, edges[i].b));
                sln.use(i);
            }
            cerr << endl;
        } else {
            break;
        }
    }

    // use edges greedily, order by P / M
    vector<int> order(E);
    iota(ALL(order), 0);
    sort(ALL(order), [&](int i, int j) {
        return edges[i].p * edges[j].m > edges[j].p * edges[i].m;
    });
    for (int i : order) {
        if (not sln.used[i] and sln.used_sum_m + edges[i].m <= NM) {
            sln.use(i);
            cerr << "use edge: " << i << endl;
        }
    }

    return sln.get_answer();
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

        return find_solution(NM, N, E, parsed_edges, R, parsed_routes);
    }
};
