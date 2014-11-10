//
//  graph_list_search.h
//  sa_transit
//
//  Created by Bryant Davis on 11/7/14.
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//

#ifndef __sa_transit__graph_list_search__
#define __sa_transit__graph_list_search__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transit_graph_build.h"
#include "queue_int.h"

struct graph_list; //why do I have to give this here?

struct x_order {
    int* data;
    int size;
};

void graph_list_breadth_first_search(int start_place, struct graph_list* my_graph);
void graph_list_get_parent_short_path(int source_stop_id, int dest_stop_id, struct graph_list* my_graph);
struct x_order* x_order_build(void);
void graph_list_depth_first_search(struct x_order* discovery_order, struct x_order* finished_order, int start_place, struct graph_list* my_graph);
void depth_first_print(struct x_order* discovery_order, struct graph_list* my_graph);

#endif /* defined(__sa_transit__graph_list_search__) */
