#
# Makefile - Graph-2026 과제 (HW5)
#  - 그래프 ADT, Prim/Kruskal MST, BFS/DFS, Connected Component, 선후수 경로
#

CXX    = g++
FLAGS  = -Wall -O2 -std=c++11 -Iinclude

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

TARGET = $(BIN_DIR)/graph

OBJS = $(OBJ_DIR)/graph.o \
       $(OBJ_DIR)/mst.o   \
       $(OBJ_DIR)/search.o \
       $(OBJ_DIR)/course_path.o \
       $(OBJ_DIR)/main.o

# ---- 빌드 ----
all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(FLAGS) -o $(TARGET) $(OBJS)

# ---- 개별 컴파일 규칙 ----
$(OBJ_DIR)/graph.o: $(SRC_DIR)/graph.cpp include/graph.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/mst.o: $(SRC_DIR)/mst.cpp include/mst.h include/graph.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/search.o: $(SRC_DIR)/search.cpp include/search.h include/graph.h include/mst.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/course_path.o: $(SRC_DIR)/course_path.cpp include/course_path.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.cpp include/graph.h include/mst.h include/search.h include/course_path.h
	@mkdir -p $(OBJ_DIR)
	$(CXX) $(FLAGS) -c $< -o $@

# ---- 실행 ----
run: all
	./$(TARGET)

# ---- 정리 ----
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

.PHONY: all run clean
