#include "graph.h"
#include "mst.h"
#include "search.h"
#include "course_path.h"

/* ============================================================
 * main.cpp
 *  - 자료구조 과제 #5: 그래프의 자료구조 이해와 활용
 *
 *  실행 시나리오
 *    Problem 1 : Prim MST
 *    Problem 2 : Kruskal MST
 *    Problem 3 : BFS / DFS 탐색
 *    Problem 4 : Connected Component
 *    Problem 5 : 영역별 최소 이수 학점 경로 (선후수 그래프)
 *
 *  옵션
 *    -i / --interactive : 사용자 시작 정점 입력 모드
 *    -n <int>           : 정점 수 변경(기본 50)
 *    -d <double>        : 간선 밀도 변경(기본 0.6)
 *    -s <int>           : 난수 시드 변경(기본 42)
 * ============================================================ */

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <string>

/* ---- Problem 1 & 2: MST ---- */
static void testMST(const Graph &g) {
    puts("========================================");
    puts(" Problem 1 & 2 : Minimum Spanning Tree");
    puts("========================================");
    g.printSummary();
    putchar('\n');

    MSTResult primR = prim(g, 0);
    printMSTResult("Prim (시작 정점 = 0)", primR);
    putchar('\n');

    MSTResult krusR = kruskal(g);
    printMSTResult("Kruskal", krusR);
    putchar('\n');

    /* 일치성 검증 */
    bool same = (primR.totalCost == krusR.totalCost);
    printf("  >> Prim/Kruskal 총 가중치 일치 여부 : %s\n",
           same ? "OK (동일 - 가중치 합)" : "WARN (다른 MST가 존재할 수 있음)");

    /* 스패닝 트리 판정 함수 검증 */
    printf("  >> isSpanningTree(Prim)    : %s\n",
           isSpanningTree(g, primR.edges) ? "YES" : "NO");
    printf("  >> isSpanningTree(Kruskal) : %s\n",
           isSpanningTree(g, krusR.edges) ? "YES" : "NO");
    putchar('\n');
}

/* ---- Problem 3: BFS / DFS ---- */
static void testSearch(const Graph &g, int start) {
    puts("========================================");
    puts(" Problem 3 : Graph Search (BFS / DFS)");
    puts("========================================");
    printf("  - 시작 정점         : %d\n\n", start);

    SearchResult b = bfs(g, start);
    printSearchResult("BFS", g, b);
    putchar('\n');

    SearchResult d = dfs(g, start);
    printSearchResult("DFS", g, d);
    putchar('\n');
}

/* ---- Problem 4: Connected Component ---- */
static void testConnectedComponent() {
    puts("========================================");
    puts(" Problem 4 : Connected Component");
    puts("========================================");

    /* 강의 자료의 비연결 그래프 예제(11 노드, 두 컴포넌트)를 재현 */
    Graph g(11, false);
    g.addEdge(0, 1);          /* 1-2 */
    g.addEdge(0, 3);          /* 1-4 */
    g.addEdge(1, 4);          /* 2-5 */
    g.addEdge(2, 4);          /* 3-5 */
    g.addEdge(3, 5);          /* 4-6 */
    g.addEdge(4, 6);          /* 5-7 */
    g.addEdge(5, 6);          /* 6-7 */
    /* 두 번째 컴포넌트 */
    g.addEdge(7, 8);          /* 8-9 */
    g.addEdge(7, 10);         /* 8-11 */
    g.addEdge(9, 10);         /* 10-11 */

    g.printSummary();
    putchar('\n');

    auto comps = connectedComponents(g);
    printComponents(comps);
    printf("\n  - 연결 그래프 여부  : %s\n",
           isConnected(g) ? "YES" : "NO");
    putchar('\n');

    puts("  [임의 생성 그래프 (Problem 1-3 사용)도 함께 확인]");
    /* 임의 생성 그래프는 spanning chain으로 보강되어 1개 컴포넌트 보장 */
}

/* ---- Problem 5 ---- */
static void testCoursePlan() {
    puts("========================================");
    puts(" Problem 5 : 영역별 이수 학점을 만족하는 교과목 이수 경로");
    puts("========================================");
    runCoursePlanDemo();
    putchar('\n');
}

/* ---- main ---- */
int main(int argc, char *argv[]) {
    int        n           = 50;
    double     density     = 0.60;
    unsigned   seed        = 42;
    bool       interactive = false;

    /* 옵션 파싱 */
    for (int i = 1; i < argc; ++i) {
        if (std::strcmp(argv[i], "-i") == 0 ||
            std::strcmp(argv[i], "--interactive") == 0) {
            interactive = true;
        } else if (std::strcmp(argv[i], "-n") == 0 && i + 1 < argc) {
            n = std::atoi(argv[++i]);
        } else if (std::strcmp(argv[i], "-d") == 0 && i + 1 < argc) {
            density = std::atof(argv[++i]);
        } else if (std::strcmp(argv[i], "-s") == 0 && i + 1 < argc) {
            seed = (unsigned)std::atoi(argv[++i]);
        }
    }
    if (n < 2) n = 2;
    if (density < 0.0) density = 0.0;
    if (density > 1.0) density = 1.0;

    puts("============================================================");
    puts(" 자료구조 과제 #5 : 그래프의 자료구조 이해와 활용");
    if (interactive) puts(" (대화형 모드: 사용자 입력 활성화)");
    puts("============================================================\n");

    /* 임의 그래프 생성 (무방향, 가중치 1~100) */
    Graph g;
    g.generateRandom(n, density, 1, 100, seed, /*directed=*/false);

    puts("  [생성된 임의 그래프]");
    g.printSummary();
    putchar('\n');
    puts("  [인접 리스트 (앞 10행만 표시)]");
    g.printAdjList(10);
    putchar('\n');

    /* Problem 1 & 2 */
    testMST(g);

    /* Problem 3 */
    int startVertex = 0;
    if (interactive) {
        printf("BFS/DFS 시작 정점 입력 (0 ~ %d) > ", n - 1);
        fflush(stdout);
        std::string line;
        if (std::getline(std::cin, line) && !line.empty()) {
            int v = std::atoi(line.c_str());
            if (v >= 0 && v < n) startVertex = v;
        }
        putchar('\n');
    }
    testSearch(g, startVertex);

    /* Problem 4 */
    testConnectedComponent();

    /* Problem 5 */
    testCoursePlan();

    return 0;
}
