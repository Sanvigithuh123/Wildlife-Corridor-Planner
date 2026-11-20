
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -O2

SOURCES = main.c graph.c data_loader.c \
          kruskal.c prim.c dijkstra.c knapsack.c \
          merge_sort.c quick_sort.c binary_search.c \
          activity_loader.c activity_selection.c \
          bellman_ford.c branch_bound.c \
          ford_fulkerson.c bipartite_matching.c \
          scenario.c benchmark.c export.c priority.c recommendations.c

OBJECTS = $(SOURCES:.c=.o)
LIBOBJECTS = $(filter-out main.o,$(OBJECTS))
TARGET = wildlife

.PHONY: all run clean test

all: $(TARGET) libwildlife.a

$(TARGET): main.o libwildlife.a
	$(CC) $(CFLAGS) main.o -L. -lwildlife -o $(TARGET)

libwildlife.a: $(LIBOBJECTS)
	ar rcs $@ $(LIBOBJECTS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

run: $(TARGET)
	./$(TARGET)

test: $(TARGET)
	bash tests/smoke_test.sh

clean:
	rm -f $(OBJECTS) $(TARGET) libwildlife.a
