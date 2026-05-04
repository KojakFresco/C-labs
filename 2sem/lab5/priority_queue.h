#ifndef LABAC_PRIORITY_QUEUE_H
#define LABAC_PRIORITY_QUEUE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    int vertex;
    long long distance;
} Pair;

typedef struct {
    Pair* data;
    int capacity;
    int size;
} PriorityQueue;

PriorityQueue* init_queue(void);
void free_queue(PriorityQueue* queue);
int is_empty(const PriorityQueue* queue);
int push(PriorityQueue*, const Pair pair);
int pop(PriorityQueue* queue, Pair* out);
Pair* top(const PriorityQueue* queue);

#ifdef __cplusplus
}
#endif

#endif //LABAC_PRIORITY_QUEUE_H
