#pragma GCC optimize("Ofast")
#include <bits/stdc++.h>
using namespace std;
#define rep(i, n) for (int i = 0; i < (int)(n); i++)
#define ALL(v) (v).begin(), (v).end()
using ll = long long;
constexpr int INF = 1e9;
constexpr long long LINF = 1e18;
constexpr long long MOD = 1e9 + 7;

constexpr double TL1 = 8.0;
constexpr double TL2 = 9.0;

constexpr int dy[] = {0, 1, 0, -1}, dx[] = {1, 0, -1, 0};

int N, C, H;
vector<vector<int>> grid;

struct Timer {
    chrono::system_clock::time_point start, now;

    Timer() {
        start = chrono::system_clock::now();
    }

    double getTime() {
        now = chrono::system_clock::now();
        return chrono::duration<double>(now - start).count();
    }
};

// https://natsugiri.hatenablog.com/entry/2016/10/10/035445
template <class T>
struct PriorityQueue {
    vector<T> d;

    PriorityQueue() {}

    PriorityQueue(const vector<T>& d_) : d(d_) {
        make_heap();
    }

    template <class Iter>
    PriorityQueue(Iter first, Iter last) : d(first, last) {
        make_heap();
    }

    void push(const T& x) {
        int k = d.size();
        d.push_back(x);
        up(k);
    }

    void pop_min() {
        if (d.size() < 3u) {
            d.pop_back();
        } else {
            swap(d[1], d.back());
            d.pop_back();
            int k = down(1);
            up(k);
        }
    }

    void pop_max() {
        if (d.size() < 2u) {
            d.pop_back();
        } else {
            swap(d[0], d.back());
            d.pop_back();
            int k = down(0);
            up(k);
        }
    }

    const T& get_min() const {
        return d.size() < 2u ? d[0] : d[1];
    }

    const T& get_max() const {
        return d[0];
    }

    int size() const {
        return d.size();
    }

    bool empty() const {
        return d.empty();
    }

    void make_heap() {
        for (int i = d.size(); i--;) {
            if (i & 1 && d[i - 1] < d[i]) swap(d[i - 1], d[i]);
            int k = down(i);
            up(k, i);
        }
    }

    inline int parent(int k) const {
        return ((k >> 1) - 1) & ~1;
    }

    int down(int k) {
        int n = d.size();
        if (k & 1) {  // min heap
            while (2 * k + 1 < n) {
                int c = 2 * k + 3;
                if (n <= c || d[c - 2] < d[c]) c -= 2;
                if (c < n && d[c] < d[k]) {
                    swap(d[k], d[c]);
                    k = c;
                } else
                    break;
            }
        } else {  // max heap
            while (2 * k + 2 < n) {
                int c = 2 * k + 4;
                if (n <= c || d[c] < d[c - 2]) c -= 2;
                if (c < n && d[k] < d[c]) {
                    swap(d[k], d[c]);
                    k = c;
                } else
                    break;
            }
        }
        return k;
    }

    int up(int k, int root = 1) {
        if ((k | 1) < (int)d.size() && d[k & ~1] < d[k | 1]) {
            swap(d[k & ~1], d[k | 1]);
            k ^= 1;
        }

        int p;
        while (root < k && d[p = parent(k)] < d[k]) {  // max heap
            swap(d[p], d[k]);
            k = p;
        }
        while (root < k && d[k] < d[p = parent(k) | 1]) {  // min heap
            swap(d[p], d[k]);
            k = p;
        }
        return k;
    }
};

Timer tmr;
double startTime = -1, nowTime = -1;

struct Move {
    int r, c;
    char move, dir;
};

struct State {
    vector<Move> moves;
    vector<vector<int>> grid;
    double score;
    int rawscore;
    bool operator<(const State& a) const {
        return score < a.score;
    };
};

