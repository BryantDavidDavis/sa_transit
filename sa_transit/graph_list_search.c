//
//  graph_list_search.c
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


//declaring these two array static hides them inside this file, we will then probably need getter functions now
static int visited[INITIAL_VERTEX_STORE_CAP] = {0};
static int parents[INITIAL_VERTEX_STORE_CAP] = {0}; //perhaps I should initialize this to another value that couldn't be a stop_id like -2?

void graph_list_breadth_first_search(int star_place, struct graph_list* my_graph);
void graph_list_get_parent_short_path(int source_stop_id, int dest_stop_id, struct graph_list* my_graph);

void graph_list_breadth_first_search(int start_place, struct graph_list* my_graph) {
    struct edge_list_node* temp;
    struct queue_int* my_queue = queue_int_build(INITIAL_QUEUE_INT_CAP);
    int u = start_place;
    //    int u = 0;
    //    while (my_graph->vertices[u] == NULL) {
    //        u++;
    //    }
    if (queue_int_offer(u, my_queue)) {
        parents[u] = -1;
        while (my_queue->size > 0) {
            u = queue_int_poll(my_queue);
            temp = my_graph->vertices[u];
            while (temp != NULL) {
                if ((visited[temp->edge->dest->stop_id] == 0)&&(queue_int_contains(temp->edge->dest->stop_id, my_queue) == 0)) {
                    queue_int_offer(temp->edge->dest->stop_id, my_queue);
                    parents[temp->edge->dest->stop_id] = u;
                }
                temp = temp->next;
            }
            visited[u] = 1;
        }
    } else {
        printf("failed to offer item %d to the queue", u);
    }
}

//the reason why this doesn't work is because we are performing breadth first search in relation to an arbitrary point, whereas we want it relative to a specific starting point
void graph_list_get_parent_short_path(int source_stop_id, int dest_stop_id, struct graph_list* my_graph) {
    graph_list_breadth_first_search(source_stop_id, my_graph);
    printf("final stop is %d\n", dest_stop_id);
    int previous_stop = parents[dest_stop_id];
    printf("the stop before it was %d\n", previous_stop);
    while ((previous_stop != source_stop_id) &&(previous_stop != -1)) {
        previous_stop = parents[previous_stop];
        printf("the stop before it was %d\n", previous_stop);
    }
    printf("the first stop is the beginning of your journey, stop %d", previous_stop);
    
}