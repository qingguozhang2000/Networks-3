#include <stdio.h>
#include "project3.h"
#include "utilities.h"

void init_distance_table(struct distance_table &dt, struct NeighborCosts &neighbors, int datafor) {
    for (int nodenum = 0; nodenum < 4; nodenum++) {
        dt->costs[datafor][nodenum] = neighbors->NodeCosts[nodenum];
        printf("%d,", dt.costs[datafor][nodenum]);
    }
    printf("\n");
}