#ifndef LABAC_GRAPH_H
#define LABAC_GRAPH_H
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct edge {
    int from;
    int to;
    int weight;
} Edge;

typedef struct graph {
    int vertex_count;
    int edge_count;
    int capacity;
    Edge* edges;
} Graph;

Graph* init_graph(void);
void destroy_graph(Graph* graph);
int add_vertex(Graph* graph);
int add_edge(Graph* graph, int from, int to, int weight);

int load_graph_from_stream(Graph* graph, FILE* stream);
int save_graph_to_stream(const Graph* graph, FILE* stream);

int load_graph_from_file(Graph* graph, const char* filename);
int save_graph_to_file(const Graph* graph, const char* filename);

#ifdef __cplusplus
}
#endif

#endif //LABAC_GRAPH_H
