#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilities.h"

// struct distance_table dt;

void init_distance_table(struct distance_table *dt, struct NeighborCosts *neighbors, int datafor) {
    for (int nodenum = 0; nodenum < 4; nodenum++) {
        dt->costs[datafor][nodenum] = neighbors->NodeCosts[nodenum];
        printf("%d,", dt->costs[datafor][nodenum]);
    }
    printf("\n");
}

int** read_costs(const char *config_file_name, int *n) {
    char line[100];
    char *rc;

    FILE *fp = fopen(config_file_name, "r");
    if (fp == NULL) {
        perror("Unable to open NodeConfigurationFile");
        exit(1);
    }

    fgets(line, sizeof(line), fp);
    sscanf(line, "%d", n);
    if (*n > MAX_NODES) {
        printf("Unexpected number of nodes");
        exit(1);
    }

    int **matrix = malloc((*n) * sizeof(int *));
    for (int i=0; i<*n; i++) {
        int *row = malloc((*n) * sizeof(int));

        rc = fgets(line, sizeof(line), fp);
        printf("%s\n", line);
        if (rc == NULL) {
            perror("expected line of config data not found");
            exit(1);
        }

        char *token = strtok(line, ", ");
        int j = 0;
        while (j < *n && token != NULL) {
            row[j] = atoi(token);
            printf("%s\n", token);
            token = strtok(NULL, ", ");
            j++;
        }

        matrix[i] = row;
    }

    fclose(fp);

    return matrix;
}

void free_matrix(int n, int **matrix) {
    for (int i=0; i<n; i++) {
        free(matrix[i]);
    }

    free(matrix);
}

void print_costs(int n, int **matrix) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%d ", matrix[i][j]);
        }
        printf("\n");
    }

    printf("\n");
}
