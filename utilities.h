#include "project3.h"

struct distance_table {
  int costs[MAX_NODES][MAX_NODES];
};

void init_node(int current_node,struct NeighborCosts **neighborCosts, struct distance_table *dt);
void update_node(int current_node, struct NeighborCosts **neighborCosts, struct distance_table *dt, struct RoutePacket *rcvdpkt);
void Dijkstra(int Graph[MAX_NODES][MAX_NODES], int n, int start);
