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
static int depth_parents[INITIAL_VERTEX_STORE_CAP] = {0};

void graph_list_breadth_first_search(int star_place, struct graph_list* my_graph);
void graph_list_get_parent_short_path(int source_stop_id, int dest_stop_id, struct graph_list* my_graph);
struct x_order* x_order_build(void);
void graph_list_depth_first_search(struct x_order* discovery_order, struct x_order* finished_order, int start_place, struct graph_list* my_graph);
void depth_first_print(struct x_order* discovery_order, struct graph_list* my_graph);
struct dijkstra_result* graph_list_dijkstra_alg(struct graph_list* my_graph, int start_place);

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

struct x_order* x_order_build() {
    struct x_order* new_order = malloc(sizeof(struct x_order));
    if (new_order != NULL) {
        new_order->data = malloc(sizeof(int) * INITIAL_VERTEX_STORE_CAP);
        if (new_order->data != NULL) {
            new_order->size = 0;
            for (int i = 0; i < INITIAL_VERTEX_STORE_CAP; i++) {
                new_order->data[i] = 0;
            }
            return new_order;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
}

void graph_list_depth_first_search(struct x_order* discovery_order, struct x_order* finished_order, int start_place, struct graph_list* my_graph) {
    
    discovery_order->data[discovery_order->size] = start_place;
    if (discovery_order->size == 0) {
        depth_parents[start_place] = -1;
    }
    discovery_order->size++;
    
    struct edge_list_node* temp = my_graph->vertices[start_place];
    
    while (temp != NULL) { //while the vertex exists and is not in the parents array
        //actually before we add it to the parents array we need to make sure that it isn't already there
        if (depth_parents[temp->edge->dest->stop_id] == 0) {
            depth_parents[temp->edge->dest->stop_id] = start_place;
            printf("%d items visited.  Now stop %d's parent is now set to %d\n", discovery_order->size, temp->edge->dest->stop_id, start_place);
            graph_list_depth_first_search(discovery_order, finished_order, temp->edge->dest->stop_id, my_graph);
        }
        temp = temp->next;
    }
    
    finished_order->data[finished_order->size] = start_place;
    finished_order->size++;
}

void depth_first_print(struct x_order* discovery_order, struct graph_list* my_graph) {
    for (int i = 0; i < discovery_order->size; i++) {
        printf("bus_stop %d\n", discovery_order->data[i]);
    }
    printf("\n");
}

struct dijkstra_result* graph_list_dijkstra_alg(struct graph_list* my_graph, int start_place) {
    struct dijkstra_result* my_result = malloc(sizeof(struct dijkstra_result));
    if (my_result != NULL) {
        my_result->v_s = graph_list_build_vertex_array(my_graph);
        my_result->s = malloc(sizeof(int)* INITIAL_VERTEX_STORE_CAP);
        my_result->d = malloc(sizeof(float)* INITIAL_VERTEX_STORE_CAP);
        my_result->p = malloc(sizeof(int)* INITIAL_VERTEX_STORE_CAP);
        if ((my_result->v_s == NULL)||(my_result->s == NULL)||(my_result->d == NULL)||(my_result->p == NULL)) {
            return NULL;
        }
    } else {
        return NULL;
    }

    for (int i = 0; i < INITIAL_VERTEX_STORE_CAP; i++) {
        my_result->s[i] = 0;
        my_result->p[i] = start_place;
        my_result->d[i] = NOT_ADJACENT;
    }
    my_result->s[start_place] = 1;
    my_result->v_s[start_place] = 0;
    struct edge_list_node* temp = my_graph->vertices[start_place];
    while (temp != NULL) {
        my_result->d[temp->edge->dest->stop_id] = temp->edge->weight;
        temp = temp->next;
    }
    //now that the result struct has been properly allocated and initialized we can perform the algorithm
    for (int i = 1; i < INITIAL_VERTEX_STORE_CAP; i++) {
        int smallest = 62000; //initialize to an arbitrarily large number beyond the bounds of the vertex store cap
        if (my_result->v_s[i] != 0) {
            for (int j = i; j < INITIAL_VERTEX_STORE_CAP; j++) {
                if (my_result->v_s[j] != 0) {
                    if (j == i) {
                        smallest = j;
                    } else if (my_result->d[j] < my_result->d[smallest]) {
                        smallest = j;
                    }
                }
            }
        
            my_result->v_s[smallest] = 0;
            my_result->s[smallest] = 1;
            int u = smallest;
            int v = 0;
            temp = my_graph->vertices[u];
            while (temp != NULL) {
                v = temp->edge->dest->stop_id;
                if ((my_result->d[v] == NOT_ADJACENT)||((my_result->d[u] + temp->edge->weight) < my_result->d[v])) {
                    my_result->d[v] = my_result->d[u] + temp->edge->weight;
                    my_result->p[v] = u;
                }
                temp = temp->next;
            }
        }
    }
    return my_result;
}
