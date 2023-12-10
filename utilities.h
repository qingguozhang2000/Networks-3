#include "project3.h"

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

void init_distance_table(struct distance_table *dt, struct NeighborCosts *neighbors, int datafor);