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

    //printf("%lu\t", (my_graph)->vertices[12380]->edge->trips[3]);
    //printf("%f\t", (my_graph)->vertices[12380]->edge->weight);
    //graph_list_breadth_first_search(28, my_graph);
    ///graph_list_get_parent_short_path(3650,11399, my_graph);
    
    //struct x_order* discovery_order = x_order_build();
    //struct x_order* finished_order = x_order_build();
    //graph_list_depth_first_search(discovery_order, finished_order, 2, my_graph);
    //depth_first_print(discovery_order, my_graph);
    struct dijkstra_result* my_result = graph_list_dijkstra_alg(my_graph, 4);
    dijkstra_print_result(my_result, 6206);
    //so now that we have obtained our result it's time to check it to see what it tells us
    return 0;
}
