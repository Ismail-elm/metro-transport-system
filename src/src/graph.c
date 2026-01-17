/* Graph data structures and file loading (lists of adjacency).
 * Simple hash table for name->id lookup is also provided.
 */
#include "graph.h"
#include "dijkstra.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define TABLE_SIZE 1031  

// simple hash function for strings
unsigned int hash(const char* str, int table_size) {
    unsigned int h = 0;
    while (*str) {
        h = (h * 31 + (unsigned char)(*str++)) % table_size;
    }
    return h;
}

// Insert a name->id mapping into the hash table
void insert_hash(Graph* g, const char* name, int id) {
    /* insert name->id mapping into hash table */
    unsigned int h = hash(name, g->table_size);
    HashNode* node = malloc(sizeof(HashNode));
    if (!node) return; /* allocation guard */
    strcpy(node->name, name);
    node->id = id;
    node->next = g->table[h];
    g->table[h] = node;
}

// Lookup station ID by name
int get_id_by_name(Graph* g, const char* name) {
    /* lookup id by name in hash table */
    unsigned int h = hash(name, g->table_size);
    HashNode* node = g->table[h];
    while (node) {
        if (strcmp(node->name, name) == 0) return node->id;
        node = node->next;
    }
    return -1;
}

// detects the maximum station ID in the file
static int max_station_id(const char* filename) {
    FILE* f = fopen(filename, "r"); 
    if (!f) return -1;
    char line[256];
    int max = -1;
    while (fgets(line, sizeof(line), f)) {
        if (strncmp(line, "STATION", 7) == 0) {
            int id;
            if (sscanf(line, "STATION;%d;", &id) == 1) {
                if (id > max) max = id;
            }
        }
    }
    fclose(f);
    return max + 1;
}

// Load graph from file
Graph* load_graph(const char* filename) {
    /* open and parse the input file to build the graph */
    FILE* f = fopen(filename, "r");
    if (!f) { perror("Erreur ouverture fichier"); return NULL; }

    Graph* g = malloc(sizeof(Graph));
    if (!g) { fclose(f); return NULL; }
    g->nb_stations = max_station_id(filename);
    if (g->nb_stations <= 0) {
        fprintf(stderr, "Erreur: fichier invalide ou aucune station trouvée\n");
        free(g);
        fclose(f);
        return NULL;
    }
    g->stations = calloc(g->nb_stations, sizeof(Station));
    g->table_size = TABLE_SIZE;
    g->table = calloc(TABLE_SIZE, sizeof(HashNode*));

    for (int i = 0; i < g->nb_stations; i++) {
        g->stations[i].id = i;
        strcpy(g->stations[i].name, "INCONNUE");
        g->stations[i].adj = NULL;
        g->stations[i].degree = 0;
    }

    char line[256];
    rewind(f);

    /* parse STATION and EDGE lines */
    while (fgets(line, sizeof(line), f)) {
        if (line[0] == '#' || strlen(line) < 2) continue;

        if (strncmp(line, "STATION", 7) == 0) {
            int id;
            char name[128];
            if (sscanf(line, "STATION;%d;%127[^\n]", &id, name) == 2) {
                if (id >= g->nb_stations || id < 0) {
                    printf("Erreur : id de station invalide: %d\n", id);
                    continue;
                }
                strcpy(g->stations[id].name, name);
                insert_hash(g, name, id);
            }
        }

        if (strncmp(line, "EDGE", 4) == 0) {
            int s, d, t;
            if (sscanf(line, "EDGE;%d;%d;%d", &s, &d, &t) == 3) {
                if (s >= g->nb_stations || d >= g->nb_stations || s < 0 || d < 0) {
                    printf("Erreur : arête invalide %d -> %d\n", s, d);
                    continue;
                }
                Edge* e = g->stations[s].adj;
                int found = 0;
                while (e) { if (e->dest == d) { found = 1; break; } e = e->next; }
                if (found) continue;
                add_edge(g, s, d, t);
            }
        }
    }

    int n = g->nb_stations;
    g->dists = malloc(n * sizeof(int*));
    g->prevs = malloc(n * sizeof(int*));
    if (!g->dists || !g->prevs) {
        perror("Erreur allocation matrices Dijkstra");
        fclose(f);
        free_graph(g);
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        g->dists[i] = malloc(n * sizeof(int));
        g->prevs[i] = malloc(n * sizeof(int));
        if (!g->dists[i] || !g->prevs[i]) {
            perror("Erreur allocation matrices Dijkstra");
            fclose(f);
            free_graph(g);
            return NULL;
        }
        compute_dijkstra_from(g, i, g->dists[i], g->prevs[i]);
    }

    fclose(f);
    return g;
}

// Add an edge to the graph
void add_edge(Graph* g, int src, int dest, int time) {
    Edge* e = malloc(sizeof(Edge));
    e->dest = dest;
    e->time = time;
    e->next = g->stations[src].adj;
    g->stations[src].adj = e;
    g->stations[src].degree++;
}

// Print station information
void print_station(Graph* g, int id) {
    if (id < 0 || id >= g->nb_stations) { printf("Station inexistante\n"); return; }
    Station* s = &g->stations[id];
    printf("Station %d : %s\n", s->id, s->name);
    printf("Degré : %d\n", s->degree);
    if (s->adj) printf("Arêtes sortantes :\n");
    for (Edge* e = s->adj; e; e = e->next)
        printf(" -> %d (%s) [%d min]\n", e->dest, g->stations[e->dest].name, e->time);
}

// List neighbors of a station
void list_neighbors(Graph* g, int id) {
    if (id < 0 || id >= g->nb_stations) { printf("Station inexistante\n"); return; }
    Edge* e = g->stations[id].adj;
    while (e) {
        printf("-> %d (%s) [%d min]\n", e->dest, g->stations[e->dest].name, e->time);
        e = e->next;
    }
}

// Free all allocated memory for the graph
void free_graph(Graph* g) {
    for (int i = 0; i < g->nb_stations; i++) {
        Edge* e = g->stations[i].adj;
        while (e) {
            Edge* tmp = e;
            e = e->next;
            free(tmp);
        }
    }
    // Free hash table
    for (int i = 0; i < g->table_size; i++) {
        HashNode* n = g->table[i];
        while (n) { HashNode* tmp = n; n = n->next; free(tmp); }
    }
    free(g->table);
    if (g->dists) {
        for (int i = 0; i < g->nb_stations; i++) free(g->dists[i]);
        free(g->dists);
    }
    if (g->prevs) {
        for (int i = 0; i < g->nb_stations; i++) free(g->prevs[i]);
        free(g->prevs);
    }
    free(g->stations);
    free(g);
}
