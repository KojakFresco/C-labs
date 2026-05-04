#ifndef LABAC_DIJKSTRA_H
#define LABAC_DIJKSTRA_H
#include "graph.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int to;
    long long weight;
} AdjacencyNode;

typedef struct {
    AdjacencyNode** adjacency_lists;
    int* list_sizes;
    int* list_capacities;
} AdjacencyList;

long long* dijkstra(const Graph* graph, int vertex);

#ifdef __cplusplus
}
#endif

#endif //LABAC_DIJKSTRA_H
