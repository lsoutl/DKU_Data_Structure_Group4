#include "search.h"
#include "mst.h"

/* ============================================================
 * search.cpp
 *  - BFS / DFS / Connected Component 구현
 *  - 각 탐색은 spanning tree 후보 간선을 함께 생성
 * ============================================================ */

#include <cstdio>
#include <queue>
#include <stack>
#include <vector>

/* ---- BFS ----
 *  - 시작 정점 enqueue → dequeue 시 인접 정점 방문 → enqueue
 *  - 큐가 비면 종료
 */
SearchResult bfs(const Graph &g, int start) {
    int n = g.numVertices();
    SearchResult r;
    r.parent.assign(n, -1);
    r.visited.assign(n, false);
    if (n == 0 || start < 0 || start >= n) return r;

    std::queue<int> q;
    r.visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        r.order.push_back(u);

        for (const AdjEntry &a : g.neighbors(u)) {
            if (!r.visited[a.to]) {
                r.visited[a.to] = true;
                r.parent[a.to] = u;
                r.treeEdges.push_back(Edge(u, a.to, a.w));
                q.push(a.to);
            }
        }
    }
    return r;
}

/* ---- DFS 보조 ----
 *  - 재귀 호출로 깊이 우선 방문
 *  - tree edge를 함께 기록
 */
static void dfsVisit(const Graph &g, int u, SearchResult &r) {
    r.visited[u] = true;
    r.order.push_back(u);
    for (const AdjEntry &a : g.neighbors(u)) {
        if (!r.visited[a.to]) {
            r.parent[a.to] = u;
            r.treeEdges.push_back(Edge(u, a.to, a.w));
            dfsVisit(g, a.to, r);
        }
    }
}

/* ---- DFS ----
 *  - 시작 정점부터 재귀로 깊이 우선 탐색
 */
SearchResult dfs(const Graph &g, int start) {
    int n = g.numVertices();
    SearchResult r;
    r.parent.assign(n, -1);
    r.visited.assign(n, false);
    if (n == 0 || start < 0 || start >= n) return r;

    dfsVisit(g, start, r);
    return r;
}

/* ---- Connected Component ----
 *  - 미방문 정점이 남아있으면 BFS로 새 연결요소 추출
 *  - 결과: 각 연결요소의 정점 인덱스 리스트
 */
std::vector<std::vector<int>> connectedComponents(const Graph &g) {
    int n = g.numVertices();
    std::vector<std::vector<int>> comps;
    std::vector<bool> visited(n, false);

    for (int s = 0; s < n; ++s) {
        if (visited[s]) continue;
        /* BFS로 한 연결요소 추출 */
        std::vector<int> comp;
        std::queue<int> q;
        visited[s] = true;
        q.push(s);
        while (!q.empty()) {
            int u = q.front(); q.pop();
            comp.push_back(u);
            for (const AdjEntry &a : g.neighbors(u)) {
                if (!visited[a.to]) {
                    visited[a.to] = true;
                    q.push(a.to);
                }
            }
        }
        comps.push_back(comp);
    }
    return comps;
}

/* ---- 연결 그래프 판정 ----
 *  - 연결요소 수가 1이면 connected graph
 */
bool isConnected(const Graph &g) {
    if (g.numVertices() == 0) return true;
    return connectedComponents(g).size() == 1;
}

/* ---- 탐색 결과 출력 ----
 *  - 선택된 노드/경로, 노드 수, 엣지 수, spanning tree 여부 출력
 */
void printSearchResult(const char *title, const Graph &g,
                       const SearchResult &r, int maxOrder) {
    int visitedCnt = 0;
    for (bool b : r.visited) if (b) ++visitedCnt;

    printf("  [%s]\n", title);
    printf("  - 방문한 정점 수    : %d / %d\n", visitedCnt, g.numVertices());
    printf("  - 트리 간선 수      : %d\n", (int)r.treeEdges.size());

    /* spanning tree 판단:
     *   - 그래프가 연결되어 있고
     *   - 트리 간선 수 = |V| - 1 이면 spanning tree */
    bool spanning = ((int)r.treeEdges.size() == g.numVertices() - 1)
                    && isSpanningTree(g, r.treeEdges);
    printf("  - 스패닝 트리 여부  : %s\n",
           spanning ? "YES" : "NO (그래프가 비연결이거나 누락된 정점 존재)");

    printf("  - 방문 순서(앞부분) : ");
    int show = ((int)r.order.size() < maxOrder) ? (int)r.order.size() : maxOrder;
    for (int i = 0; i < show; ++i) {
        printf("%d", r.order[i]);
        if (i + 1 < show) printf(" -> ");
    }
    if ((int)r.order.size() > maxOrder) printf(" ...");
    putchar('\n');

    /* 탐색 트리(부모 관계) 일부 출력 */
    printf("  - 트리 간선(앞부분) : ");
    int eShow = ((int)r.treeEdges.size() < 10) ? (int)r.treeEdges.size() : 10;
    for (int i = 0; i < eShow; ++i) {
        const Edge &e = r.treeEdges[i];
        printf("(%d->%d) ", e.u, e.v);
    }
    if ((int)r.treeEdges.size() > 10) printf("...");
    putchar('\n');
}

/* ---- 연결요소 출력 ---- */
void printComponents(const std::vector<std::vector<int>> &comps) {
    printf("  - 연결요소 수       : %d\n", (int)comps.size());
    for (size_t i = 0; i < comps.size(); ++i) {
        printf("    component #%zu (크기 %zu): ", i + 1, comps[i].size());
        int show = ((int)comps[i].size() < 30) ? (int)comps[i].size() : 30;
        for (int j = 0; j < show; ++j) {
            printf("%d ", comps[i][j]);
        }
        if ((int)comps[i].size() > 30) printf("...");
        putchar('\n');
    }
}
