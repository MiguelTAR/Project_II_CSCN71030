#include <stdio.h>
#include <stdlib.h>
#include "Exit.h"

void printChampion(const Team* champion) {
    if (champion == NULL) {
        printf("No Champion!\n");
        return;
    }
    printf("=== CHAMPION: %s (Group %c) ===\n", champion->name, champion->group);
    printf("Record: %dW-%dD-%dL, GF:%d GA:%d GD:%d\n",
        champion->wins, champion->draws, champion->losses,
        champion->goal_for, champion->goal_against);
}
