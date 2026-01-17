/* Graph, stations, edges and hash table lookup. */
#ifndef GRAPH_H
#define GRAPH_H
#include <stdio.h>

// Edge structure
typedef struct Edge {
    int dest;         
    int time;         
    struct Edge* next;
} Edge;

// Station structure
typedef struct {
    int id;
    char name[128];
    Edge* adj;
    int degree;
} Station;

// Hash table node
typedef struct HashNode {
    char name[128];
    int id;
    struct HashNode* next;
} HashNode;

// Graph structure
typedef struct {
    Station* stations;
    int nb_stations;
    HashNode** table;   
    int table_size; 
    int** dists; 
    int** prevs; 
} Graph;

// Graph
Graph* load_graph(const char* filename);
void add_edge(Graph* g, int src, int dest, int time);
void print_station(Graph* g, int id);
void list_neighbors(Graph* g, int id);
void free_graph(Graph* g);

// Hash
unsigned int hash(const char* str, int table_size);
void insert_hash(Graph* g, const char* name, int id);
int get_id_by_name(Graph* g, const char* name);

#endif
