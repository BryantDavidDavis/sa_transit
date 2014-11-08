//
//  queue_int.c
//  sa_transit
//
//  Created by Bryant Davis on 11/7/14.
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transit_graph_build.h"
#include "queue_int.h"
#include "graph_list_search.h"

struct queue_int* queue_int_build(int capacity);
int queue_int_offer(int item, struct queue_int* my_queue);
int queue_int_poll(struct queue_int* my_queue);
int queue_int_reallocate(struct queue_int* old_queue);
int queue_int_contains(int item, struct queue_int* my_queue);
void graph_list_breadth_first(struct graph_list* my_graph, int visited[], int parents[]);

int queue_int_contains(int item, struct queue_int* my_queue) {
    int i = 0, iterator = my_queue->front;
    while (i < my_queue->size) {
        if (my_queue->data[iterator] == item) {
            return 1;
        }
        i++;
    }
    return 0;
}

struct queue_int* queue_int_build(int capacity) {
    struct queue_int* new_queue = malloc(sizeof(struct queue_int));
    if (new_queue != NULL) {
        new_queue->data = malloc(sizeof(int)*capacity);
        if (new_queue->data != NULL) {
            new_queue->size = 0;
            new_queue->front = 0;
            new_queue->capacity = capacity;
            new_queue->rear = capacity - 1;
            for (int i = 0; i < capacity; i++) {
                new_queue->data[i] = 0;
            }
            return new_queue;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

int queue_int_offer(int item, struct queue_int* my_queue) {
    int successful = 1; //flag to track success of reallocation
    if (my_queue->size == my_queue->capacity) {
        successful = queue_int_reallocate(my_queue);
    }
    if (successful) { //successful is only zero if reallocation attempted and failed
        my_queue->rear = (my_queue->rear + 1) % my_queue->capacity;
        my_queue->data[my_queue->rear] = item;
        my_queue->size++;
        return 1;
    } else {
        printf("queue reallocation failed");
        return 0;
    }
}

int queue_int_poll(struct queue_int* my_queue) {
    int return_value = my_queue->data[my_queue->front];
    my_queue->front = (my_queue->front + 1) % my_queue->capacity;
    my_queue->size--;
    return return_value;
}

int queue_int_reallocate(struct queue_int* old_queue) {
    int* new_data = malloc(sizeof(int) * old_queue->capacity * 2);
    if (new_data != NULL) {
        int new_cap = old_queue->capacity * 2;
        int new_size = 0;
        int new_front = 0;
        int new_rear = new_cap - 1;
        for (int i = 0; i < new_cap; i++) {
            new_data[i] = 0;
        }
        for (int i = 0; i < old_queue->size; i++) {
            new_data[i] = old_queue->data[old_queue->front];
            old_queue->front = (old_queue->front + 1) % old_queue->capacity;
            new_size++;
            new_rear = (new_rear + 1) % new_cap;
        }
        free(old_queue->data);
        old_queue->data = new_data;
        old_queue->size = new_size;
        old_queue->front = new_front;
        old_queue->rear = new_rear;
        old_queue->capacity = new_cap;
        return 1;
    } else {
        return 0;
    }
    
}