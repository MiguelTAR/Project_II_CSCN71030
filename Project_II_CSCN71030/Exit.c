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

ChampionshipRecord buildChampionshipPath(Match fullHistory[], int numMatches,
    const Team* champion) {
    ChampionshipRecord record = { champion, NULL, 0 };
    int count = 0;

    for (int i = 0; i < numMatches; i++)
        if (fullHistory[i].team1 == champion || fullHistory[i].team2 == champion)
            count++;

    record.path = malloc(count * sizeof(Match));
    if (record.path == NULL) {
        printf("WARNING: Could not allocate championship path.\n");
        return record;
    }

    int j = 0;
    for (int i = 0; i < numMatches; i++) {
        if (fullHistory[i].team1 == champion || fullHistory[i].team2 == champion) {
            record.path[j++] = fullHistory[i];
        }
    }
    record.pathLength = count;
    return record;
}

void freeMatchData(ChampionshipRecord* record) {
    free(record->path);
    record->path = NULL;
    record->pathLength = 0;
}

int promptUserRestart(void) {
    int choice;
    do {
        printf("Enter 1 to run a new simulation, 0 to exit: ");
        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            choice = -1;
        }
    } while (choice != 0 && choice != 1);
    return choice;
}
