/* Simple Dijkstra implementation used to precompute shortest paths
 * (by travel time) from each source station. Results are stored
 * in Graph->dists and Graph->prevs for reuse.
 */
#include "dijkstra.h"
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int parse_station(Graph* g, const char* s) {
    int id = -1;
    if (sscanf(s, "%d", &id) == 1) return id;
    return get_id_by_name(g, s);
}

/* Compute single-source shortest paths (Dijkstra) from `start`. */
void compute_dijkstra_from(Graph* g, int start, int* dist, int* prev) {
    int n = g->nb_stations;
    for (int i = 0; i < n; i++) { dist[i] = INT_MAX; prev[i] = -1; }
    int* visited = calloc(n, sizeof(int));
    if (!visited) return;
    dist[start] = 0;

    for (int i = 0; i < n; i++) {
        int u = -1;
        for (int j = 0; j < n; j++)
            if (!visited[j] && (u == -1 || dist[j] < dist[u])) u = j;
        if (u == -1 || dist[u] == INT_MAX) break;
        visited[u] = 1;
        for (Edge* e = g->stations[u].adj; e; e = e->next) {
            if (dist[u] + e->time < dist[e->dest]) {
                dist[e->dest] = dist[u] + e->time;
                prev[e->dest] = u;
            }
        }
    }
    free(visited);
}

/* Display shortest-path (using precomputed tables) between two stations.
 * `start_str` and `end_str` can be an id or a name.
 */
void dijkstra(Graph* g, const char* start_str, const char* end_str) {
    int start = parse_station(g, start_str);
    int end = parse_station(g, end_str);
    if (start < 0 || start >= g->nb_stations || end < 0 || end >= g->nb_stations) {
        printf("Station introuvable\n");
        return;
    }

    int n = g->nb_stations;
    int* dist = NULL;
    int* prev = NULL;
    if (g->dists && g->prevs) {
        dist = g->dists[start];
        prev = g->prevs[start];
    }
    if (!dist || !prev) {
        printf("Erreur: Dijkstra non pré-calculé\n");
        return;
    }

    if (dist[end] == INT_MAX) { printf("Aucun chemin\n"); return; }

    int* path = malloc(n * sizeof(int));
    int len = 0;
    for (int v = end; v != -1; v = prev[v]) path[len++] = v;

    printf("Chemin minimal (%d min) :\n", dist[end]);
    for (int i = len - 1; i >= 0; i--) {
        printf("%s%s", g->stations[path[i]].name, i ? " -> " : "\n");
    }

    free(path);
}
