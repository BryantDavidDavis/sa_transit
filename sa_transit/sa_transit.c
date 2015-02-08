//
//  main.c
//  sa_transit
//
//  Created by Bryant Davis on 11/5/14.
//  This project is intended to to model and peform graph-based searches on the sa-transit system
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//  Bus routes, stops, and times used to create this graph is provided by Via Metropolitan Transit

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph_list_search.h"
#include "transit_graph_build.h"
#include "queue_int.h"

int* depth_parents;

int main(int argc, const char * argv[]) {
    
    
    struct graph_list* my_graph = build_graph_list();
    graph_build_vertices_edges(&my_graph, "/Users/bryantdavis/code/practice/C/sa_transit/sa_transit/stop_times.txt");
    struct dijkstra_result* my_result = graph_list_dijkstra_alg(my_graph, 23);
    dijkstra_print_result(my_result, 6206);
    return 0;
}
