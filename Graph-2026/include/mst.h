#ifndef MST_H
#define MST_H

/* ============================================================
 * mst.h
 *  - 최소 길이 스패닝 트리(Minimum Spanning Tree) ADT 선언
 *
 *  Problem 1: Prim 알고리즘
 *    - 시작 정점부터 인접 최소 가중치 간선을 그리디로 확장
 *    - 자료구조: 최소 힙(priority_queue)
 *    - 시간복잡도: O((V+E) log V)
 *
 *  Problem 2: Kruskal 알고리즘
 *    - 모든 간선을 가중치 오름차순 정렬 후 사이클 안 만드는 간선 채택
 *    - 자료구조: Union-Find (Disjoint Set)
 *    - 시간복잡도: O(E log E)
 * ============================================================ */

#include "graph.h"
#include <vector>

/* ---- MST 결과 구조체 ----
 *  - edges      : 선택된 트리 간선들
 *  - totalCost  : 선택된 간선 가중치 합
 *  - vertices   : 트리에 포함된 정점 수
 *  - isSpanning : 모든 정점을 덮는 스패닝 트리인지 여부
 */
struct MSTResult {
    std::vector<Edge> edges;
    long long         totalCost;
    int               vertices;
    bool              isSpanning;
};

/* Prim 알고리즘으로 MST 계산 */
MSTResult prim(const Graph &g, int start = 0);

/* Kruskal 알고리즘으로 MST 계산 */
MSTResult kruskal(const Graph &g);

/* 주어진 간선 집합이 그래프 g의 스패닝 트리인지 판정 */
bool isSpanningTree(const Graph &g, const std::vector<Edge> &treeEdges);

/* MST 결과를 표 형태로 출력 */
void printMSTResult(const char *title, const MSTResult &r, int maxEdges = 20);

#endif /* MST_H */
