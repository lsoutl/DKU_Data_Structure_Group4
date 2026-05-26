#include "mst.h"

/* ============================================================
 * mst.cpp
 *  - Prim / Kruskal MST 알고리즘 구현
 *  - 스패닝 트리 판정 함수 포함
 * ============================================================ */

#include <cstdio>
#include <queue>
#include <vector>
#include <algorithm>
#include <functional>
#include <climits>

/* ---- 내부 Union-Find ----
 *  - Kruskal 사이클 판정에 사용
 *  - 경로 압축(path compression)으로 효율 향상
 */
struct DSU {
    std::vector<int> parent;
    std::vector<int> rank_;

    DSU(int n) : parent(n), rank_(n, 0) {
        for (int i = 0; i < n; ++i) parent[i] = i;
    }

    /* 루트 찾기 (경로 압축) */
    int find(int x) {
        while (parent[x] != x) {
            parent[x] = parent[parent[x]];
            x = parent[x];
        }
        return x;
    }

    /* 두 집합을 합치고, 새 결합이 일어났는지 반환 */
    bool unite(int a, int b) {
        a = find(a); b = find(b);
        if (a == b) return false;
        if (rank_[a] < rank_[b]) std::swap(a, b);
        parent[b] = a;
        if (rank_[a] == rank_[b]) ++rank_[a];
        return true;
    }
};

/* ---- Prim 알고리즘 ----
 *  - 시작 정점에서 출발하여 트리 외부와 잇는 최소 가중치 간선을 선택
 *  - priority_queue<(weight, to, from)>로 후보 간선을 관리
 */
MSTResult prim(const Graph &g, int start) {
    int n = g.numVertices();
    MSTResult r;
    r.totalCost  = 0;
    r.vertices   = 0;
    r.isSpanning = false;
    if (n == 0) return r;

    std::vector<bool> inTree(n, false);

    /* 우선순위 큐 원소: (가중치, 새 정점, 부모 정점) */
    typedef std::tuple<int, int, int> PQItem;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

    inTree[start] = true;
    r.vertices    = 1;
    for (const AdjEntry &a : g.neighbors(start))
        pq.push(std::make_tuple(a.w, a.to, start));

    while (!pq.empty() && r.vertices < n) {
        int w, v, u;
        std::tie(w, v, u) = pq.top();
        pq.pop();
        if (inTree[v]) continue;             /* 사이클 형성 시 폐기 */

        inTree[v] = true;
        ++r.vertices;
        r.edges.push_back(Edge(u, v, w));
        r.totalCost += w;

        for (const AdjEntry &a : g.neighbors(v)) {
            if (!inTree[a.to])
                pq.push(std::make_tuple(a.w, a.to, v));
        }
    }

    r.isSpanning = (r.vertices == n);
    return r;
}

/* ---- Kruskal 알고리즘 ----
 *  - 모든 간선을 가중치 오름차순 정렬
 *  - Union-Find로 사이클 검사 후 채택
 */
MSTResult kruskal(const Graph &g) {
    int n = g.numVertices();
    MSTResult r;
    r.totalCost  = 0;
    r.vertices   = 0;
    r.isSpanning = false;

    std::vector<Edge> es = g.allEdges();
    std::sort(es.begin(), es.end(),
              [](const Edge &a, const Edge &b) { return a.w < b.w; });

    DSU dsu(n);
    int picked = 0;
    for (const Edge &e : es) {
        if (dsu.unite(e.u, e.v)) {
            r.edges.push_back(e);
            r.totalCost += e.w;
            if (++picked == n - 1) break;
        }
    }

    /* 트리 내 정점 수 집계 */
    std::vector<bool> seen(n, false);
    for (const Edge &e : r.edges) { seen[e.u] = seen[e.v] = true; }
    r.vertices = 0;
    for (int i = 0; i < n; ++i) if (seen[i]) ++r.vertices;

    r.isSpanning = (picked == n - 1);
    return r;
}

/* ---- 스패닝 트리 판정 ----
 *  - 조건 1: 간선 수 = |V| - 1
 *  - 조건 2: 사이클 없음 (Union-Find 검사)
 *  - 조건 3: 모든 정점이 한 연결요소에 포함
 */
bool isSpanningTree(const Graph &g, const std::vector<Edge> &treeEdges) {
    int n = g.numVertices();
    if ((int)treeEdges.size() != n - 1) return false;

    DSU dsu(n);
    for (const Edge &e : treeEdges) {
        if (!dsu.unite(e.u, e.v)) return false;   /* 사이클 발생 */
    }
    /* 한 연결요소 확인 */
    int root = dsu.find(0);
    for (int i = 1; i < n; ++i)
        if (dsu.find(i) != root) return false;
    return true;
}

/* ---- MST 결과 출력 ---- */
void printMSTResult(const char *title, const MSTResult &r, int maxEdges) {
    printf("  [%s]\n", title);
    printf("  - 선택된 간선 수    : %d\n", (int)r.edges.size());
    printf("  - 트리 내 정점 수   : %d\n", r.vertices);
    printf("  - 총 가중치(cost)   : %lld\n", r.totalCost);
    printf("  - 스패닝 트리 여부  : %s\n",
           r.isSpanning ? "YES (모든 정점을 덮음)" : "NO");
    printf("  - 선택된 간선 일부  :\n");

    int show = ((int)r.edges.size() < maxEdges) ? (int)r.edges.size() : maxEdges;
    for (int i = 0; i < show; ++i) {
        const Edge &e = r.edges[i];
        printf("      (%2d - %2d)  weight=%d\n", e.u, e.v, e.w);
    }
    if ((int)r.edges.size() > maxEdges)
        printf("      ... (총 %d개 중 %d개만 표시)\n",
               (int)r.edges.size(), show);
}
