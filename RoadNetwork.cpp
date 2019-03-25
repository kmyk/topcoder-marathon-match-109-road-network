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


constexpr ll INF = 1e9 + 7;
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
    vector<vector<ll> > dist_f;
    vector<vector<int> > reconstruct;  // `reconstruct[a][b]` is the parent node of `b` on a shortest-path tree from `a`

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
    }
};

struct all_pairs_shortest_path {
    vector<vector<ll> > p;
    vector<vector<ll> > m;
    vector<vector<ll> > f;
    vector<vector<int> > reconstruct;  // `reconstruct[a][b]` is the parent node of `b` on a shortest-path tree from `a`

    template <class Func>
    all_pairs_shortest_path(parameters const & param, Func func) {
        auto const & N = param.N;
        auto const & edges = param.edges;
        auto const & edges_of = param.edges_of;

        p.resize(N, vector<ll>(N, -1));
        m.resize(N, vector<ll>(N, INF));
        f.resize(N, vector<ll>(N, INF));
        REP (a, N) {
            p[a][a] = 0;
            m[a][a] = 0;
            f[a][a] = 0;
        }
        for (auto const & edge : edges) {
            p[edge.a][edge.b] = edge.p;
            m[edge.a][edge.b] = edge.m;
            f[edge.a][edge.b] = func(edge);
            p[edge.b][edge.a] = edge.p;
            m[edge.b][edge.a] = edge.m;
            f[edge.b][edge.a] = func(edge);
        }
        REP (c, N) {
            REP (a, N) if (f[a][c] < INF) {
                REP (b, N) if (f[c][b] < INF) {
                    if (f[a][b] > f[a][c] + f[c][b]) {
                        m[a][b] = m[a][c] + m[c][b];
                        p[a][b] = p[a][c] + p[c][b];
                        f[a][b] = f[a][c] + f[c][b];
                    }
                }
            }
        }

        reconstruct.resize(N, vector<int>(N, -1));
        REP (a, N) {
            REP (b, N) if (b != a) {
                for (int i : edges_of[b]) {
                    int c = opposite(b, edges[i]);
                    if (f[a][c] + func(edges[i]) == f[a][b]
                            and m[a][c] + edges[i].m == m[a][b]
                            and p[a][c] + edges[i].p == p[a][b]) {
                        reconstruct[a][b] = i;
                        break;
                    }
                }
            }
        }
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

void merge_greedily(parameters const & param, all_pairs_shortest_path const & dist, vector<bool> const & selected, solution & sln) {
    auto const & NM = param.NM;
    auto const & N = param.N;
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

    vector<bool> supernode_exist(N, true);
    vector<int> supernode_of(N, -1);
    REP (i, supernodes.size()) {
        supernode_of[supernodes[i].front()] = i;
    }

    vector<vector<ll> > cost(supernodes.size(), vector<ll>(supernodes.size(), INF));
    vector<vector<pair<int, int> > > link(supernodes.size(), vector<pair<int, int> >(supernodes.size(), make_pair(-1, -1)));
    REP (i, supernodes.size()) {
        cost[i][i] = 0;
    }
    REP (i, supernodes.size()) {
        REP (j, i) {
            for (int a : supernodes[i]) {
                for (int b : supernodes[j]) {
                    if (dist.f[a][b] < cost[i][j]) {
                        cost[i][j] = dist.f[a][b];
                        link[i][j] = make_pair(a, b);
                    }
                }
            }
            cost[j][i] = cost[i][j];
            link[j][i] = link[i][j];
        }
    }

    constexpr ll INF = 1e9 + 7;
    while (true) {
        pair<int, int> super_path;
        ll max_f = INF;
        REP (i, supernodes.size()) if (supernode_exist[i]) {
            REP (j, i) if (supernode_exist[j]) {
                if (cost[i][j] < max_f) {
                    max_f = cost[i][j];
                    super_path = make_pair(i, j);
                }
            }
        }
        if (max_f == INF) break;

        int i, j; tie(i, j) = super_path;
        int a, b; tie(a, b) = link[i][j];

        vector<int> path;
        ll sum_m = 0;
        while (b != a) {
            int k = dist.reconstruct[a][b];
            assert (k != -1);
            path.push_back(k);
            sum_m += edges[k].m;
            b = opposite(b, edges[k]);
        }
        if (sln.sum_m + sum_m > NM) return;

        vector<int> connected;
        for (int k : path) {
            sln.use(k);
            for (int a : { edges[k].a, edges[k].b }) {
                if (supernode_of[a] != -1 and not count(ALL(connected), supernode_of[a])) {
                    connected.push_back(supernode_of[a]);
                }
            }
        }
        sort(ALL(connected), [&](int i, int j) {
            return supernodes[i].size() > supernodes[j].size();
        });
        i = connected.front();
        while (connected.size() >= 2) {
            int j = connected.back();
            connected.pop_back();

            REP (k, supernodes.size()) if (i != k and j != k and supernode_exist[k]) {
                if (cost[j][k] < cost[i][k]) {
                    cost[i][k] = cost[k][i] = cost[j][k];
                    link[i][k] = link[k][i] = link[j][k];
                }
            }
            for (int a : supernodes[j]) {
                supernode_of[a] = i;
            }
            copy(ALL(supernodes[j]), back_inserter(supernodes[i]));
            supernode_exist[j] = false;
        }
        for (int k : path) {
            for (int a : { edges[k].a, edges[k].b }) if (supernode_of[a] == -1) {
                REP (j, supernodes.size()) if (i != j and supernode_exist[j]) {
                    for (int b : supernodes[j]) {
                        if (dist.f[a][b] < cost[i][j]) {
                            cost[i][j] = cost[j][i] = dist.f[a][b];
                            link[i][j] = link[j][i] = make_pair(a, b);
                        }
                    }
                }
                supernodes[i].push_back(a);
                supernode_of[a] = i;
            }
        }
    }
}

vector<bool> make_initial_selected(parameters const & param, all_pairs_shortest_path const & dist, solution & sln) {
    auto const & NM = param.NM;
    auto const & N = param.N;
    auto const & edges = param.edges;
    auto const & R = param.R;
    auto const & routes = param.routes;
    assert (sln.answer.empty());

    auto get_completed = [&](vector<bool> const & selected) {
        sln.reset();
        merge_greedily(param, dist, selected, sln);
        vector<bool> completed(R);
        for (int i : sln.get_completed()) {
            completed[i] = true;
        }
        return completed;
    };

    vector<int> order(R);
    iota(ALL(order), 0);
    sort(ALL(order), [&](int i, int j) {
        ll m_i = dist.m[routes[i].a][routes[i].b];
        ll m_j = dist.m[routes[j].a][routes[j].b];
        return m_i < m_j;
    });
    auto get_greedy_selected = [&](int size) {
        vector<bool> selected(R);
        REP (i, size) {
            selected[order[i]] = true;
        }
        return selected;
    };

    int size = 0;
    REP_R (k, 30) {
        int next_size = size | (1 << k);
        if (next_size <= R) {
            auto completed = get_completed(get_greedy_selected(next_size));
            if (count(ALL(completed), true) >= next_size) {
                size = next_size;
            }
        }
    }
    return get_completed(get_greedy_selected(size));
}

template <class Generator>
vector<int> find_solution(ll NM, int N, int E, vector<connection_t> const & edges, int R, vector<route_t> const & routes, double clock_begin, Generator & gen) {
    parameters param(NM, N, E, edges, R, routes);

    all_pairs_shortest_path dist(param, [&](connection_t const & edge) {
        const int dist_table[5] = { 6, 5, 4, 3, 3 };
        int r = edge.p / edge.m;
        return dist_table[r - 1] * edge.m;
    });

    solution sln(&param);
    vector<bool> selected = make_initial_selected(param, dist, sln);
    sln.reset();
    merge_greedily(param, dist, selected, sln);
    ll score = sln.get_raw_score();

    vector<int> answer = sln.answer;
    ll highscore = score;
    cerr << "initial completed routes: " << count(ALL(selected), true) << " / " << R << endl;
    cerr << "initial score: " << highscore << endl;

    double temperature = 1;
    for (unsigned iteration = 0; ; ++ iteration) {
        temperature = 1.0 - (rdtsc() - clock_begin) / (TLE * 0.95);
        if (temperature <= 0.0) {
            cerr << "iteration = " << iteration << " : done" << endl;
            break;
        }

        // choose a neighborhood
        int i = uniform_int_distribution<int>(0, R - 1)(gen);
        int j = -1;
        int k = -1;
        if (selected[i] and bernoulli_distribution(0.8)(gen)) {
            int j1 = uniform_int_distribution<int>(0, R - 1)(gen);
            j = j1;
            while (j < R and selected[j]) ++ j;
            if (j == R) j = 0;
            while (j < j1 and selected[j]) ++ j;
        }
        if (iteration >= 50000 and bernoulli_distribution(0.8)(gen)) {
            k = uniform_int_distribution<int>(0, R - 1)(gen);
        }
        if (i != -1) selected[i] = not selected[i];
        if (j != -1) selected[j] = not selected[j];
        if (k != -1) selected[k] = not selected[k];

        // compute
        sln.reset();
        merge_greedily(param, dist, selected, sln);
        sln.add_edges_greedily();

        ll delta = sln.get_raw_score() - score;
        auto probability = [&]() {
            double boltzmann = 0.3 / (1 + sqrt(highscore));
            return exp(boltzmann * delta / temperature);
        };
        if (delta >= 0 or bernoulli_distribution(probability())(gen)) {
            // if (delta < 0) cerr << "iteration = " << iteration << ": delta = " << delta << endl;
            score += delta;
            fill(ALL(selected), false);
            for (int i : sln.get_completed()) {
                selected[i] = true;
            }
        } else {
            // revert
            if (i != -1) selected[i] = not selected[i];
            if (j != -1) selected[j] = not selected[j];
            if (k != -1) selected[k] = not selected[k];
        }

        // update the answer
        if (delta > 0) {
            sln.remove_unnecessary_edges();
            sln.add_edges_greedily();
            if (highscore < sln.get_raw_score()) {
                highscore = sln.get_raw_score();
                answer = sln.answer;
                // cerr << "iteration = " << iteration << " : highscore = " << highscore << " / " << endl;
            }
        }
    }

    int count_r[5] = {};
    for (int i : answer) {
        int r = edges[i].p / edges[i].m;
        ++ count_r[r - 1];
    }
    REP (r, 5) {
        cerr << "P / M = " << r + 1 << ": " << count_r[r] << endl;
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
