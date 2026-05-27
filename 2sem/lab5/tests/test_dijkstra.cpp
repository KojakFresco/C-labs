#include <gtest/gtest.h>

#include "../graph.h"
#include "../dijkstra.h"

TEST(Dijkstra, SmallGraph) {
    Graph* g = init_graph();
    ASSERT_NE(g, nullptr);
    // simple graph: 0-1 (7), 0-2 (9), 1-2 (10), 1-3 (15), 2-3 (11)
    add_edge(g, 0, 1, 7);
    add_edge(g, 0, 2, 9);
    add_edge(g, 1, 2, 10);
    add_edge(g, 1, 3, 15);
    add_edge(g, 2, 3, 11);
    long long* d = dijkstra(g, 0);
    ASSERT_NE(d, nullptr);
    // expected distances: 0->0, 1->7, 2->9, 3->20 (0->2->3)
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[1], 7);
    EXPECT_EQ(d[2], 9);
    EXPECT_EQ(d[3], 20);
    free(d);
    destroy_graph(g);
}

TEST(Dijkstra, DisconnectedGraph) {
    Graph* g = init_graph();
    ASSERT_NE(g, nullptr);
    add_edge(g, 0, 1, 1);
    add_edge(g, 1, 2, 2);
    // Вершина 3 изолирована
    long long* d = dijkstra(g, 0);
    ASSERT_NE(d, nullptr);
    EXPECT_EQ(d[0], 0);
    EXPECT_EQ(d[1], 1);
    EXPECT_EQ(d[2], 3);
    EXPECT_EQ(d[3], LLONG_MAX);
    free(d);
    destroy_graph(g);
}
