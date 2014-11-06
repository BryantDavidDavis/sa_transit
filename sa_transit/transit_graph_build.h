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
#include "transit_graph_build.h"

#define INITIAL_VERTEX_STORE_CAP 20000
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

struct vertex* build_vertex(int stop_id, float latitude, float longitude, char* name);
struct vertex_storage* build_vertex_storage(void);
int graph_build_vertices_edges(void);

#endif /* defined(__sa_transit__transit_graph_build__) */
