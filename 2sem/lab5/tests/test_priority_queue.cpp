#include "../priority_queue.h"

#include <gtest/gtest.h>

TEST(PriorityQueue, PushPopOrder) {
    PriorityQueue* q = init_queue();
    ASSERT_NE(q, nullptr);
    Pair a = { .vertex = 0, .distance = 50 };
    Pair b = { .vertex = 1, .distance = 10 };
    Pair c = { .vertex = 2, .distance = 20 };
    EXPECT_EQ(push(q, a), 0);
    EXPECT_EQ(push(q, b), 0);
    EXPECT_EQ(push(q, c), 0);

    Pair out;
    ASSERT_EQ(pop(q, &out), 0);
    EXPECT_EQ(out.vertex, 1);
    EXPECT_EQ(out.distance, 10);
    ASSERT_EQ(pop(q, &out), 0);
    EXPECT_EQ(out.vertex, 2);
    ASSERT_EQ(pop(q, &out), 0);
    EXPECT_EQ(out.vertex, 0);

    free_queue(q);
}

