#ifndef COURSE_PATH_H
#define COURSE_PATH_H

/* ============================================================
 * course_path.h
 *  - Problem 5: 영역별 이수 체계에 따른 선후수 교과목 이수 경로
 *
 *  설계 개요
 *   - 정점(vertex)         = 교과목
 *   - 방향 간선(u -> v)    = u가 v의 선수 과목
 *   - 교과목 속성          = 영역(area), 학점(credit)
 *   - 목표                 = 영역별 최소 이수 학점을 만족하는
 *                            교과목 부분집합과 이수 순서 산출
 *
 *  핵심 알고리즘
 *   1) 위상 정렬(Topological sort)로 가능한 이수 순서 후보 생성
 *   2) DFS + 백트래킹으로 영역별 학점 제약 만족 여부 탐색
 *   3) 선택된 교과목들의 부분 그래프에서 BFS로 실제 이수 경로 출력
 *
 *  참고
 *   - 본 모듈은 설계 방법을 보이는 데 목적이 있으므로
 *     비교적 작은 예제 데이터로 동작을 시연한다.
 * ============================================================ */

#include <string>
#include <vector>

/* ---- 교과목 정보 구조체 ----
 *  - id     : 정점 인덱스
 *  - name   : 교과목명
 *  - area   : 이수 영역
 *  - credit : 학점
 */
struct Course {
    int         id;
    std::string name;
    std::string area;
    int         credit;
};

/* 선후수 그래프와 영역별 최소 학점을 입력으로
 * 영역별 학점을 만족하는 한 가지 이수 경로를 찾아 출력 */
void runCoursePlanDemo();

#endif /* COURSE_PATH_H */
