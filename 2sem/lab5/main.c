#include "graph.h"
#include "dijkstra.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static void print_usage(const char *prog) {
    fprintf(stderr,
        "Usage: %s --input <file> --start <vertex> [--out-result <file>] [--out-graph <file>]\n"
        "  --input      path to input graph file (first line: N M, then M lines: u v w, 0-based)\n"
        "  --start      start vertex index (0-based)\n"
        "  --out-result path to write distances (one per line), optional\n"
        "  --out-graph  path to write graph (optional)\n",
        prog);
}

static int save_result_to_file(const long long *dists, int n, const char *filename) {
    if (dists == NULL || filename == NULL) return -1;
    FILE *f = fopen(filename, "w");
    if (f == NULL) return -1;
    for (int i = 0; i < n; ++i) {
        if (dists[i] == LLONG_MAX) fprintf(f, "%d INF\n", i);
        else fprintf(f, "%d %lld\n", i, (long long)dists[i]);
    }
    fclose(f);
    return 0;
}

static bool interactive_mode = false;

static void run_interactive_mode(const char *out_result, const char *out_graph) {
    printf("Interactive mode activated.\n");
    printf("Enter the number of vertices and edges (N M): ");

    int n, m;
    if (scanf("%d %d", &n, &m) != 2 || n <= 0 || m < 0) {
        fprintf(stderr, "Invalid input for vertices and edges.\n");
        return;
    }

    Graph *g = init_graph();
    if (g == NULL) {
        fprintf(stderr, "Failed to allocate graph.\n");
        return;
    }

    for (int i = 0; i < m; ++i) {
        printf("Enter edge %d (u v w): ", i + 1);
        int u, v;
        long long w;
        if (scanf("%d %d %lld", &u, &v, &w) != 3 || u < 0 || v < 0 || u >= n || v >= n || w < 0) {
            fprintf(stderr, "Invalid edge input.\n");
            destroy_graph(g);
            return;
        }
        add_edge(g, u, v, w);
    }

    printf("Enter the start vertex: ");
    int start;
    if (scanf("%d", &start) != 1 || start < 0 || start >= n) {
        fprintf(stderr, "Invalid start vertex.\n");
        destroy_graph(g);
        return;
    }

    long long *dists = dijkstra(g, start);
    if (dists == NULL) {
        fprintf(stderr, "Dijkstra failed.\n");
        destroy_graph(g);
        return;
    }

    printf("Shortest paths from vertex %d:\n", start);
    for (int i = 0; i < n; ++i) {
        if (dists[i] == LLONG_MAX) printf("%d INF\n", i);
        else printf("%d %lld\n", i, dists[i]);
    }

    if (out_result != NULL) {
        if (save_result_to_file(dists, n, out_result) != 0) {
            fprintf(stderr, "Failed to write result to '%s'\n", out_result);
        } else {
            printf("Results successfully written to '%s'\n", out_result);
        }
    } else {
        fprintf(stderr, "No output file specified for results.\n");
    }

    if (out_graph != NULL) {
        if (save_graph_to_file(g, out_graph) != 0) {
            fprintf(stderr, "Failed to write graph to '%s'\n", out_graph);
        } else {
            printf("Graph successfully written to '%s'\n", out_graph);
        }
    } else {
        fprintf(stderr, "No output file specified for graph.\n");
    }

    free(dists);
    destroy_graph(g);
}

int main(int argc, char **argv) {
    const char *input = NULL;
    const char *out_result = NULL;
    const char *out_graph = NULL;
    int start = 0;

    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--input") == 0 && i + 1 < argc) {
            input = argv[++i];
        } else if (strcmp(argv[i], "--start") == 0 && i + 1 < argc) {
            start = atoi(argv[++i]);
        } else if (strcmp(argv[i], "--out-result") == 0 && i + 1 < argc) {
            out_result = argv[++i];
        } else if (strcmp(argv[i], "--out-graph") == 0 && i + 1 < argc) {
            out_graph = argv[++i];
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            print_usage(argv[0]);
            return 0;
        } else if (strcmp(argv[i], "--interactive") == 0) {
            interactive_mode = true;
        } else {
            fprintf(stderr, "Unknown argument: %s\n", argv[i]);
            print_usage(argv[0]);
            return 1;
        }
    }

    if (interactive_mode) {
        run_interactive_mode(out_result, out_graph);
        return 0;
    }

    if (input == NULL) {
        fprintf(stderr, "Input file is required.\n");
        print_usage(argv[0]);
        return 1;
    }

    Graph *g = init_graph();
    if (g == NULL) {
        fprintf(stderr, "Failed to allocate graph.\n");
        return 1;
    }
    if (load_graph_from_file(g, input) != 0) {
        fprintf(stderr, "Failed to load graph from '%s'\n", input);
        destroy_graph(g);
        return 1;
    }

    if (start < 0 || start >= g->vertex_count) {
        fprintf(stderr, "Start vertex out of range (0..%d)\n", g->vertex_count - 1);
        destroy_graph(g);
        return 1;
    }

    long long *dists = dijkstra(g, start);
    if (dists == NULL) {
        fprintf(stderr, "Dijkstra failed\n");
        destroy_graph(g);
        return 1;
    }

    if (out_result != NULL) {
        if (save_result_to_file(dists, g->vertex_count, out_result) != 0) {
            fprintf(stderr, "Failed to write result to '%s'\n", out_result);
        } else {
            printf("Results successfully written to '%s'\n", out_result);
        }
    } else {
        fprintf(stderr, "No output file specified for results.\n");
    }

    if (out_graph != NULL) {
        if (save_graph_to_file(g, out_graph) != 0) {
            fprintf(stderr, "Failed to write graph to '%s'\n", out_graph);
        } else {
            printf("Graph successfully written to '%s'\n", out_graph);
        }
    } else {
        fprintf(stderr, "No output file specified for graph.\n");
    }

    free(dists);
    destroy_graph(g);
    return 0;
}