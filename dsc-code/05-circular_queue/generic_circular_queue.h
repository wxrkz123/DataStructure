#pragma once
#include <stddef.h>
#include <stdbool.h>

typedef struct CircularQueue Queue;

Queue* queue_create(size_t capacity, size_t element_size);
void queue_destroy(Queue** p_queue);
bool queue_enqueue(Queue* queue, const void* element_data);
bool queue_dequeue(Queue* queue, void* output_buffer);
bool queue_peek(const Queue* queue, void* output_buffer);
bool queue_is_empty(const Queue* queue);
bool queue_is_full(const Queue* queue);
size_t queue_get_size(const Queue* queue);
size_t queue_get_capacity(const Queue* queue);