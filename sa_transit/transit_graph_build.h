//
//  transit_graph_build.h
//  sa_transit
//
//  Created by Bryant Davis on 11/5/14.
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//

#ifndef __sa_transit__transit_graph_build__
#define __sa_transit__transit_graph_build__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue_int.h"
#include "transit_graph_build.h"
#include "graph_list_search.h"


#define INITIAL_VERTEX_STORE_CAP 20000
#define INITIAL_TRIP_CAP 50
#define INITIAL_STOP_NAME_LENGTH 40
struct vertex {
    int stop_id;
    float latitude;
    float longitude;
    char* name;
};

struct vertex_storage {
    struct vertex** contents; //an array of vertex pointers
    long unsigned size;
    long unsigned capacity;
};

struct edge {
    struct vertex* source;
    struct vertex* dest;
    float weight;
    unsigned long* trips;
    unsigned long num_trips;
    unsigned long trips_cap;
};

struct edge_list_node {
    struct edge* edge;
    struct edge_list_node* next;
};

struct graph_list {
    struct edge_list_node** vertices;
    int num_vert;
};

struct vertex* build_vertex(int stop_id, float latitude, float longitude, char* name);
struct vertex_storage* build_vertex_storage(void);
int graph_build_vertices_edges(struct graph_list** my_graph, char* filename);
struct edge_list_node* build_edge_node(struct edge* my_edge);
struct graph_list* build_graph_list();
struct edge* build_edge(struct vertex* src, struct vertex* dest, float weight, unsigned long trip_id);
int graph_add_edge(struct edge* new_edge, struct graph_list** my_graph);

#endif /* defined(__sa_transit__transit_graph_build__) */
