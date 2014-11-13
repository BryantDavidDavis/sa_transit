//
//  graph_list_search.c
//  sa_transit
//
//  Created by Bryant Davis on 11/7/14.
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//  Bus routes, stops, and times used to create this graph is provided by Via Metropolitan Transit
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
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
void dijkstra_print_result(struct dijkstra_result* my_result, int destination);

void graph_list_breadth_first_search(int start_place, struct graph_list* my_graph) {
    struct edge_list_node* temp;
    struct queue_int* my_queue = queue_int_build(INITIAL_QUEUE_INT_CAP);
    int u = start_place;
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
        my_result->start_place = start_place;
        if ((my_result->v_s == NULL)||(my_result->s == NULL)||(my_result->d == NULL)||(my_result->p == NULL)) {
            return NULL;
        }
    } else {
        return NULL;
    }

    for (int i = 0; i < INITIAL_VERTEX_STORE_CAP; i++) {
        my_result->s[i] = 0;
        //what do we do in this case, because the arrays p and d shouldn't contain start_place, so do we set them equal to something like -1?
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
    for (int i = 1; i < INITIAL_VERTEX_STORE_CAP; i++) { //basically we want to do this until v-s is empty, so this way isn't efficient but it will get the job done
        int smallest = INT_MAX; //initialize to an arbitrarily large number beyond the bounds of the vertex store cap this is simply to initialize it, doesnt mean anything
        int first_iteration = 1; //will change to zero, so we initialize smallest to the proper index once
        for (int j = 1; j < INITIAL_VERTEX_STORE_CAP; j++) {
            if (my_result->v_s[j] != 0) {
                // since smallest is an index, we need to set smallest to a possible index before we compare it as an index.
                //lets use a flag to identify the first iteration, and set smallest on that first iteration
                if (first_iteration == 1) {
                    //printf("smallest index in v-s is %d.  let's find smallest value of d[u]...", j);
                    smallest = j;
                    first_iteration = 0;
                } else if (my_result->d[j] < my_result->d[smallest]) {
                    smallest = j;
                }
            }
            
        }
        //now that the iteration has run its course and we have removed all stops from v-s, we have to avoid the second portion, let's use another flag, or the same flag
        if (first_iteration == 0) { //we have found a stop remaning in v-s, otherwise it is empty
            //printf("we found stop %d, with intermediate stop %d and %f from stop 2\n", smallest, my_result->p[smallest], my_result->d[smallest]);
            my_result->v_s[smallest] = 0;
            my_result->s[smallest] = 1;
            int u = smallest;
            int v = 0;
            temp = my_graph->vertices[u];
            while (temp != NULL) {
                v = temp->edge->dest->stop_id;
                if ((my_result->d[u] + temp->edge->weight) < my_result->d[v]) {
                    my_result->d[v] = my_result->d[u] + temp->edge->weight;
                    //printf("updating stop %d to distance of %f from intermediate stop %d\n", v, temp->edge->weight, u);
                    my_result->p[v] = u;
                }
                temp = temp->next;
            }
        }
    }
    return my_result;
}

void dijkstra_print_result(struct dijkstra_result* my_result, int destination) {
    int i = destination;
    while (i != my_result->start_place) {
        printf("current stop is %d\t", i);
        if (my_result->p[i] == my_result->start_place) {
            printf("to get there we traveled %f miles from the origin at stop %d\n", my_result->d[i], my_result->start_place);
            break;
        } else {
            printf("to get there we traveled %f miles from %d, and %f miles from origin at stop %d\n", (my_result->d[i] - my_result->d[my_result->p[i]]), my_result->p[i], my_result->d[i], my_result->start_place);
            i = my_result->p[i];
        }
    }
}
