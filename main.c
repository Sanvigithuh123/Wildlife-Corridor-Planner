#include <stdio.h>
#include "graph.h"
#include "habitat.h"

int main(){

    Corridor edges[] = {
        {0, 1, 4, 50, 1},
        {0, 2, 2, 30, 1},
        {1, 3, 3, 40, 1},
        {2, 3, 5, 25, 1},
        {0, 3, 7, 60, 1}
    };
    int edgeCount=sizeof(edges)/sizeof(edges[0]);
    int choice;
    do{
        printf("\n====WILDLIFE CORRIDOR PLANNER====\n");
        printf("1. Display Corridors\n");
        printf("0. Exit\n");
        if(scanf("%d",&choice)!=1){
            printf("Invalid Input\n");
            return 1;
        }
        switch(choice){
            case 1:
            displayGraph()
        }
    }

    return 0;
}