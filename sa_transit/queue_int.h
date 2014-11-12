//
//  queue_int.h
//  sa_transit
//
//  Created by Bryant Davis on 11/7/14.
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//

#ifndef __sa_transit__queue_int__
#define __sa_transit__queue_int__

#define INITIAL_QUEUE_INT_CAP 20

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transit_graph_build.h"
#include "graph_list_search.h"

struct graph_list;

struct queue_int {
    int* data;
    int size;
    int front;
    int rear;
    int capacity;
};

struct queue_int* queue_int_build(int capacity);
int queue_int_offer(int item, struct queue_int* my_queue);
int queue_int_poll(struct queue_int* my_queue);
//int queue_int_reallocate(struct queue_int* old_queue); don't need this method in h file, because it is internal function
int queue_int_contains(int item, struct queue_int* my_queue);
void graph_list_breadth_first(struct graph_list* my_graph, int* visited, int* parents);

#endif /* defined(__sa_transit__queue_int__) */
