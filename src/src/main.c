#include "graph.h"
#include "dijkstra.h"
#include "sort.h"
#include <locale.h>

/*
 * Main program: interactive menu to query the transport network.
 * Loads the graph from file and dispatches commands.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#ifdef _WIN32
#include <windows.h>
#endif

int main(int argc, char** argv) {
    /* basic argument check */
    if (argc != 2) { printf("Usage: %s metro.txt\n", argv[0]); return 1; }

    Graph* g = load_graph(argv[1]);
    if (!g) return 1;

    int choice;
    char buffer[128], start[128], end[128];
    do {
        printf("\n===== MENU RESEAU DE TRANSPORT =====\n");
        printf("1 - Afficher les informations d'une station\n");
        printf("2 - Lister les voisins d'une station\n");
        printf("3 - Calculer un chemin minimal\n");
        printf("4 - Afficher les stations triées par degré\n");
        printf("0 - Quitter\n");
        printf("Votre choix : ");
        if (scanf("%d", &choice) != 1) { printf("Erreur de saisie\n"); return 1; }
        getchar(); 

        switch (choice) {
            case 1:
                printf("Id ou nom de la station : ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                int id1 = -1;
                if (sscanf(buffer, "%d", &id1) != 1) id1 = get_id_by_name(g, buffer);
                print_station(g, id1);
                break;
            case 2:
                printf("Id ou nom de la station : ");
                fgets(buffer, sizeof(buffer), stdin);
                buffer[strcspn(buffer, "\n")] = 0;
                int id2 = -1;
                if (sscanf(buffer, "%d", &id2) != 1) id2 = get_id_by_name(g, buffer);
                list_neighbors(g, id2);
                break;
            case 3:
                printf("Station départ : ");
                fgets(start, sizeof(start), stdin);
                start[strcspn(start, "\n")] = 0;
                printf("Station arrivée : ");
                fgets(end, sizeof(end), stdin);
                end[strcspn(end, "\n")] = 0;
                dijkstra(g, start, end);
                break;
            case 4: {
                Degree* d = compute_degrees(g);
                if (!d) { printf("Erreur mémoire\n"); break; }
                int n = g->nb_stations;
                Degree* a = malloc(n * sizeof(Degree));
                Degree* b = malloc(n * sizeof(Degree));
                Degree* c = malloc(n * sizeof(Degree));
                memcpy(a, d, n * sizeof(Degree));
                memcpy(b, d, n * sizeof(Degree));
                memcpy(c, d, n * sizeof(Degree));
                long comp_s = 0, swap_s = 0, comp_i = 0, swap_i = 0, comp_q = 0, swap_q = 0;
                selection_sort(a, n, &comp_s, &swap_s);
                insertion_sort(b, n, &comp_i, &swap_i);
                quick_sort(c, 0, n-1, &comp_q, &swap_q);
                // show results
                print_sorted_degrees(g, a, n);
                // show stats
                printf("\nAlgorithm statistics:\n");
                printf("Selection: comps=%ld swaps=%ld\n", comp_s, swap_s);
                printf("Insertion: comps=%ld swaps=%ld\n", comp_i, swap_i);
                printf("Quick   : comps=%ld swaps=%ld\n", comp_q, swap_q);
                free(a); free(b); free(c); free(d);
                break;
            }
            case 0: printf("Au revoir\n"); break;
            default: printf("Choix invalide\n");
        }
    } while (choice != 0);

    free_graph(g);
    return 0;
}
