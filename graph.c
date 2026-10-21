#include <stdio.h>
#include "graph.h"

void displayGraph(Corridor edges[], int edgeCount){
    printf("\nAvailable Wildlife Corridors:\n ");
    for(int i=0;i<edgeCount;i++){
        if(edges[i].available==1)
        printf("Forest %d -> Forest %d | Cost: %d\n",edges[i].source,edges[i].destination,edges[i].cost);
    }
}