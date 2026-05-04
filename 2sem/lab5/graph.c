#include "graph.h"

#include <stdlib.h>

static int _ensure_edge_capacity(Graph* graph, int needed) {
    if (graph->capacity >= needed) {
        return 0;
    }

    int new_capacity = graph->capacity;
    while (new_capacity < needed) {
        new_capacity *= 2;
    }

    Edge* new_edges = realloc(graph->edges, sizeof(Edge) * new_capacity);
    if (new_edges == NULL) {
        return -1;
    }

    graph->edges = new_edges;
    graph->capacity = new_capacity;
    return 0;
}

Graph* init_graph(void) {
    Graph* graph = malloc(sizeof(Graph));
    if (graph == NULL) {
        fprintf(stderr, "Error allocating memory\n");
        return NULL;
    }
    graph->vertex_count = 0;
    graph->edge_count = 0;
    graph->edges = malloc(sizeof(Edge) * 2);
    if (graph->edges == NULL) {
        free(graph);
        fprintf(stderr, "Error allocating memory for the edges\n");
        return NULL;
    }
    graph->capacity = 2;
    return graph;
}

void destroy_graph(Graph* graph) {
    if (graph == NULL) {
        return;
    }
    if (graph->edges != NULL) {
        free(graph->edges);
    }
    free(graph);
}

int add_vertex(Graph* graph) {
    if (graph == NULL) {
        return -1;
    }
    graph->vertex_count++;
    return 0;
}

int add_edge(Graph* graph, int from, int to, int weight) {
    if (graph == NULL || from < 0 || to < 0) {
        return -1;
    }

    if (_ensure_edge_capacity(graph, graph->edge_count + 1) != 0) {
        return -1;
    }

    graph->edges[graph->edge_count].from = from;
    graph->edges[graph->edge_count].to = to;
    graph->edges[graph->edge_count].weight = weight;
    graph->edge_count++;

    if (graph->vertex_count < from + 1) {
        graph->vertex_count = from + 1;
    }
    if (graph->vertex_count < to + 1) {
        graph->vertex_count = to + 1;
    }

    return 0;
}

int load_graph_from_stream(Graph* graph, FILE* stream) {
    if (stream == NULL || graph == NULL) {
        return -1;
    }
    if (fscanf(stream, "%d %d", &graph->vertex_count, &graph->edge_count) != 2) {
        return -1;
    }
    if (graph->edge_count > graph->capacity) {
        if (_ensure_edge_capacity(graph, graph->edge_count) != 0) {
            return -1;
        }
    }

    int from, to, weight;
    for (int i = 0; i < graph->edge_count; i++) {
        if (fscanf(stream, "%d %d %d", &from, &to, &weight) != 3) {
            return -1;
        }
        graph->edges[i].from = from;
        graph->edges[i].to = to;
        graph->edges[i].weight = weight;
    }
    return 0;
}

int save_graph_to_stream(const Graph* graph, FILE* stream) {
    if (stream == NULL || graph == NULL) {
        return -1;
    }

    fprintf(stream, "%d %d\n", graph->vertex_count, graph->edge_count);

    for (int i = 0; i < graph->edge_count; i++) {
        fprintf(stream, "%d %d %d\n", graph->edges[i].from, graph->edges[i].to, graph->edges[i].weight);
    }
    return 0;
}

int load_graph_from_file(Graph* graph, const char* filename) {
    FILE* stream = fopen(filename, "r");
    if (stream == NULL) {
        return -1;
    }
    if (load_graph_from_stream(graph, stream) != 0) {
        fclose(stream);
        return -1;
    }
    fclose(stream);
    return 0;
}

int save_graph_to_file(const Graph* graph, const char* filename) {
    FILE* stream = fopen(filename, "w");
    if (stream == NULL) {
        return -1;
    }
    if (save_graph_to_stream(graph, stream) != 0) {
        fclose(stream);
        return -1;
    }
    fclose(stream);
    return 0;
}