/* Degree computation and simple sorting implementations.
 * Provides selection, insertion and quicksort variants and basic
 * statistics counting for comparisons and swaps.
 */
#include "sort.h"
#include <stdio.h>
#include <stdlib.h>

Degree* compute_degrees(Graph* g) {
    /* build an array of (id, degree) pairs */
    Degree* arr = malloc(g->nb_stations * sizeof(Degree));
    if (!arr) return NULL;
    for (int i = 0; i < g->nb_stations; i++) {
        arr[i].id = i;
        arr[i].degree = g->stations[i].degree;
    }
    return arr;
}

// Swap two Degree elements
static void swap(Degree* a, Degree* b) { Degree tmp = *a; *a = *b; *b = tmp; }

// Selection sort (descending by degree)
void selection_sort(Degree* array, int n, long* comparisons, long* swaps) {
    *comparisons = *swaps = 0;
    for (int i = 0; i < n - 1; i++) {
        int max_idx = i;
        for (int j = i + 1; j < n; j++) {
            (*comparisons)++;
            if (array[j].degree > array[max_idx].degree) max_idx = j;
        }
        if (max_idx != i) { swap(&array[i], &array[max_idx]); (*swaps)++; }
    }
}

// Insertion sort (descending by degree)
void insertion_sort(Degree* array, int n, long* comparisons, long* swaps) {
    *comparisons = *swaps = 0;
    for (int i = 1; i < n; i++) {
        Degree key = array[i];
        int j = i - 1;
        while (j >= 0) {
            (*comparisons)++;
            if (array[j].degree < key.degree) break;
            array[j + 1] = array[j];
            (*swaps)++;
            j--;
        }
        array[j + 1] = key;
    }
}

// Quicksort (descending by degree)
void quick_sort(Degree* array, int left, int right, long* comparisons, long* swaps) {
    if (left >= right) return;
    int i = left, j = right;
    Degree pivot = array[(left + right) / 2];
    while (i <= j) {
        while (++(*comparisons) && array[i].degree > pivot.degree) i++;
        while (++(*comparisons) && array[j].degree < pivot.degree) j--;
        if (i <= j) { swap(&array[i], &array[j]); (*swaps)++; i++; j--; }
    }
    if (left < j) quick_sort(array, left, j, comparisons, swaps);
    if (i < right) quick_sort(array, i, right, comparisons, swaps);
}

// Print sorted degrees
void print_sorted_degrees(Graph* g, Degree* array, int n) {
    printf("\nStations triées par degré décroissant :\n");
    printf("---------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("Station %3d | %-30s | degré = %d\n",
               array[i].id, g->stations[array[i].id].name, array[i].degree);
    }
}
