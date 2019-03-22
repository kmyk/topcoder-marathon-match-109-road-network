#include <algorithm>
#include <cassert>
#include <climits>
#include <cmath>
#include <cstdlib>
#include <functional>
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
template <class T, class U> inline void chmax(T & a, U const & b) { a = max<T>(a, b); }
template <class T, class U> inline void chmin(T & a, U const & b) { a = min<T>(a, b); }


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
    vector<int> non_isolated;
    union_find_tree() = default;
    explicit union_find_tree(size_t n) : data(n, -1) {}
    bool is_root(int i) { return data[i] < 0; }
    int find_root(int i) { return is_root(i) ? i : (data[i] = find_root(data[i])); }
    int tree_size(int i) { return - data[find_root(i)]; }
    int unite_trees(int i, int j) {
        i = find_root(i); j = find_root(j);
        if (tree_size(i) == 1) non_isolated.push_back(i);
        if (tree_size(j) == 1) non_isolated.push_back(j);
        if (i != j) {
            if (tree_size(i) < tree_size(j)) swap(i,j);
            data[i] += data[j];
            data[j] = i;
        }
        return i;
    }
    bool is_same(int i, int j) { return find_root(i) == find_root(j); }
    vector<int> const & list_non_isolated() { return non_isolated; }
    void reset() {
        for (int i : non_isolated) {
            data[i] = -1;
        }
        non_isolated.clear();
    }
    void swap_(union_find_tree & other) {
        this->data.swap(other.data);
        this->non_isolated.swap(other.non_isolated);
    }
};


constexpr int MIN_N = 30;
constexpr int MAX_N = 1000;

struct connection_t {
    int a, b;
    ll m, p;
};

struct route_t {
    int a, b;
    ll p;
};

int opposite(int a, connection_t const & edge) {
    return a ^ edge.a ^ edge.b;
}

template <class Container, class Generator>
typename Container::value_type const & choose(Container const & a, Generator & gen) {
    assert (not a.empty());
    int i = uniform_int_distribution<int>(0, a.size() - 1)(gen);
    return a[i];
}

vector<array<int, 3> > list_triangles(int E, vector<connection_t> const & edges, vector<vector<int> > const & edges_of) {
    vector<array<int, 3> > triangles;
    REP (i, E) {
        for (int j : edges_of[edges[i].a]) if (i < j) {
            int c1 = opposite(edges[i].a, edges[j]);
            for (int k : edges_of[edges[i].b]) if (j < k) {
                int c2 = opposite(edges[i].b, edges[k]);
                if (c1 == c2) {
                    triangles.push_back({ i, j, k });
                }
            }
        }
    }
    return triangles;
}

vector<array<int, 4> > list_quadrangles(int E, vector<connection_t> const & edges, vector<vector<int> > const & edges_of) {
    vector<array<int, 4> > quadrangles;
    REP (i, E) {
        int a = edges[i].a;
        int b = edges[i].b;
        for (int j : edges_of[b]) if (i < j) {
            int c = opposite(b, edges[j]);
            for (int k : edges_of[c]) if (i < k and j < k) {
                int d1 = opposite(c, edges[k]);
                for (int l : edges_of[a]) if (i < l and l != j and l != k) {
                    int d2 = opposite(a, edges[l]);
                    if (d1 == d2) {
                        quadrangles.push_back({ i, j, k, l });
                    }
                }
            }
        }
    }
    return quadrangles;
}


struct parameters {
    ll NM;
    int N;
    int E;
    vector<connection_t> edges;
    int R;
    vector<route_t> routes;

    vector<int> greedy_order;

    vector<vector<int> > edges_of;
    vector<vector<int> > routes_of;

    vector<vector<ll> > dist_p;
    vector<vector<ll> > dist_m;
    vector<vector<int> > reconstruct;  // `reconstruct[a][b]` is the parent node of `b` on a shortest-path tree from `a`

