#include "priority_queue.h"

#include <stdlib.h>

static void _swap_pair(Pair* a, Pair* b) {
    const Pair tmp = *a;
    *a = *b;
    *b = tmp;
}

PriorityQueue* init_queue(void) {
    PriorityQueue* queue = malloc(sizeof(PriorityQueue));
    if (queue == NULL) {
        return NULL;
    }
    queue->capacity = 2;
    queue->size = 0;
    queue->data = malloc(sizeof(Pair) * queue->capacity);
    if (queue->data == NULL) {
        free(queue);
        return NULL;
    }
    return queue;
}

void free_queue(PriorityQueue* queue) {
    if (queue == NULL) return;
    free(queue->data);
    free(queue);
}

int is_empty(const PriorityQueue* queue) {
    if (queue == NULL || queue->size == 0) return 1;
    return 0;
}

int push(PriorityQueue* queue, const Pair pair) {
    if (queue == NULL) return -1;
    if (queue->size >= queue->capacity) {
        const int new_capacity = queue->capacity * 2;
        Pair* new_data = realloc(queue->data, sizeof(Pair) * new_capacity);
        if (new_data == NULL) return -1;
        queue->data = new_data;
        queue->capacity = new_capacity;
    }
    int i = queue->size;
    queue->data[i] = pair;
    queue->size++;

    while (i > 0) {
        const int parent = (i - 1) / 2;
        if (queue->data[parent].distance <= queue->data[i].distance) break;
        _swap_pair(&queue->data[parent], &queue->data[i]);
        i = parent;
    }
    return 0;
}

int pop(PriorityQueue* queue, Pair* out) {
    if (queue == NULL || queue->size == 0) return -1;
    if (out != NULL) *out = queue->data[0];
    queue->size--;
    if (queue->size > 0) {
        queue->data[0] = queue->data[queue->size];

        int i = 0;
        while (1) {
            const int l = 2 * i + 1;
            const int r = 2 * i + 2;
            int smallest = i;
            if (l < queue->size && queue->data[l].distance < queue->data[smallest].distance) smallest = l;
            if (r < queue->size && queue->data[r].distance < queue->data[smallest].distance) smallest = r;
            if (smallest == i) break;
            _swap_pair(&queue->data[i], &queue->data[smallest]);
            i = smallest;
        }
    }
    return 0;
}

Pair* top(const PriorityQueue* queue) {
    if (queue == NULL || queue->size == 0) return NULL;
    return &queue->data[0];
}