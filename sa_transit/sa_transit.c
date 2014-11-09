//
//  main.c
//  sa_transit
//
//  Created by Bryant Davis on 11/5/14.
//  This project is intended to to model and peform graph-based searches on the sa-transit system
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_list_search.h"
#include "transit_graph_build.h"
#include "queue_int.h"



int main(int argc, const char * argv[]) {
    
    
    struct graph_list* my_graph = build_graph_list();
    graph_build_vertices_edges(&my_graph, "/Users/bryantdavis/code/practice/C/sa_transit/sa_transit/stop_times.txt");

    //printf("%lu\t", (my_graph)->vertices[12380]->edge->trips[3]);
    //printf("%f\t", (my_graph)->vertices[12380]->edge->weight);
    //graph_list_breadth_first_search(28, my_graph);
    ///graph_list_get_parent_short_path(3650,11399, my_graph);
    return 0;
}
