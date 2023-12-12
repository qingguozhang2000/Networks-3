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

    if (TraceLevel >= 1) {
        int Graph[4][4];
        for (int i=0; i<4; i++) {
            for (int j=0; j<4; j++) {
                Graph[i][j] = neighborCosts[i]->NodeCosts[j];
            }
        }
        Dijkstra(Graph, n, current_node);
        // printf("node %d current distance vector: ", current_node);
        // for (int i=0; i<n; i++) {
        //     int min_cost = INFINITY;
        //     for (int j=0; j<n; j++) {
        //         if (i == j) { continue; }
        //         int cost = dt->costs[i][j];
        //         if (cost < min_cost) {
        //             min_cost = cost;
        //         }
        //     }
        //     printf("%d ", min_cost);
        // }
        // printf("\n");
    }
}

/*********** Dikstra Algorithm ***********/

void Dijkstra(int Graph[MAX_NODES][MAX_NODES], int n, int current_node) {
  int cost[MAX_NODES][MAX_NODES], distance[MAX_NODES], pred[MAX_NODES];
  int visited[MAX_NODES], count, mindistance, nextnode, i, j;

  // Creating cost matrix
  for (i = 0; i < n; i++)
    for (j = 0; j < n; j++)
      if (Graph[i][j] == 0)
        cost[i][j] = INFINITY;
      else
        cost[i][j] = Graph[i][j];

  for (i = 0; i < n; i++) {
    distance[i] = cost[current_node][i];
    pred[i] = current_node;
    visited[i] = 0;
  }

  distance[current_node] = 0;
  visited[current_node] = 1;
  count = 1;

  while (count < n - 1) {
    mindistance = INFINITY;

    for (i = 0; i < n; i++)
      if (distance[i] < mindistance && !visited[i]) {
        mindistance = distance[i];
        nextnode = i;
      }

    visited[nextnode] = 1;
    for (i = 0; i < n; i++)
      if (!visited[i])
        if (mindistance + cost[nextnode][i] < distance[i]) {
          distance[i] = mindistance + cost[nextnode][i];
          pred[i] = nextnode;
        }
    count++;
  }

  // Printing the distance
  if (TraceLevel >= 1) {
    printf("At time t=%1.6f, ", clocktime);
    printf("node %d current distance vector: ", current_node);
    for (int i=0; i<n; i++) {
        printf("%d ", distance[i]);
    }
    printf("\n");
  }
}
