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

        neighborCosts[i] = malloc(sizeof(struct NeighborCosts));
        neighborCosts[i]->NodesInNetwork = n;
        for (int j=0; j<n; j++) {
            neighborCosts[i]->NodeCosts[j] = INFINITY;
        }

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

    for (int i = 0; i < n; i++) { // i = destNode
        if (i == current_node) { continue; }
        for (int j = 0; j < n; j++) { // j = viaNode
            if (j == current_node) { continue; }
            if (i == j) {
                dt->costs[i][i] = neighborCosts[current_node]->NodeCosts[i];
            } else {
                dt->costs[i][j] = INFINITY;
            }
        }
    }

    // // Initialize the distance table and other structures
    // for (int i = 0; i < n; i++) {
    //     if (i != current_node) {
    //         neighborCosts[i] = getNeighborCosts(i);
    //     }
    // }

    // for (int i = 0; i < n; i++) { // i = destNode
    //     if (i == current_node) { continue; }
    //     for (int j = 0; j < n; j++) { // j = viaNode
    //         if (j == current_node) { continue; }
    //         if (i == j) {
    //             dt->costs[i][i] = neighborCosts[current_node]->NodeCosts[i];
    //             continue;
    //         }

    //         int src_via_cost = neighborCosts[current_node]->NodeCosts[j];
    //         int via_dest_cost = neighborCosts[j]->NodeCosts[i];

    //         int total_cost = src_via_cost + via_dest_cost;
    //         if (total_cost > INFINITY) {
    //             total_cost = INFINITY;
    //         }

    //         dt->costs[i][j] = total_cost;
    //     }
    // }
}

void update_node(int current_node, struct NeighborCosts **neighborCosts, struct distance_table *dt, struct RoutePacket *rcvdpkt) {
    int n = neighborCosts[current_node]->NodesInNetwork;

    if (TraceLevel >= 1) {
        printf("At time t=%1.6f, ", clocktime);
        printf("rtupdate%d called, ", current_node);
        printf("by a pkt received from Sender id: %d.\n", rcvdpkt->sourceid);
    }

    for (int i=0; i<n; i++) {
        neighborCosts[rcvdpkt->sourceid]->NodeCosts[i] = rcvdpkt->mincost[i];
        dt->costs[rcvdpkt->sourceid][i] = rcvdpkt->mincost[i];
    }

    // for (int i = 0; i < n; i++) { // i = destNode
    //     for (int j = 0; j < n; j++) { // j = viaNode
    //         int src_via_cost = neighborCosts[current_node]->NodeCosts[j];
    //         int via_dest_cost = INFINITY;
    //         if (neighborCosts[j] != NULL) {
    //             via_dest_cost = neighborCosts[j]->NodeCosts[i];
    //         }

    //         int total_cost = src_via_cost + via_dest_cost;
    //         if (total_cost > INFINITY) {
    //             total_cost = INFINITY;
    //         }

    //         dt->costs[i][j] = total_cost;
    //     }
    // }

    if (TraceLevel >= 1) {
        printf("node %d current distance vector: ", current_node);
        for (int i=0; i<n; i++) {
            int min_cost = INFINITY;
            for (int j=0; j<n; j++) {
                if (i == j) { continue; }
                int cost = dt->costs[i][j];
                if (cost < min_cost) {
                    min_cost = cost;
                }
            }
            printf("%d ", min_cost);
        }
        printf("\n");
    }
}
