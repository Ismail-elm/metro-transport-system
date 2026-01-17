/* Sorting algorithms and degree pair structure. */
#ifndef SORT_H
#define SORT_H
#include "graph.h"

typedef struct {
    int id;
    int degree;
} Degree;

Degree* compute_degrees(Graph* g);
void selection_sort(Degree* array, int n, long* comparisons, long* swaps);
void insertion_sort(Degree* array, int n, long* comparisons, long* swaps);
void quick_sort(Degree* array, int left, int right, long* comparisons, long* swaps);
void print_sorted_degrees(Graph* g, Degree* array, int n);

#endif
