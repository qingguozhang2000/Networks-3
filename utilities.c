#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "utilities.h"

extern int TraceLevel;
extern float clocktime;

void init_node(int current_node, struct NeighborCosts **neighborCosts, struct distance_table *dt) {
    if (TraceLevel >= 1) {
        printf("At time t=%1.6f, rtinit%d called.\n", clocktime, current_node);
    }
    // Find our neighbor nodes and the costs to those nodes
    neighborCosts[current_node] = getNeighborCosts(current_node);
    int n = neighborCosts[current_node]->NodesInNetwork;

    if (TraceLevel >= 1) {
        printf("At time t=%1.6f, ", clocktime);
        printf("node %d initial distance vector: ", current_node);
        for (int i=0; i<n; i++) {
            printf("%d ", neighborCosts[current_node]->NodeCosts[i]);
        }
        printf("\n");
    }

    struct RoutePacket packet;
    packet.sourceid = current_node;
    for (int j=0; j<n; j++) {
        packet.mincost[j] = neighborCosts[current_node]->NodeCosts[j];
    }

    for (int i = 0; i < n; i++) {
        if (i == current_node) { continue; }

        if (TraceLevel >= 1) {
            printf("At time t=%1.6f, ", clocktime);
            printf("node %d sends packet to node %d with: ", current_node, i);
            for (int i=0; i<n; i++) {
                printf("%d ", neighborCosts[current_node]->NodeCosts[i]);
            }
            printf("\n");
        }
        packet.destid = i;
        toLayer2(packet);
    }

    // Initialize the distance table and other structures
    for (int i = 0; i < n; i++) {
        if (i != current_node) {
            neighborCosts[i] = getNeighborCosts(i);
        }
    }

    for (int i = 0; i < n; i++) { // i = destNode
        if (i == current_node) { continue; }
        for (int j = 0; j < n; j++) { // j = viaNode
            if (j == current_node) { continue; }
            if (i == j) {
                dt->costs[i][i] = neighborCosts[current_node]->NodeCosts[i];
                continue;
            }

            int src_via_cost = neighborCosts[current_node]->NodeCosts[j];
            int via_dest_cost = neighborCosts[j]->NodeCosts[i];

            int total_cost = src_via_cost + via_dest_cost;
            if (total_cost > INFINITY) {
                total_cost = INFINITY;
            }

            dt->costs[i][j] = total_cost;
        }
    }
}
