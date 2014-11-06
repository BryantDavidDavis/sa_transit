//
//  transit_graph_build.c
//  sa_transit
//
//  Created by Bryant Davis on 11/5/14.
//  Copyright (c) 2014 Bryant Davis. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "transit_graph_build.h"

//struct vertex_storage* vertices = malloc(sizeof(struct vertex_storage));
//struct vertex_storage* vertices;

struct vertex* build_vertex(int stop_id, float latitude, float longitude, char* name) {
    struct vertex* new_vertex = malloc(sizeof(struct vertex));
    if (new_vertex) {
        new_vertex->name = malloc(sizeof(char) * INITIAL_STOP_NAME_LENGTH);
        if (new_vertex->name) {
            new_vertex->stop_id = stop_id;
            new_vertex->latitude = latitude;
            new_vertex->longitude = longitude;
            if (name) {
                strcpy(name, new_vertex->name);
            } else {
                new_vertex->name = NULL;
            }
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }
    return new_vertex;
}

struct vertex_storage* build_vertex_storage() {
    struct vertex_storage* vertices = malloc(sizeof(struct vertex_storage));
    if (vertices) {
        vertices->contents = malloc(sizeof(struct vertex*) * INITIAL_VERTEX_STORE_CAP);
        if (vertices->contents) {
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

//we don't need this function for now, because we know how many the maximum number of stops is
//int vertex_storage_reallocate(struct vertex_storage** old_storage) {
//    struct vertex_storage* new_storage = malloc(sizeof(struct vertex_storage));
//    if (new_storage) {
//        new_storage->contents = malloc(sizeof(struct vertex*) * ((*old_storage)->capacity) * 2);
//        if (new_storage->contents) {
//            new_storage->size = 0;
//            for (unsigned long i = 0; i < (*old_storage)->size; i++) {
//                new_storage->contents[i] = (*old_storage)->contents[i];
//                new_storage->size++;
//            }
//            for (unsigned long i = new_storage->size; i < new_storage->capacity; i++) {
//                new_storage->contents[i] = NULL;
//            }
//            free(*old_storage);
//            *old_storage = new_storage;
//            return 1;
//        } else {
//            return 0;
//        }
//    } else {
//        return 0;
//    }
//}

int add_vertex_to_storage(struct vertex* my_vertex, struct vertex_storage** vertices) {
    if ((*vertices)->contents[my_vertex->stop_id] == NULL) {
        (*vertices)->contents[my_vertex->stop_id] = my_vertex;
        (*vertices)->size++;
        return 1;
    } else {
        return 0; //already in there
    }
    //no need to reallocate for now because we know the max capacity
    //    if ((*vertices)->size == (*vertices)->capacity) {
//        if(!vertex_storage_reallocate(vertices)) {
//            return 0;
//        } else {
//            (*vertices)->contents[(*vertices)->size] = my_vertex;
//            (*vertices)->size++;
//            return 1;
//        }
//    }
//    return 0;
}

struct vertex* new_node;

int graph_build_vertices_edges () {
    //system("pwd");
    FILE* fp;
    fp = fopen("/Users/bryantdavis/code/practice/C/sa_transit/sa_transit/stop_times.txt", "r");
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
    if (vertices == NULL) {
        printf("storage couldn't be built");
        fclose(fp);
        return 0;
    } else {
        
        while((first_letter = getc(fp)) != '\n') {
            printf("%c", first_letter);
        }; //advances the pointer to the first character of the second line
        int line_number = 1;
        
        for (int i = 0; i < 900; i++) {
            line_number++;
            printf("printing line number %d\t", line_number);
            
            got = fscanf(fp, "%d,%[^,],%[^,],%d,%d,,,,", &trip_id, arrival_time, departure_time, &stop_id, &stop_seq);
            if (got != 5) {
                printf("error reading file at line %d", i);
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
            //build_vertex(stop_id, 0, 0, 0); //this works
            printf("%d\n", add_vertex_to_storage(build_vertex(stop_id, 0, 0, 0), &vertices));
            
        }

    }
    fclose(fp);
    return 1;
}


