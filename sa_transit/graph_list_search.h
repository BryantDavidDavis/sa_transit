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
void graph_list_breadth_first_search(struct graph_list* my_graph);

#endif /* defined(__sa_transit__graph_list_search__) */
