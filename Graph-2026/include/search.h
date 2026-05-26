#ifndef SEARCH_H
#define SEARCH_H

/* ============================================================
 * search.h
 *  - 그래프 탐색 ADT 선언
 *
 *  Problem 3:
 *    - BFS(Breadth-First Search): FIFO Queue 기반 레벨 순회
 *    - DFS(Depth-First Search) : Stack(또는 재귀) 기반 깊이 우선
 *
 *  Problem 4:
 *    - Connected Component 찾기
 *    - 그래프 탐색을 반복 적용하여 연결요소를 분리
 * ============================================================ */

#include "graph.h"
#include <vector>

/* ---- 탐색 결과 구조체 ----
 *  - order     : 방문 순서대로 정점 인덱스
 *  - parent    : 탐색 트리(spanning tree)에서의 부모 정점 (없으면 -1)
 *  - treeEdges : 탐색이 만든 간선 집합(스패닝 트리 후보)
 *  - visited   : 정점별 방문 여부
 */
struct SearchResult {
    std::vector<int>  order;
    std::vector<int>  parent;
    std::vector<Edge> treeEdges;
    std::vector<bool> visited;
};

/* BFS: 시작 정점에서 도달 가능한 정점들을 너비 우선 방문 */
SearchResult bfs(const Graph &g, int start);

/* DFS: 시작 정점에서 도달 가능한 정점들을 깊이 우선 방문 (재귀) */
SearchResult dfs(const Graph &g, int start);

/* Connected Component: 그래프 전체를 탐색하여 연결요소 목록 반환 */
std::vector<std::vector<int>> connectedComponents(const Graph &g);

/* 전체 그래프가 하나의 연결 그래프(connected graph)인지 판정 */
bool isConnected(const Graph &g);

/* 탐색 결과 출력 (선택된 노드/경로, 노드/엣지 수) */
void printSearchResult(const char *title, const Graph &g,
                       const SearchResult &r, int maxOrder = 20);

/* 연결요소 결과 출력 */
void printComponents(const std::vector<std::vector<int>> &comps);

#endif /* SEARCH_H */
