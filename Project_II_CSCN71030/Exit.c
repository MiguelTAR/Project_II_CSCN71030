#include <stdio.h>
#include <stdlib.h>
#include "Exit.h"

void printChampion(const Team* champion) {
    if (champion == NULL) {
        printf("No Champion!\n");
        return;
    }
    printf("=== WORLD CHAMPION: %s (Group %c) ===\n", champion->name, champion->group);
    printf("Record: %dW-%dD-%dL\n",
        champion->wins, champion->draws, champion->losses);
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
    for (int i = 0; i <= numMatches; i++) {
        if (fullHistory[i].team1 == champion || fullHistory[i].team2 == champion) {
            record.path[j++] = fullHistory[i];
        }
    }
    record.pathLength = count;
    return record;
}

void printKnResults(const Bracket* bracket)
{
    const char* roundNames[5] = {
        "Round of 32", "Round of 16", "Quarterfinal", "Semifinal", "Final"
    };

    for (int r = 0; r < 5; r++) {
        printf("%s\n", roundNames[r]);
        for (int i = 0; i < bracket->roundCount[r]; i++) {
            Match m = bracket->rounds[r][i];
            printf("%s %d - %d %s\n", m.team1->name, m.team1Score, m.team2Score, m.team2->name);
        }
        printf("\n");
    }
}

void printChampionshipPath(const ChampionshipRecord* record) {
    printf("--- %s's Path to the Title ---\n", record->champion->name);
    for (int i = 0; i < record->pathLength; i++) {
        Match m = record->path[i];
        Team* opponent = (m.team1 == record->champion) ? m.team2 : m.team1;
        int myScore = (m.team2 == record->champion) ? m.team1Score : m.team2Score;
        int oppScore = (m.team1 == record->champion) ? m.team1Score : m.team2Score;
        if (i <= 1)
        {
            int round = 32 / (i + 1);
            printf("Round %d vs %-20s  %d - %d\n", round, opponent->name,
                myScore, oppScore);
        }
        else if (i = 2)
        {
            printf("Quarterfinal vs %-20s  %d - %d\n", opponent->name,
                myScore, oppScore);
        }
        else if (i = 3)
        {
            printf("Semifinal vs %-20s  %d - %d\n", opponent->name,
                myScore, oppScore);
        }
        else
        {
            printf("Final vs %-20s  %d - %d\n", opponent->name,
                myScore, oppScore);
        }
        
    }
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

void resetSimData(Team teams[], int numTeams, Bracket* bracket
) {
    for (int i = 0; i < numTeams; i++) {
        teams[i].wins = teams[i].draws = teams[i].losses = 0;
        teams[i].goal_for = teams[i].goal_against = 0;
    }
    for (int r = 0; r < 5; r++) {
        free(bracket->rounds[r]);
        bracket->rounds[r] = NULL;
        bracket->roundCount[r] = 0;
    }
    bracket->champion = NULL;
}

void cleanupMemory(Team* teams, Bracket* bracket) {
    for (int r = 0; r < 5; r++) {
        free(bracket->rounds[r]);
        bracket->rounds[r] = NULL;
    }
    free(teams);
}

void exitSim(Team* teams
    ,Bracket* bracket, 
    ChampionshipRecord* record) {
    freeMatchData(record);
    cleanupMemory(teams, bracket
    );
    printf("Thanks for playing. Goodbye!\n");
    exit(0);
}

void restartSim(Team teams[], int numTeams, Bracket* bracket
    ) {
    resetSimData(teams, numTeams, bracket
    );
}