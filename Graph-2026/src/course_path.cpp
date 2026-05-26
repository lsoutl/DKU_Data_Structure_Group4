#include "course_path.h"

/* ============================================================
 * course_path.cpp
 *  - Problem 5 구현
 *  - 영역별 최소 이수 학점 + 선후수 제약을 만족하는 교과목
 *    이수 경로(순서)를 위상정렬 + 백트래킹으로 탐색
 * ============================================================ */

#include <cstdio>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>

/* ---- 데모 데이터 정의 ----
 *  - 영역: BASIC(교양), CS(전공기초), AI(응용), SE(공학), MATH(수학)
 *  - 선후수 관계는 (prereq, course) 쌍으로 표현
 */
static std::vector<Course> buildSampleCourses() {
    return {
        { 0, "Calculus-I",       "MATH",  3},
        { 1, "Calculus-II",      "MATH",  3},
        { 2, "Linear-Algebra",   "MATH",  3},
        { 3, "Discrete-Math",    "MATH",  3},
        { 4, "Programming-I",    "BASIC", 3},
        { 5, "Programming-II",   "BASIC", 3},
        { 6, "Data-Structure",   "CS",    3},
        { 7, "Algorithm",        "CS",    3},
        { 8, "OS",               "CS",    3},
        { 9, "Database",         "CS",    3},
        {10, "AI-Intro",         "AI",    3},
        {11, "Machine-Learning", "AI",    3},
        {12, "Deep-Learning",    "AI",    3},
        {13, "Software-Eng",     "SE",    3},
        {14, "System-Design",    "SE",    3},
    };
}

/* 선수 → 후수 방향 간선 목록 */
static std::vector<std::pair<int,int>> buildPrereqs() {
    return {
        {0, 1},   /* Cal-I  -> Cal-II  */
        {1, 2},   /* Cal-II -> Linear  */
        {4, 5},   /* Prog-I -> Prog-II */
        {5, 6},   /* Prog-II-> DS      */
        {6, 7},   /* DS     -> Algo    */
        {6, 8},   /* DS     -> OS      */
        {6, 9},   /* DS     -> DB      */
        {7,10},   /* Algo   -> AI      */
        {2,11},   /* Linear -> ML      */
        {10,11},  /* AI     -> ML      */
        {11,12},  /* ML     -> DL      */
        {6,13},   /* DS     -> SE      */
        {13,14},  /* SE     -> SysDes  */
    };
}

/* ---- 위상 정렬 ----
 *  - in-degree 0인 정점부터 차례로 추출
 *  - 사이클이 있으면 빈 결과 반환
 */
static std::vector<int> topoSort(int n,
                                 const std::vector<std::vector<int>> &adj) {
    std::vector<int> indeg(n, 0);
    for (int u = 0; u < n; ++u)
        for (int v : adj[u]) ++indeg[v];

    std::queue<int> q;
    for (int i = 0; i < n; ++i) if (indeg[i] == 0) q.push(i);

    std::vector<int> order;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (--indeg[v] == 0) q.push(v);
        }
    }
    return ((int)order.size() == n) ? order : std::vector<int>();
}

/* ---- 백트래킹: 영역별 최소 학점을 만족하는 부분집합 탐색 ----
 *  - 위상 순서대로 한 과목씩 (수강/미수강) 분기
 *  - 선수 과목이 모두 수강된 경우에만 "수강" 분기 시도
 *  - 영역별 학점이 목표를 만족하면 종료
 */
static bool dfsSelect(int idx,
                      const std::vector<int> &order,
                      const std::vector<Course> &courses,
                      const std::vector<std::vector<int>> &prereqOf,
                      const std::map<std::string,int> &needed,
                      std::map<std::string,int> &cur,
                      std::vector<bool> &take) {
    /* 목표 달성 여부 검사 */
    bool done = true;
    for (auto &p : needed) {
        auto it = cur.find(p.first);
        int got = (it == cur.end()) ? 0 : it->second;
        if (got < p.second) { done = false; break; }
    }
    if (done) return true;

    if (idx >= (int)order.size()) return false;

    int cid         = order[idx];
    const Course &c = courses[cid];

    /* 선수 과목이 모두 수강되었는지 확인 */
    bool prereqOk = true;
    for (int p : prereqOf[cid]) {
        if (!take[p]) { prereqOk = false; break; }
    }

    /* (1) 이 과목을 수강하는 경우 (선수 충족 시만 시도) */
    if (prereqOk) {
        take[cid] = true;
        cur[c.area] += c.credit;
        if (dfsSelect(idx + 1, order, courses, prereqOf, needed, cur, take))
            return true;
        cur[c.area] -= c.credit;
        take[cid] = false;
    }

    /* (2) 이 과목을 수강하지 않는 경우 */
    if (dfsSelect(idx + 1, order, courses, prereqOf, needed, cur, take))
        return true;
    return false;
}

/* ---- 데모 실행 ---- */
void runCoursePlanDemo() {
    std::vector<Course> courses = buildSampleCourses();
    std::vector<std::pair<int,int>> prereqs = buildPrereqs();
    int n = (int)courses.size();

    /* 인접 리스트(방향 그래프) 구성
     *  - adj[u]      : u의 후수 과목 목록 (u -> v)
     *  - prereqOf[v] : v의 선수 과목 목록
     */
    std::vector<std::vector<int>> adj(n);
    std::vector<std::vector<int>> prereqOf(n);
    for (auto &p : prereqs) {
        adj[p.first].push_back(p.second);
        prereqOf[p.second].push_back(p.first);
    }

    /* 영역별 최소 이수 학점(예시) */
    std::map<std::string,int> needed = {
        {"BASIC", 6},
        {"MATH",  6},
        {"CS",    9},
        {"AI",    6},
        {"SE",    3},
    };

    printf("  - 전체 교과목 수    : %d\n", n);
    printf("  - 선후수 관계 수    : %d\n", (int)prereqs.size());
    printf("  - 영역별 최소 학점  :\n");
    for (auto &p : needed)
        printf("      %-7s >= %d 학점\n", p.first.c_str(), p.second);

    /* (1) 위상 정렬: 가능한 이수 순서 */
    std::vector<int> topo = topoSort(n, adj);
    if (topo.empty()) {
        printf("  사이클이 존재하여 이수 순서를 만들 수 없음\n");
        return;
    }
    printf("  - 위상정렬 순서     : ");
    for (int v : topo) printf("%s ", courses[v].name.c_str());
    putchar('\n');

    /* (2) 백트래킹 탐색 */
    std::vector<bool> take(n, false);
    std::map<std::string,int> cur;
    bool ok = dfsSelect(0, topo, courses, prereqOf, needed, cur, take);
    if (!ok) {
        printf("  - 결과              : 영역별 최소 학점을 만족하는 조합 없음\n");
        return;
    }

    /* (3) 선택 결과 출력 */
    printf("  - 결과              : 이수 경로 발견\n");
    int totalCredit = 0;
    printf("  - 선택된 교과목     :\n");
    for (int v : topo) {
        if (take[v]) {
            printf("      [%s] %-18s (%d학점)\n",
                   courses[v].area.c_str(),
                   courses[v].name.c_str(),
                   courses[v].credit);
            totalCredit += courses[v].credit;
        }
    }
    printf("  - 총 이수 학점      : %d\n", totalCredit);

    printf("  - 영역별 이수 학점  :\n");
    for (auto &p : cur) {
        printf("      %-7s : %d / %d 학점\n",
               p.first.c_str(), p.second, needed[p.first]);
    }
}
