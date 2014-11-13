//
//  transit_graph_build.c
//  sa_transit
//
//  Created by Bryant Davis on 11/5/14.
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//  Bus routes, stops, and times used to create this graph is provided by Via Metropolitan Transit

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transit_graph_build.h"
#include "queue_int.h"
#include "graph_list_search.h"

struct vertex* build_vertex(int stop_id, float latitude, float longitude, char* name);
struct vertex_storage* build_vertex_storage(void);
int graph_build_vertices_edges(struct graph_list** my_graph, char* filename);
struct edge_list_node* build_edge_node(struct edge* my_edge);
struct graph_list* build_graph_list();
struct edge* build_edge(struct vertex* src, struct vertex* dest, float weight, unsigned long trip_id);
int graph_add_edge(struct edge* new_edge, struct graph_list** my_graph);
float edge_get_weight(); //need to figure this out
struct edge* trips_reallocate(struct edge* my_edge);
int* graph_list_build_vertex_array(struct graph_list* my_graph);  //possibly unneccessary since we have a build_vertex_storage function but lets handle this later

struct edge* trips_reallocate(struct edge* my_edge){
    long unsigned* trips = malloc(sizeof(long unsigned) * (my_edge->trips_cap) * 2);
    if (trips != NULL) {
        my_edge->trips_cap = my_edge->trips_cap * 2;
        for (int i = 0; i < my_edge->num_trips; i++) {
            trips[i] = my_edge->trips[i];
        }
        free(my_edge->trips);
        my_edge->trips = trips;
    } else {
        printf("reallocation of trips failed miserably");
    }
    return my_edge;
}

struct edge_list_node* build_edge_node(struct edge* my_edge) {
    struct edge_list_node* new_node = malloc(sizeof(struct edge_list_node));
    if (new_node != NULL) {
        new_node->edge = my_edge;
        new_node->next = NULL;
        return new_node;
    } else {
        printf("build_edge_node failed");
        return NULL;
    }
}

struct graph_list* build_graph_list() {
    struct graph_list* new_graph = malloc(sizeof(struct graph_list));
    if (new_graph != NULL) {
        new_graph->vertices = malloc(sizeof(struct edge_list_node*) * INITIAL_VERTEX_STORE_CAP);
        new_graph->num_vert = 0;
        for (int i = 0; i < INITIAL_VERTEX_STORE_CAP ; i++) {
            new_graph->vertices[i] = NULL;
        }
        return new_graph;
    } else {
        printf("build graph list failed");
        return NULL;
    }
}

struct edge* build_edge(struct vertex* src, struct vertex* dest, float weight, unsigned long trip_id) {
    struct edge* new_edge = malloc(sizeof(struct edge));
    if (new_edge != NULL) {
        new_edge->source = src;
        new_edge->dest = dest;
        new_edge->weight = weight;
        new_edge->trips_cap = INITIAL_TRIP_CAP;
        new_edge->trips = malloc(sizeof(unsigned long) * new_edge->trips_cap); //will need to dynamically reallocate later, but I need to keep track of trip number
        if (new_edge->trips) {
            for (int i = 0; i < new_edge->trips_cap; i++) {
                new_edge->trips[i] = 0;
            }
            new_edge->trips[0] = trip_id;
            new_edge->num_trips = 1;  //stores the first trip on which the edge occurs
            return new_edge;
        } else {
            printf("build edge trips failed");
            return NULL;
        }
    } else {
        printf("build new edge failed");
        return NULL;
    }
}

struct vertex* build_vertex(int stop_id, float latitude, float longitude, char* name) {
    struct vertex* new_vertex = malloc(sizeof(struct vertex));
    if (new_vertex != NULL) {
        new_vertex->name = malloc(sizeof(char) * INITIAL_STOP_NAME_LENGTH);
        if (new_vertex->name != NULL) {
            new_vertex->stop_id = stop_id;
            new_vertex->latitude = latitude;
            new_vertex->longitude = longitude;
            if (name) {
                strcpy(name, new_vertex->name);
            } else {
                new_vertex->name = NULL; //perhaps we don't want to put NULL here? What happens when we want to add a name?
            }
        } else {
            printf("new_vertex->name allocation failed");
            return NULL;
        }
    } else {
        printf("new_vertex allocation failed");
        return NULL;
    }
    return new_vertex;
}

struct vertex_storage* build_vertex_storage() {
    struct vertex_storage* vertices = malloc(sizeof(struct vertex_storage));
    if (vertices != NULL) {
        vertices->contents = malloc(sizeof(struct vertex*) * INITIAL_VERTEX_STORE_CAP);
        if (vertices->contents != NULL) {
            vertices->size = 0;
            vertices->capacity = INITIAL_VERTEX_STORE_CAP;
            for (int i = 0; i < vertices->capacity; i++) {
                vertices->contents[i] = NULL;
            }
            return vertices;
        } else {
            printf("there was a problem initializing the contents");
            return NULL;
        }
    } else {
        printf("There was a problem creating the vertex storage");
        return NULL;
    }
}

int add_vertex_to_storage(struct vertex* my_vertex, struct vertex_storage** vertices) {
    if ((*vertices)->contents[my_vertex->stop_id] == NULL) {
        (*vertices)->contents[my_vertex->stop_id] = my_vertex;
        (*vertices)->size++;
        return 1;
    } else {
        return 0; //already in there
    }
}