    vector<array<int, 3> > triangles;
    vector<array<int, 4> > quadrangles;
    vector<vector<int> > triangles_of;
    vector<vector<int> > quadrangles_of;

    parameters(ll NM_, int N_, int E_, vector<connection_t> const & edges_, int R_, vector<route_t> const & routes_)
            : NM(NM_), N(N_), E(E_), edges(edges_), R(R_), routes(routes_) {
        greedy_order.resize(E);
        iota(ALL(greedy_order), 0);
        sort(ALL(greedy_order), [&](int i, int j) {
            return edges[i].p * edges[j].m > edges[j].p * edges[i].m;
        });

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


        // Warshall-Floyd
        constexpr ll INF = 1e9 + 7;
        dist_p.resize(N, vector<ll>(N, -1));
        dist_m.resize(N, vector<ll>(N, INF));
        REP (a, N) {
            dist_p[a][a] = 0;
            dist_m[a][a] = 0;
        }
        for (auto const & edge : edges) {
            dist_p[edge.a][edge.b] = edge.p;
            dist_m[edge.a][edge.b] = edge.m;
            dist_p[edge.b][edge.a] = edge.p;
            dist_m[edge.b][edge.a] = edge.m;
        }
        REP (c, N) {
            REP (a, N) if (dist_p[a][c] >= 0) {
                REP (b, N) if (dist_p[c][b] >= 0) {
                    if (dist_m[a][b] > dist_m[a][c] + dist_m[c][b]) {
                        dist_m[a][b] = dist_m[a][c] + dist_m[c][b];
                        dist_p[a][b] = dist_p[a][c] + dist_p[c][b];
                    }
                }
            }
        }

        reconstruct.resize(N, vector<int>(N, -1));
        REP (a, N) {
            REP (b, N) if (b != a) {
                for (int i : edges_of[b]) {
                    int c = opposite(b, edges[i]);
                    if (dist_m[a][c] + edges[i].m == dist_m[a][b]) {
                        reconstruct[a][b] = i;
                        break;
                    }
                }
            }
        }

        triangles = list_triangles(E, edges, edges_of);
        triangles_of.resize(E);
        REP (i, triangles.size()) {
            triangles_of[triangles[i][0]].push_back(i);
            triangles_of[triangles[i][1]].push_back(i);
            triangles_of[triangles[i][2]].push_back(i);
        }

        quadrangles = list_quadrangles(E, edges, edges_of);
        quadrangles_of.resize(E);
        REP (i, quadrangles.size()) {
            quadrangles_of[quadrangles[i][0]].push_back(i);
            quadrangles_of[quadrangles[i][1]].push_back(i);
            quadrangles_of[quadrangles[i][2]].push_back(i);
            quadrangles_of[quadrangles[i][3]].push_back(i);
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

/**
 * @note O((|X| + R) \alpha(|X|)) for the answer X
 */
vector<int> list_completed_routes(parameters const & param, vector<int> const & answer) {
    static union_find_tree uft(MAX_N);
    auto const & edges = param.edges;
    auto const & R = param.R;
    auto const & routes = param.routes;

    vector<int> completed;
    for (int i : answer) {
        auto const & edge = edges[i];
        uft.unite_trees(edge.a, edge.b);
    }
    REP (i, R) {
        if (uft.is_same(routes[i].a, routes[i].b)) {
            completed.push_back(i);
        }
    }
    uft.reset();
    return completed;
}

struct solution {
    const parameters *param;

    // the state of solution
    vector<int> answer;
    vector<bool> used;  // edges
    ll sum_p;
    ll sum_m;
    union_find_tree uft;
    vector<bool> used_node;

private:
    vector<int> m_completed;  // routes
    ll m_route_sum_p;

public:
    vector<int> const & get_completed() {
        auto const & routes = param->routes;
        if (m_route_sum_p != -1) {
            return m_completed;
        }
        m_completed = list_completed_routes(*param, answer);
        m_route_sum_p = 0;
        for (int i : m_completed) {
            m_route_sum_p += routes[i].p;
        }
        return m_completed;
    }

    ll get_route_sum_p() {
        get_completed();
        return m_route_sum_p;
    }

public:

    solution(const parameters *param_)
            : param(param_) {
        assert (param != nullptr);
        auto const & N = param->N;
        auto const & E = param->E;
        used.resize(E);
        sum_p = 0;
        sum_m = 0;
        uft = union_find_tree(N);
        used_node.resize(N);
        m_route_sum_p = 0;
    }

    void use(int i) {
        auto const & NM = param->NM;
        auto const & E = param->E;
        auto const & edges = param->edges;
        assert (0 <= i and i <= E);
        if (used[i]) return;
        assert (sum_m + edges[i].m <= NM);

        auto const & edge = edges[i];
        answer.push_back(i);
        used[i] = true;
        sum_p += edge.p;
        sum_m += edge.m;
        for (int c : { edge.a, edge.b }) {
            if (not used_node[c]) {
                used_node[c] = true;
            }
        }
        uft.unite_trees(edge.a, edge.b);
        m_completed.clear();
        m_route_sum_p = -1;
    }

    void reset() {
        for (int i : answer) {
            used[i] = false;
        }
        answer.clear();
        sum_p = 0;
        sum_m = 0;
        for (int a : uft.list_non_isolated()) {
            used_node[a] = false;
        }
        uft.reset();
        m_completed.clear();
        m_route_sum_p = 0;
    }

    void swap_(solution & other) {
        swap(this->param, other.param);

        this->answer.swap(other.answer);
        this->used.swap(other.used);
        swap(this->sum_p, other.sum_p);
        swap(this->sum_m, other.sum_m);
        this->uft.swap_(other.uft);
        this->used_node.swap(other.used_node);

        this->m_completed.swap(other.m_completed);
        swap(this->m_route_sum_p, other.m_route_sum_p);
    }

    ll get_raw_score() {
        return sum_p * get_route_sum_p();
    }

    void remove_unnecessary_edges() {
        struct state_t {
            ll p, m;
            int parent;
            int edge;
            bool fixed;
        };
        static vector<bool> original_used;
        static vector<state_t> dist(MAX_N, { -1ll, 1ll, -1, -1, false });
        static vector<int> dist_used;
        auto const & E = param->E;
        auto const & edges = param->edges;
        auto const & routes = param->routes;
        auto const & edges_of = param->edges_of;

        vector<int> original_nodes = uft.list_non_isolated();
        vector<int> original_answer = answer;
        if (original_used.size() < E) {
            original_used.resize(E, false);
        }
        for (int i : original_answer) {
            original_used[i] = true;
        }
        vector<int> completed = get_completed();
        reset();
        for (int i : completed) {
            auto const & route = routes[i];
            if (uft.is_same(route.a, route.b)) continue;

            // Dijkstra
            auto cmp = [&](tuple<int, ll, ll> const & a, tuple<int, ll, ll> const & b) {
                ll a_m, a_p; tie(ignore, a_m, a_p) = a;
                ll b_m, b_p; tie(ignore, b_m, b_p) = b;
                return a_p * b_m < b_p * a_m;
            };
            priority_queue<tuple<int, ll, ll>, vector<tuple<int, ll, ll> >, decltype(cmp)> que(cmp);
            function<void (int)> go = [&](int a) {
                dist[a].fixed = true;
                for (int i : edges_of[a]) if (original_used[i]) {
                    auto const & edge = edges[i];
                    int b = opposite(a, edge);
                    if (not dist[b].fixed) {
                        if (uft.is_same(a, b)) {
                            if (dist[b].p == -1) {
                                dist_used.push_back(b);
                            }
                            dist[b].p = dist[a].p + edge.p;
                            dist[b].m = dist[a].m + edge.m;
                            dist[b].parent = dist[a].parent;
                            dist[b].edge = dist[a].edge;
                            go(b);
                        } else if ((dist[a].p + edge.p) * dist[b].m > dist[b].p * (dist[a].m + edge.m)) {
                            if (dist[b].p == -1) {
                                dist_used.push_back(b);
                            }
                            dist[b].p = dist[a].p + edge.p;
                            dist[b].m = dist[a].m + edge.m;
                            dist[b].parent = a;
                            dist[b].edge = i;
                            que.emplace(b, dist[b].m, dist[b].p);
                        }
                    }
                }
            };
            dist[route.a].p = 0;
            dist[route.a].m = 0;
            dist[route.a].parent = -1;
            dist[route.a].edge = -1;
            dist_used.push_back(route.a);
            go(route.a);
            while (not dist[route.b].fixed) {
                assert (not que.empty());
                int a; ll a_m, a_p; tie(a, a_m, a_p) = que.top();
                que.pop();
                if (a_p == dist[a].p and a_m == dist[a].m) {
                    go(a);
                }
            }

            // reconstruct
            for (int a = route.b; dist[a].edge != -1; a = dist[a].parent) {
                assert (original_used[dist[a].edge]);
                assert (not used[dist[a].edge]);
                use(dist[a].edge);
            }

            // clean up
            for (int a : dist_used) {
                dist[a].p = -1;
                dist[a].m = 1;
                dist[a].parent = -1;
                dist[a].edge = -1;
                dist[a].fixed = false;
            }
            dist_used.clear();
        }

        // clean up
        for (int i : original_answer) {
            original_used[i] = false;
        }
    }

    int add_edges_greedily() {
        auto const & NM = param->NM;
        auto const & edges = param->edges;
        int succeeded = 0;
        int failed = 0;
        for (int i : param->greedy_order) {
            if (sum_m == NM) break;
            if (sum_m + edges[i].m <= NM) {
                if (not used[i]) {
                    ++ succeeded;
                    use(i);
                }
            } else {
                ++ failed;
                if (failed >= 10) break;
            }
        }
        return succeeded;
    }
};

void merge_greedily(parameters const & param, vector<bool> const & selected, double ratio, solution & sln) {
    auto const & NM = param.NM;
    auto const & edges = param.edges;
    auto const & R = param.R;
    auto const & routes = param.routes;
    assert (sln.answer.empty());

    vector<vector<int> > supernodes;
    REP (i, R) if (selected[i]) {
        supernodes.emplace_back(1, routes[i].a);
        supernodes.emplace_back(1, routes[i].b);
    }
    sort(ALL(supernodes));
    supernodes.erase(unique(ALL(supernodes)), supernodes.end());

    constexpr ll INF = 1e9 + 7;
    while (supernodes.size() >= 2) {
        tuple<int, int, int, int> path;
        ll sum_p = -1;
        ll sum_m = INF;

        REP (i, supernodes.size()) {
            REP (j, i) {
                for (int a : supernodes[i]) {
                    for (int b : supernodes[j]) {
                        if ((1 + ratio * sum_p) * param.dist_m[a][b] < (1 + ratio * param.dist_p[a][b]) * sum_m) {
                            sum_p = param.dist_p[a][b];
                            sum_m = param.dist_m[a][b];
                            path = make_tuple(i, j, a, b);
                        }
                    }
                }
            }
        }
        if (sln.sum_m + sum_m > NM) break;

        int i, j, a, b; tie(i, j, a, b) = path;
        if (supernodes[i].size() < supernodes[j].size()) {
            swap(i, j);
        }
        while (b != a) {
            int k = param.reconstruct[a][b];
            assert (k != -1);
            sln.use(k);
            b = opposite(b, edges[k]);
            if (b != a) {
                supernodes[i].push_back(b);
            }
        }
        copy(ALL(supernodes[j]), back_inserter(supernodes[i]));
        supernodes[j].swap(supernodes.back());
        supernodes.pop_back();
    }
}

template <class Generator>
vector<int> find_solution(ll NM, int N, int E, vector<connection_t> const & edges, int R, vector<route_t> const & routes, double clock_begin, Generator & gen) {
    parameters param(NM, N, E, edges, R, routes);
    solution sln(&param);
    vector<int> answer;
    ll highscore = -1;

    vector<bool> selected(R);
    {  // use initial state constructed by greedy
        vector<int> order(R);
        iota(ALL(order), 0);
        sort(ALL(order), [&](int i, int j) {
            ll p_i = param.dist_p[routes[i].a][routes[i].b];
            ll m_i = param.dist_m[routes[i].a][routes[i].b];
            ll p_j = param.dist_p[routes[j].a][routes[j].b];
            ll m_j = param.dist_m[routes[j].a][routes[j].b];
            return p_i * m_j > p_j * m_i;
        });
        order.resize(R / 2);
        for (int i : order) {
            selected[i] = true;
        }
    }
    double ratio = 0.05;
    ll score = -1;
    while (score == -1) {
        sln.reset();
        merge_greedily(param, selected, ratio, sln);
        if (sln.get_completed().size() < count(ALL(selected), true)) {
            REP (i, R) if (selected[i]) {
                selected[i] = bernoulli_distribution(0.9)(gen);
            }
        } else {
            score = sln.get_raw_score();
            answer = sln.answer;
            highscore = score;
        }
    }

    double temperature = 1;
    for (unsigned iteration = 0; ; ++ iteration) {
        temperature = 1.0 - (rdtsc() - clock_begin) / (TLE * 0.95);
        if (temperature <= 0.0) {
            cerr << "iteration = " << iteration << " : done" << endl;
            break;
        }

        // choose a neighborhood
        int i = -1;
        int j = -1;
        double next_ratio = ratio;
        if (bernoulli_distribution(0.95)(gen)) {
            i = uniform_int_distribution<int>(0, R - 1)(gen);
            if (selected[i] and bernoulli_distribution(0.8)(gen)) {
                int j1 = uniform_int_distribution<int>(0, R - 1)(gen);
                j = j1;
                while (j < R and selected[j]) ++ j;
                if (j == R) j = 0;
                while (j < j1 and selected[j]) ++ j;
            }
        } else {
            next_ratio *= uniform_real_distribution<double>(0.8, 1.2)(gen);
        }
        if (i != -1) selected[i] = not selected[i];
        if (j != -1) selected[j] = not selected[j];

        // compute
        sln.reset();
        merge_greedily(param, selected, next_ratio, sln);
        sln.add_edges_greedily();

        ll delta = sln.get_raw_score() - score;
        auto probability = [&]() {
            double boltzmann = 0.1 / (1 + sqrt(highscore));
            return exp(boltzmann * delta / temperature);
        };
        if (delta >= 0 or bernoulli_distribution(probability())(gen)) {
            // if (delta < 0) cerr << "iteration = " << iteration << ": delta = " << delta << endl;
            score += delta;
            ratio = next_ratio;
            fill(ALL(selected), false);
            for (int i : sln.get_completed()) {
                selected[i] = true;
            }
        } else {
            // revert
            if (i != -1) selected[i] = not selected[i];
            if (j != -1) selected[j] = not selected[j];
        }

        // update the answer
        if (delta > 0) {
            REP (p, 2) {
                if (highscore < sln.get_raw_score()) {
                    highscore = sln.get_raw_score();
                    answer = sln.answer;
                    // cerr << "iteration = " << iteration << " : highscore = " << highscore << " / " << ratio << endl;
                }
                if (not p) {
                    sln.remove_unnecessary_edges();
                    sln.add_edges_greedily();
                }
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

        assert (MIN_N <= N and N <= MAX_N);
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
