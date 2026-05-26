#include "graph.h"

/* ============================================================
 * graph.cpp
 *  - Graph 클래스 구현
 *  - 인접 리스트 기반 표현
 *  - 임의 그래프 생성: 밀도(density) 기준 간선 무작위 선택
 * ============================================================ */

#include <cstdio>
#include <random>
#include <algorithm>

/* ---- 생성자 ---- */
Graph::Graph(int n, bool directed)
    : n_(n), directed_(directed), edgeCount_(0), adj_(n) {}

/* ---- 그래프 초기화 ---- */
void Graph::reset(int n, bool directed) {
    n_         = n;
    directed_  = directed;
    edgeCount_ = 0;
    adj_.assign(n, std::vector<AdjEntry>());
}

/* ---- 간선 추가 ---- */
void Graph::addEdge(int u, int v, int w) {
    if (u < 0 || v < 0 || u >= n_ || v >= n_) return;
    if (u == v) return;                       /* self-loop 제외 */
    adj_[u].push_back(AdjEntry(v, w));
    if (!directed_) adj_[v].push_back(AdjEntry(u, w));
    ++edgeCount_;
}

/* ---- 모든 간선 목록 반환 (중복 제거) ---- */
std::vector<Edge> Graph::allEdges() const {
    std::vector<Edge> es;
    es.reserve(edgeCount_);
    for (int u = 0; u < n_; ++u) {
        for (const AdjEntry &a : adj_[u]) {
            /* 무방향 그래프: u<v 인 경우만 한 번 등록 */
            if (!directed_ && u >= a.to) continue;
            es.push_back(Edge(u, a.to, a.w));
        }
    }
    return es;
}

/* ---- 임의 그래프 생성 ----
 *  - density: 완전 그래프 대비 간선 비율 (0.0 ~ 1.0)
 *  - 무방향 기준 최대 간선 수 = n*(n-1)/2
 *  - 가중치는 [minW, maxW] 균일 분포
 */
void Graph::generateRandom(int n, double density, int minW, int maxW,
                           unsigned seed, bool directed) {
    reset(n, directed);

    std::mt19937 rng(seed);
    std::uniform_int_distribution<int> wDist(minW, maxW);

    /* 모든 가능한 (u,v) 쌍을 순회하며 확률적으로 간선 추가 */
    std::uniform_real_distribution<double> pDist(0.0, 1.0);
    for (int u = 0; u < n; ++u) {
        int start = directed ? 0 : u + 1;
        for (int v = start; v < n; ++v) {
            if (u == v) continue;
            if (pDist(rng) < density) {
                addEdge(u, v, wDist(rng));
            }
        }
    }

    /* 무방향 그래프: 연결 그래프를 보장하기 위해 spanning chain 보강 */
    if (!directed) {
        std::vector<int> perm(n);
        for (int i = 0; i < n; ++i) perm[i] = i;
        std::shuffle(perm.begin(), perm.end(), rng);
        for (int i = 1; i < n; ++i) {
            int u = perm[i - 1], v = perm[i];
            /* 이미 인접한지 확인 */
            bool exists = false;
            for (const AdjEntry &a : adj_[u]) {
                if (a.to == v) { exists = true; break; }
            }
            if (!exists) addEdge(u, v, wDist(rng));
        }
    }
}

/* ---- 그래프 요약 출력 ---- */
void Graph::printSummary() const {
    int maxEdges = directed_ ? n_ * (n_ - 1) : n_ * (n_ - 1) / 2;
    double density = (maxEdges == 0) ? 0.0
                     : (double)edgeCount_ / maxEdges * 100.0;
    printf("  - 정점 수(|V|)      : %d\n", n_);
    printf("  - 간선 수(|E|)      : %d\n", edgeCount_);
    printf("  - 최대 간선 수      : %d\n", maxEdges);
    printf("  - 밀도(density)     : %.1f %%\n", density);
    printf("  - 방향 그래프       : %s\n", directed_ ? "yes" : "no");
}

/* ---- 인접 리스트 일부 출력 ---- */
void Graph::printAdjList(int maxRows) const {
    int rows = (n_ < maxRows) ? n_ : maxRows;
    for (int u = 0; u < rows; ++u) {
        printf("  adj[%2d] -> ", u);
        int cnt = 0;
        for (const AdjEntry &a : adj_[u]) {
            if (cnt++ > 8) { printf("..."); break; }
            printf("(%d,w=%d) ", a.to, a.w);
        }
        putchar('\n');
    }
    if (n_ > maxRows) printf("  ... (총 %d 행 중 %d 행만 표시)\n", n_, rows);
}