int graph_add_edge(struct edge* new_edge, struct graph_list** my_graph) {
    int source_id = new_edge->source->stop_id;
    if ((*my_graph)->vertices[source_id] == NULL) {
        struct edge_list_node* new_node = build_edge_node(new_edge); //new node has been successfully created
        if (new_node) {
            (*my_graph)->vertices[source_id] = new_node;
            return 1;
        } else {
            return 0;
        }
    } else {
        struct edge_list_node* temp = (*my_graph)->vertices[source_id];
        while ((temp != NULL)&&(temp->edge->dest->stop_id != new_edge->dest->stop_id)) { //traverse the list until either we have found it or know it's not there.
            temp = temp->next;
        }
        if (temp == NULL) {
            //we have traverse all the edges for this vertex, and found there is not such an edge, so we just insert at the beginning of the list
            //we have discovered that the source vertex is in the array but the edge is not, so we want to add the edge to the beginning of the list
            struct edge_list_node* new_node = build_edge_node(new_edge);
            new_node->next = (*my_graph)->vertices[source_id];
            (*my_graph)->vertices[source_id] = new_node;
        } else {
            //we have found such an edge, and just need to add a trip_id to the trips and incrememt num_trips, but what happens to the new edge thing? do we free it?
            //need to check if the number of trips has been reached, and if so reallocate the trips thing
            if (temp->edge->num_trips == temp->edge->trips_cap) {
                //need to reallocate first
                temp->edge = trips_reallocate(temp->edge);
            }
            temp->edge->trips[temp->edge->num_trips] = new_edge->trips[0];
            temp->edge->num_trips++;
        }
        return 1;
    }
    return 0;
}

int graph_build_vertices_edges (struct graph_list** my_graph, char* filename) {
    //system("pwd");
    FILE* fp;
    fp = fopen(filename, "r"); //changed this line
    if (fp != NULL) {
        printf("It's open baby\n");
    } else {
        printf("nope, can't open file.  Nice try though");
    }
    int trip_id = 0;
    char arrival_time[30];
    char departure_time[30];
    int stop_id = 0;
    int stop_seq = 0;
    float shape_dist_traveled = 0;
    int got = 0;
    long file_pos = 0;
    //the following reads character by character
    char first_letter;
    struct vertex_storage* vertices = build_vertex_storage();
    //struct graph_list* my_graph = build_graph_list();
    if (vertices == NULL) {
        printf("storage couldn't be built");
        fclose(fp);
        return 0;
    } else {
        
        while((first_letter = getc(fp)) != '\n') {
            //printf("%c", first_letter);
        }; //advances the pointer to the first character of the second line
        unsigned long line_number = 1;
        
        struct vertex* previous_vertex = NULL;
        float previous_dist_traveled = 0;
        struct vertex* current_vertex = NULL;
        float current_dist_traveled = 0;
        
        for (unsigned long i = 0; i < 1476913; i++) {
            line_number++;
            got = fscanf(fp, "%d,%[^,],%[^,],%d,%d,,,,", &trip_id, arrival_time, departure_time, &stop_id, &stop_seq);
            if (got != 5) {
                printf("error reading file at line %lu", i);
                return 0;
                //now we need to back up
            } else {
                file_pos = ftell(fp);
                if ((first_letter = getc(fp)) == '\r') {
                    shape_dist_traveled = 0.000;
                } else {
                    fseek(fp, file_pos, SEEK_SET);
                    got = fscanf(fp, "%f\n", &shape_dist_traveled);
                    if (got != 1) {
                        //printf("error reading distance traveled at line &d\n", i);
                        return 0;
                    }
                }
            }
            
            if ((i == 0) || (stop_seq == 1)) { //this is the first line of the document and there is no previous vertex, and perhaps I need to check if the sequence is 1
                previous_vertex = build_vertex(stop_id, 0, 0, 0); //this works
                previous_dist_traveled = shape_dist_traveled;
                //printf("%d\n", add_vertex_to_storage(previous_vertex, &vertices)); //add the vertex to the array
            } else {
                current_vertex = build_vertex(stop_id, 0, 0, 0); //build the vertex from the line, and store it in current
                current_dist_traveled = shape_dist_traveled;
                //printf("%d\n", add_vertex_to_storage(current_vertex, &vertices)); //add it to the array
                //now we need to try to add the edge made up of previous_vertex and current_vertex
                struct edge* new_edge = build_edge(previous_vertex, current_vertex, current_dist_traveled - previous_dist_traveled, trip_id); //the edge has been successfully created
                if (new_edge != NULL) {
                    if(graph_add_edge(new_edge, my_graph) == 1) {
                        //printf("added to graph\n");
                        previous_vertex = current_vertex;
                        previous_dist_traveled = current_dist_traveled;
                    } else {
                        printf("failed to add to graph\n");
                    }
                } else {
                    printf("new edge not created\n");
                }
            }
            
        }
    }
    fclose(fp);
    return 1;
}

int* graph_list_build_vertex_array(struct graph_list* my_graph) {
    int* stops = malloc(sizeof(int)*INITIAL_VERTEX_STORE_CAP);
    struct edge_list_node* temp;
    for (int i = 0; i < INITIAL_VERTEX_STORE_CAP; i++) {
        if (my_graph->vertices[i] != NULL) {
            temp = my_graph->vertices[i];
            stops[i] = 1;
            while (temp->next != NULL) {
                temp = temp->next;
                stops[temp->edge->dest->stop_id] = 1;
            }
        } else {
            stops[i] = 0;
        }
    }
    return stops;
}