Move makeMove(int& r, int& c, int& nr, int& nc) {
    if (nr < r) {
        if (r - 1 == nr) {
            return Move{r, c, 'M', 'U'};
        } else {
            return Move{r, c, 'S', 'U'};
        }
    } else if (nr > r) {
        if (r + 1 == nr) {
            return Move{r, c, 'M', 'D'};
        } else {
            return Move{r, c, 'S', 'D'};
        }
    } else if (nc < c) {
        if (c - 1 == nc) {
            return Move{r, c, 'M', 'L'};
        } else {
            return Move{r, c, 'S', 'L'};
        }
    } else {
        if (c + 1 == nc) {
            return Move{r, c, 'M', 'R'};
        } else {
            return Move{r, c, 'S', 'R'};
        }
    }
}

State solve() {
    using P = pair<int, int>;
    PriorityQueue<State> beamQue[N * N + 1];
    State state, bestState;
    state.grid = grid;
    state.score = 0;
    state.rawscore = 0;
    beamQue[0].push(state);
    bestState = state;
    int BEAMSIZE = 4000000 / (N * N * N);
    for (int steps = 0; steps < N * N; steps++) {
        nowTime = tmr.getTime();
        if (nowTime - startTime > TL1) BEAMSIZE = 1;
        if (nowTime - startTime > TL2) break;
        int cnt = 0;
        while (cnt < BEAMSIZE && !beamQue[steps].empty()) {
            State state = beamQue[steps].get_max();
            beamQue[steps].pop_max();
            cnt++;
            queue<P> que;
            int dist[N][N];
            fill(dist[0], dist[N], -1);
            double future[N][N];
            fill(future[0], future[N], 0);
            P to[N][N];
            int numBlock = 0;
            rep(r, N) {
                rep(c, N) {
                    if (state.grid[r][c] == -1) {
                        que.emplace(r, c);
                        dist[r][c] = 0;
                    } else if (state.grid[r][c] > 1) {
                        numBlock++;
                    }
                }
            }
            if (numBlock == 0) break;
            while (!que.empty()) {
                int r, c;
                tie(r, c) = que.front();
                que.pop();
                if (state.grid[r][c] == -1) {
                    rep(i, 4) {  // Slide
                        int nr = r + dx[i], nc = c + dy[i];
                        bool find = false;
                        int fr = -1, fc = -1;
                        int depth = 2;
                        while (0 <= nr && nr < N && 0 <= nc && nc < N) {
                            if (!find) {
                                if (state.grid[nr][nc] == 0) {
                                    que.emplace(nr, nc);
                                    dist[nr][nc] = dist[r][c] + 1;
                                    to[nr][nc] = {r, c};
                                } else if (state.grid[nr][nc] > 0) {
                                    dist[nr][nc] = dist[r][c] + 1;
                                    to[nr][nc] = {r, c};
                                    find = true;
                                    fr = nr, fc = nc;
                                }
                            } else {
                                if (state.grid[nr][nc] == -1) break;
                                if (state.grid[nr][nc] > 0) {
                                    future[fr][fc] +=
                                        (double)(state.grid[nr][nc] - 1.3) /
                                        depth;
                                    depth++;
                                }
                            }
                            nr += dx[i], nc += dy[i];
                        }
                    }
                } else {
                    rep(i, 4) {  // Slide
                        int nr = r + dx[i], nc = c + dy[i];
                        if (nr < 0 || N <= nr || nc < 0 || N <= nc ||
                            state.grid[nr][nc] > 0) {
                            nr -= 2 * dx[i], nc -= 2 * dy[i];
                            bool find = false;
                            int fr = -1, fc = -1;
                            int depth = 2;
                            while (0 <= nr && nr < N && 0 <= nc && nc < N) {
                                if (!find) {
                                    if (state.grid[nr][nc] == 0) {
                                        if (dist[nr][nc] != -1) {
                                            nr -= dx[i], nc -= dy[i];
                                            continue;
                                        }
                                        que.emplace(nr, nc);
                                        dist[nr][nc] = dist[r][c] + 1;
                                        to[nr][nc] = {r, c};
                                    } else if (state.grid[nr][nc] > 0) {
                                        if (dist[nr][nc] != -1) break;
                                        dist[nr][nc] = dist[r][c] + 1;
                                        to[nr][nc] = {r, c};
                                        find = true;
                                        fr = nr, fc = nc;
                                    }
                                } else {
                                    if (dist[nr][nc] != -1) break;
                                    if (state.grid[nr][nc] > 0) {
                                        future[fr][fc] +=
                                            (double)(state.grid[nr][nc] - 1.3) /
                                            depth;
                                        depth++;
                                    }
                                }
                                nr -= dx[i], nc -= dy[i];
                            }
                        }
                    }
                    rep(i, 4) {  // Move
                        int nr = r + dx[i], nc = c + dy[i];
                        if (nr < 0 || N <= nr || nc < 0 || N <= nc) continue;
                        if (dist[nr][nc] != -1) continue;
                        if (state.grid[nr][nc] == 0) {
                            que.emplace(nr, nc);
                            dist[nr][nc] = dist[r][c] + 1;
                            to[nr][nc] = {r, c};
                        } else if (state.grid[nr][nc] > 0) {
                            dist[nr][nc] = dist[r][c] + 1;
                            to[nr][nc] = {r, c};
                            rep(j, 4) {
                                int nnr = nr + dx[j], nnc = nc + dy[i];
                                if (nnr < 0 || N <= nnr || nnc < 0 || N <= nnc)
                                    continue;
                                if (dist[nnr][nnc] != -1) continue;
                                future[nr][nc] += (state.grid[nr][nc] - 1) / 2;
                            }
                        }
                    }
                }
            }
            vector<tuple<double, int, int>> nxt;
            rep(r, N) {
                rep(c, N) {
                    if (state.grid[r][c] > 0 && dist[r][c] >= 0) {
                        future[r][c] = max(future[r][c], 0.0);
                        nxt.emplace_back(numBlock > 10
                                             ? (pow(state.grid[r][c] - 1, 2) +
                                                future[r][c]) /
                                                   pow(dist[r][c], 3)
                                             : (N * N - state.moves.size() -
                                                dist[r][c] + 1) *
                                                   (state.grid[r][c] - 1),
                                         r, c);
                    }
                }
            }
            sort(ALL(nxt), [](const tuple<double, int, int>& a,
                              const tuple<double, int, int>& b) {
                return get<0>(a) > get<0>(b);
            });
            for (int i = 0; i < min(BEAMSIZE, (int)nxt.size()); i++) {
                double score = get<0>(nxt[i]);
                int r = get<1>(nxt[i]), c = get<2>(nxt[i]);
                if (state.moves.size() + dist[r][c] > N * N) continue;
                if (beamQue[state.moves.size() + dist[r][c]].size() >=
                    BEAMSIZE) {
                    if (beamQue[state.moves.size() + dist[r][c]]
                            .get_min()
                            .score < state.score + score) {
                        beamQue[state.moves.size() + dist[r][c]].pop_min();
                    } else {
                        break;
                    }
                }
                State candState = state;
                candState.score += score;
                candState.rawscore +=
                    (N * N - state.moves.size() - dist[r][c] + 1) *
                    (state.grid[r][c] - 1);
                candState.grid[r][c] = 0;
                while (dist[r][c] > 0) {
                    int nr = to[r][c].first, nc = to[r][c].second;
                    candState.moves.emplace_back(makeMove(r, c, nr, nc));
                    r = nr;
                    c = nc;
                }
                beamQue[candState.moves.size()].push(candState);
                if (candState.rawscore > bestState.rawscore) {
                    bestState = candState;
                }
            }
        }
    }
    return bestState;
}

void input() {
    cin >> N >> C >> H;
    grid.resize(N, vector<int>(N));
    rep(i, N) {
        rep(j, N) {
            cin >> grid[i][j];
        }
    }
}

void output(State& state) {
    cout << state.moves.size() << endl;
    for (Move move : state.moves) {
        cout << move.r << " " << move.c << " " << move.move << " " << move.dir
             << endl;
    }
}

signed main() {
    startTime = tmr.getTime();
    input();
    State state = solve();
    output(state);
    return 0;
}