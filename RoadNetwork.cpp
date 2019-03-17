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
#include <random>
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


constexpr double ticks_per_sec = 2800000000;
constexpr double ticks_per_sec_inv = 1.0 / ticks_per_sec;
inline double rdtsc() {  // in seconds
    uint32_t lo, hi;
    asm volatile ("rdtsc" : "=a" (lo), "=d" (hi));
    return (((uint64_t)hi << 32) | lo) * ticks_per_sec_inv;
}
constexpr int TLE = 10;  // sec

class xor_shift_128 {
public:
    typedef uint32_t result_type;
    xor_shift_128(uint32_t seed = 42) {
        set_seed(seed);
    }
    void set_seed(uint32_t seed) {
        a = seed = 1812433253u * (seed ^ (seed >> 30));
        b = seed = 1812433253u * (seed ^ (seed >> 30)) + 1;
        c = seed = 1812433253u * (seed ^ (seed >> 30)) + 2;
        d = seed = 1812433253u * (seed ^ (seed >> 30)) + 3;
    }
    uint32_t operator() () {
        uint32_t t = (a ^ (a << 11));
        a = b; b = c; c = d;
        return d = (d ^ (d >> 19)) ^ (t ^ (t >> 8));
    }
    static constexpr uint32_t max() { return numeric_limits<result_type>::max(); }
    static constexpr uint32_t min() { return numeric_limits<result_type>::min(); }
private:
    uint32_t a, b, c, d;
};

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
    const parameters *param;

    // the state of solution
    vector<bool> used;
    ll used_sum_p;
    ll used_sum_m;
    union_find_tree used_uft;
    ll m_raw_score;

    solution(const parameters *param_)
            : param(param_) {
        assert (param != nullptr);
        auto const & N = param->N;
        auto const & E = param->E;
        used.resize(E);
        used_sum_p = 0;
        used_sum_m = 0;
        used_uft = union_find_tree(N);
        m_raw_score = -1;
    }

    void use(int i) {
        auto const & NM = param->NM;
        auto const & E = param->E;
        auto const & edges = param->edges;
        assert (0 <= i and i <= E);
        if (used[i]) return;
        assert (used_sum_m + edges[i].m <= NM);

        used[i] = true;
        used_sum_p += edges[i].p;
        used_sum_m += edges[i].m;
        used_uft.unite_trees(edges[i].a, edges[i].b);
        m_raw_score = -1;
    }

    vector<int> get_answer() const {
        auto const & E = param->E;
        vector<int> answer;
        REP (i, E) {
            if (used[i]) {
                answer.push_back(i);
            }
        }
        return answer;
    }

    tuple<int, vector<int>, vector<vector<int> > > get_compressed_graph() {
        auto const & N = param->N;
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

    ll get_raw_score() {
        auto const & routes = param->routes;
        if (m_raw_score != -1) return m_raw_score;
        ll used_route_sum_p = 0;
        for (auto const & route : routes) {
            if (used_uft.is_same(route.a, route.b)) {
                used_route_sum_p += route.p;
            }
        }
        return m_raw_score = used_sum_p * used_route_sum_p;
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

solution use_edges_greedily_without_routes(solution sln) {
    auto const & NM = sln.param->NM;
    auto const & E = sln.param->E;
    auto const & edges = sln.param->edges;

    vector<int> order(E);
    iota(ALL(order), 0);
    sort(ALL(order), [&](int i, int j) {
        return edges[i].p * edges[j].m > edges[j].p * edges[i].m;
    });
    for (int i : order) {
        if (not sln.used[i] and sln.used_sum_m + edges[i].m <= NM) {
            sln.use(i);
        }
    }
    return sln;
}

template <class Generator>
vector<int> find_solution(ll NM, int N, int E, vector<connection_t> const & edges, int R, vector<route_t> const & routes, double clock_begin, Generator & gen) {
    parameters param(NM, N, E, edges, R, routes);
    vector<int> answer;
    ll highscore = LLONG_MIN;

    solution sln(&param);
    int size; vector<int> component_of; vector<vector<int> > vertices_of;
    tie(size, component_of, vertices_of) = sln.get_compressed_graph();

    double temperature = 1;
    for (unsigned iteration = 0; ; ++ iteration) {
        if (iteration % 32 == 0) {
            temperature = 1.0 - (rdtsc() - clock_begin) / (TLE * 0.8);
            if (temperature <= 0.0) {
                break;
            }
        }

        vector<int> path;
        ll sum_m;
        if (bernoulli_distribution(0.2)(gen)) {
            int i = uniform_int_distribution<int>(0, E - 1)(gen);
            path.push_back(i);
            sum_m = (sln.used[i] ? 0 : edges[i].m);
        } else {
            int i = uniform_int_distribution<int>(0, R - 1)(gen);
            tie(path, sum_m, ignore) = find_nice_path_for_route(param, routes[i], size, component_of, vertices_of);
        }

        solution next_sln = sln;
        if (0 < sum_m and sln.used_sum_m + sum_m <= NM) {
            for (int i : path) {
                next_sln.use(i);
            }
        } else {
            vector<int> order = path;
            REP (i, E) if (sln.used[i]) {
                order.push_back(i);
            }
            shuffle(order.begin() + path.size(), order.end(), gen);
            next_sln = solution(&param);
            for (int i : order) {
                if (next_sln.used_sum_m + edges[i].m <= NM) {
                    next_sln.use(i);
                }
            }
        }

        ll delta = next_sln.get_raw_score() - sln.get_raw_score();
        auto probability = [&]() {
            constexpr double boltzmann = 0.01;
            return exp(boltzmann * delta) * temperature;
        };
        if (delta >= 0 or bernoulli_distribution(probability())(gen)) {
            if (delta < 0) cerr << "iteration = " << iteration << ": delta = " << delta << endl;
            sln = next_sln;
            if (highscore < sln.get_raw_score()) {
                highscore = sln.get_raw_score();
                answer = sln.get_answer();
                cerr << "iteration = " << iteration << ": highscore = " << highscore << endl;
            }
        }
    }

    cerr << "score: " << highscore << endl;
    return answer;
}


class RoadNetwork {
public:
    vector<int> findSolution(int NM, int N, int E, vector<string> edges, int R, vector<string> routes) {
        double clock_begin = rdtsc();

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

        xor_shift_128 gen;
        return find_solution(NM, N, E, parsed_edges, R, parsed_routes, clock_begin, gen);
    }
};
