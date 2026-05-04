#include "dijkstra.h"
#include "priority_queue.h"

#include <stdlib.h>
#include <limits.h>

typedef long long ll;

void free_adjacency_list(AdjacencyList* adjacency_list, int vertex_count) {
    if (adjacency_list == NULL) {
        return;
    }
    if (adjacency_list->adjacency_lists != NULL) {
        for (int i = 0; i < vertex_count; i++) {
            if (adjacency_list->adjacency_lists[i] != NULL) free(adjacency_list->adjacency_lists[i]);
        }
        free(adjacency_list->adjacency_lists);
    }
    free(adjacency_list->list_capacities);
    free(adjacency_list->list_sizes);
    free(adjacency_list);
}

static AdjacencyList* build_adjacency_list(const Graph* graph) {
    if (graph == NULL) {
        return NULL;
    }
    AdjacencyList* adjacency_list = malloc(sizeof(AdjacencyList));
    if (adjacency_list == NULL) {
        return NULL;
    }

    adjacency_list->adjacency_lists = malloc(graph->vertex_count * sizeof(AdjacencyNode*));
    if (adjacency_list->adjacency_lists == NULL) {
        free(adjacency_list);
        return NULL;
    }
    adjacency_list->list_sizes = malloc(graph->vertex_count * sizeof(int));
    if (adjacency_list->list_sizes == NULL) {
        free(adjacency_list->adjacency_lists);
        free(adjacency_list);
        return NULL;
    }
    adjacency_list->list_capacities = malloc(graph->vertex_count * sizeof(int));
    if (adjacency_list->list_capacities == NULL) {
        free(adjacency_list->list_sizes);
        free(adjacency_list->adjacency_lists);
        free(adjacency_list);
        return NULL;
    }

    for (int i = 0; i < graph->vertex_count; i++) {
        adjacency_list->adjacency_lists[i] = malloc(sizeof(AdjacencyNode) * 2);
        if (adjacency_list->adjacency_lists[i] == NULL) {
            free_adjacency_list(adjacency_list, i);
            return NULL;
        }
        adjacency_list->list_sizes[i] = 0;
        adjacency_list->list_capacities[i] = 2;
    }

    for (int i = 0; i < graph->edge_count; i++) {
        Edge e = graph->edges[i];
        if (adjacency_list->list_sizes[e.from] + 1 > adjacency_list->list_capacities[e.from]) {
            adjacency_list->list_capacities[e.from] *= 2;
            adjacency_list->adjacency_lists[e.from] = realloc(adjacency_list->adjacency_lists[e.from],
                adjacency_list->list_capacities[e.from] * sizeof(AdjacencyNode));
            if (adjacency_list->adjacency_lists[e.from] == NULL) {
                free_adjacency_list(adjacency_list, graph->vertex_count);
                return NULL;
            }
        }
        adjacency_list->adjacency_lists[e.from][adjacency_list->list_sizes[e.from]].to = e.to;
        adjacency_list->adjacency_lists[e.from][adjacency_list->list_sizes[e.from]].weight = e.weight;
        adjacency_list->list_sizes[e.from]++;
    }

    return adjacency_list;
}

ll* dijkstra(const Graph* graph, const int vertex) {
    if (graph == NULL || vertex < 0 || vertex >= graph->vertex_count) {
        return NULL;
    }

    AdjacencyList* adj_list = build_adjacency_list(graph);
    if (adj_list == NULL) {
        return NULL;
    }
    ll* dists = malloc(sizeof(ll) * graph->vertex_count);
    if (dists == NULL) {
        free_adjacency_list(adj_list, graph->vertex_count);
        return NULL;
    }
    for (int i = 0; i < graph->vertex_count; i++) dists[i] = LLONG_MAX;
    dists[vertex] = 0;

    PriorityQueue* queue = init_queue();
    if (queue == NULL) {
        free_adjacency_list(adj_list, graph->vertex_count);
        free(dists);
        return NULL;
    }
    const Pair p = { .vertex = vertex, .distance = 0 };
    push(queue, p);

    while (!is_empty(queue)) {
        Pair cur;
        if (pop(queue, &cur) != 0) break;
        const int u = cur.vertex;
        const ll dist_u = cur.distance;

        if (dist_u > dists[u]) continue;

        for (int j = 0; j < adj_list->list_sizes[u]; j++) {
            const AdjacencyNode node = adj_list->adjacency_lists[u][j];
            const int v = node.to;
            const ll w = node.weight;
            if (dist_u == LLONG_MAX) continue;

            const ll new_dist = dist_u + w;
            if (dists[v] == LLONG_MAX || new_dist < dists[v]) {
                dists[v] = new_dist;
                const Pair newp = { .vertex = v, .distance = dists[v] };
                if (push(queue, newp) != 0) {
                    free_queue(queue);
                    free(dists);
                    free_adjacency_list(adj_list, graph->vertex_count);
                    return NULL;
                }
            }
        }
    }

    free_queue(queue);
    free_adjacency_list(adj_list, graph->vertex_count);
    return dists;
}
