
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

SOURCES = main.c graph.c data_loader.c \
          kruskal.c prim.c dijkstra.c knapsack.c \
          merge_sort.c quick_sort.c binary_search.c \
          max_subarray.c activity_loader.c \
          activity_selection.c interval_partitioning.c \
          huffman.c lcs.c bellman_ford.c \
          floyd_warshall.c tsp.c \
          subset_sum.c branch_bound.c \
          ford_fulkerson.c bipartite_matching.c \
          string_matching.c monitoring_search.c \
          scenario.c benchmark.c export.c

OBJECTS = $(SOURCES:.c=.o)
TARGET = wildlife

.PHONY: all run clean test

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	bash tests/smoke_test.sh

clean:
	rm -f $(OBJECTS) $(TARGET)
