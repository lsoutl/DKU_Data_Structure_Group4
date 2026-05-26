#ifndef GRAPH_H
#define GRAPH_H

/* ============================================================
 * graph.h
 *  - 가중 그래프 ADT 선언
 *  - 표현: 인접 리스트(adjacency list) 기반
 *  - 무방향(undirected) / 방향(directed) 그래프 모두 지원
 *  - 가중치 정수형(int) 사용
 * ============================================================ */

#include <vector>
#include <string>

/* ---- 간선(edge) 구조체 ----
 *  - u : 시작 정점
 *  - v : 도착 정점
 *  - w : 가중치
 */
struct Edge {
    int u;
    int v;
    int w;
    Edge(int a = 0, int b = 0, int c = 0) : u(a), v(b), w(c) {}
};

/* ---- 인접 항목(adjacency entry) ----
 *  - to : 인접 정점
 *  - w  : 간선 가중치
 */
struct AdjEntry {
    int to;
    int w;
    AdjEntry(int t = 0, int c = 0) : to(t), w(c) {}
};

/* ---- 그래프 클래스 ---- */
class Graph {
public:
    Graph(int n = 0, bool directed = false);

    /* 정점 / 간선 수 변경 */
    void reset(int n, bool directed);

    /* 간선 추가 (무방향 그래프는 양방향 자동 등록) */
    void addEdge(int u, int v, int w = 1);

    /* 기본 조회 */
    int  numVertices() const { return n_; }
    int  numEdges()    const { return edgeCount_; }
    bool isDirected()  const { return directed_; }

    /* 인접 정점 조회 */
    const std::vector<AdjEntry>& neighbors(int u) const { return adj_[u]; }

    /* 모든 간선을 한 번씩만 반환 (MST 정렬 용) */
    std::vector<Edge> allEdges() const;

    /* 임의 그래프 생성 (시드 고정으로 재현 가능) */
    void generateRandom(int n, double density, int minW, int maxW,
                        unsigned seed = 42, bool directed = false);

    /* 그래프 정보 출력 */
    void printSummary() const;
    void printAdjList(int maxRows = 10) const;

private:
    int  n_;
    bool directed_;
    int  edgeCount_;
    std::vector<std::vector<AdjEntry>> adj_;
};

#endif /* GRAPH_H */
