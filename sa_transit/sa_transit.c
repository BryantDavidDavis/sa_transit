//
//  main.c
//  sa_transit
//
//  Created by Bryant Davis on 11/5/14.
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transit_graph_build.h"


int main(int argc, const char * argv[]) {
    // insert code here...
    //printf("Hello, and Welcome to the wonderful world of sa transit!\n");
    //say_hello();
    //graph_build_vertices_edges();
    
    //i think we want something like the following
    //struct graph_list* my_graph = build_graph()
    //this creates an empty graph, now we should pass both the empty graph and a text file to create edges and vertices
    //then this text file will be passed on to the build_graph_edges_and_vertices()
    struct graph_list* my_graph = build_graph_list();
    graph_build_vertices_edges(&my_graph, "/Users/bryantdavis/code/practice/C/sa_transit/sa_transit/stop_times.txt");
    //printf("%lu\t", my_graph->vertices[12380]->edge->trips[0]);
    //printf("%lu\t", my_graph->vertices[12380]->edge->trips[1]);
    //printf("%lu\t", my_graph->vertices[12380]->edge->trips[2]);
    printf("%lu\t", (my_graph)->vertices[12380]->edge->trips[3]);
    printf("%f\t", (my_graph)->vertices[12380]->edge->weight);
    return 0;
}
