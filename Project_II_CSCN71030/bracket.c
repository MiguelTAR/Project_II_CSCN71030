#include <stdio.h>
#include <string.h>
#include <time.h>

#include "bracket.h"
#include "match.h"


const char* round_name(int round)
{
    switch (round) {
    case ROUND_OF_32:  return "Round of 32";
    case ROUND_OF_16:  return "Round of 16";
    case QUARTERFINAL: return "Quarterfinals";
    case SEMIFINAL:    return "Semifinals";
    case FINAL_ROUND:  return "Final";
    default:           return "Unknown Round";
    }
}


void initialize_bracket(Team teams[], int teamCount, Bracket* bracket)
{
    int i;

    if (bracket == NULL) {
        return;
    }

    /* Erase any previous tournament data so it doesn't leaks into this one */
    memset(bracket, 0, sizeof(Bracket));

    if (teams == NULL || teamCount <= 0) {
        printf("No qualified teams.\n");
        return;
    }

    if (teamCount > MAX_TEAMS) {
        printf("%d teams, only the first %d are used for the Round of 32.\n",
            teamCount, MAX_TEAMS);
        teamCount = MAX_TEAMS;
    }

    for (i = 0; i < teamCount; i++) {
        bracket->teams[i] = teams[i];
    }
    bracket->teamCount = teamCount;

    /* Build the Round of 32 pairings. */
    {
        Round* r32 = &bracket->rounds[ROUND_OF_32];
        int matchIndex = 0;
        int low = 0;
        int high = teamCount - 1;


        while (low < high) {
            Match* m = &r32->matches[matchIndex];

            m->team1 = &bracket->teams[low];
            m->team2 = &bracket->teams[high];
            m->team1Score = 0;
            m->team2Score = 0;
            m->prediction = 0;

            matchIndex++;
            low++;
            high--;
        }

        r32->matchCount = matchIndex;
        r32->completedCount = 0;
    }

    bracket->currentRound = ROUND_OF_32;
    bracket->champion = NULL;

    printf("Bracket initialized: %d teams placed into the %s.\n",
        bracket->teamCount, round_name(ROUND_OF_32));
}
 


void advance_winner(Bracket* bracket, Match* match)
{
    int    round;
    int    slot = -1;
    int    foundRound = -1;
    Round* currentRound;
    Round* nextRound;
    Match* nextMatch;
    Team*  winner;

    if (bracket == NULL || match == NULL) {
        return;
    }

    for (round = ROUND_OF_32; round <= FINAL_ROUND; round++) {
        Round* r = &bracket->rounds[round];
        for (int i = 0; i < r->matchCount; i++) {
            if (&r->matches[i] == match) {
                foundRound = round;
                slot = i;
                break;
            }
        }
        if (foundRound >= 0) {
            break;
        }
    }

    if (foundRound < 0) {
        return;
    }

    currentRound = &bracket->rounds[foundRound];

    if (!currentRound->played[slot]) {
        printf("Match has not been played yet.\n");
        return;
    }

    winner = currentRound->winners[slot];
    if (winner == NULL) {
        printf("No winner recorded for this match.\n");
        return;
    }

    if (foundRound >= FINAL_ROUND) {
        bracket->champion = winner;
        return;
    }

    nextRound = &bracket->rounds[foundRound + 1];
    nextMatch = &nextRound->matches[slot / 2];

    if ((slot % 2) == 0) {
        nextMatch->team1 = winner;
    }
    else {
        nextMatch->team2 = winner;
    }
}





void play_knockout_round(Bracket* bracket, int round)
{
    Round* r;
    int    i;
 
    if (bracket == NULL) {
        return;
    }
 
    if (round < ROUND_OF_32 || round > FINAL_ROUND) {
        printf("Invalid round %d.\n", round);
        return;
    }
 
    r = &bracket->rounds[round];
 
    if (r->matchCount == 0) {
        return;
    }
 
    printf("\n--- Simulating %s ---\n", round_name(round));
 
    for (i = 0; i < r->matchCount; i++) {
        Match* m = &r->matches[i];
 
        if (r->played[i]) {
            advance_winner(bracket, m);
            continue;
        }
 
        if (m->team1 == NULL || m->team2 == NULL) {
            printf("Advances automatically. \n");

        } else {
            /* Randomly roll scores until someone actually wins */
            do {
                m->team1Score = rand() % 5;   /* 0-4 goals, adjust as needed */
                m->team2Score = rand() % 5;
            } while (m->team1Score == m->team2Score);

            r->winners[i] = (m->team1Score > m->team2Score) ? m->team1 : m->team2;
            r->played[i] = 1;
            printf("%s advances\n", r->winners[i]->name);
        }
 
        r->completedCount++;
        advance_winner(bracket, m);
    }
 
    if (round < FINAL_ROUND) {
        bracket->rounds[round + 1].matchCount = (r->matchCount + 1) / 2;
        bracket->currentRound = round + 1;
    } else {
        bracket->currentRound = FINAL_ROUND;
        printf("\nThe Final has been completed.\n");
    }
}


void display_bracket(Bracket* bracket)
{
    int round;
    int i;

    if (bracket == NULL) {
        printf("Bracket is empty.\n");
        return;
    }

    printf("\n================= KNOCKOUT STAGE =================\n");

    for (round = ROUND_OF_32; round <= FINAL_ROUND; round++) {
        Round* r = &bracket->rounds[round];

        if (r->matchCount == 0) {
            continue; 
        }

        printf("\n-- %s --\n", round_name(round));

        for (i = 0; i < r->matchCount; i++) {
            Match* m = &r->matches[i];
            const char* name1 = (m->team1 != NULL) ? m->team1->name : "TBD";
            const char* name2 = (m->team2 != NULL) ? m->team2->name : "TBD";

            if (r->played[i]) {
                printf("  [%2d] %-15s %d - %d %-15s  winner: %s\n",
                    i + 1, name1, m->team1Score, m->team2Score, name2,
                    (r->winners[i] != NULL) ? r->winners[i]->name : "N/A");
            }
            else {
                printf("  [%2d] %-15s vs %-15s\n", i + 1, name1, name2);
            }
        }
    }

    if (bracket->champion != NULL) {
        printf("\n*** THE CHAMPION IS : %s ***\n", bracket->champion->name);
    }

    printf("======================================================\n");
}





Team* determine_champion(Bracket* bracket)
{
    Round* finalRound;

    if (bracket == NULL) {
        return NULL;
    }

    finalRound = &bracket->rounds[FINAL_ROUND];

    if (finalRound->matchCount == 0 || !finalRound->played[0]) {
        printf("Final has not been completed yet.\n");
        return NULL;
    }

    bracket->champion = finalRound->winners[0];
    return bracket->champion;
}