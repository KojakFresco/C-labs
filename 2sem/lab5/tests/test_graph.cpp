#include <gtest/gtest.h>
#include "../graph.h"

TEST(Graph, AddAndCheckEdge) {
    Graph* g = init_graph();
    ASSERT_NE(g, nullptr);
    add_edge(g, 0, 1, 42);
    // Проверим, что ребро добавлено
    bool found = false;
    for (int i = 0; i < g->edge_count; ++i) {
        if (g->edges[i].from == 0 && g->edges[i].to == 1 && g->edges[i].weight == 42) {
            found = true;
            break;
        }
    }
    EXPECT_TRUE(found);
    destroy_graph(g);
}

