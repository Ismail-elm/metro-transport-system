#ifndef DIJKSTRA_H
#define DIJKSTRA_H
#include "graph.h"

void dijkstra(Graph* g, const char* start_name_or_id, const char* end_name_or_id);
void compute_dijkstra_from(Graph* g, int start, int* dist, int* prev);

#endif
