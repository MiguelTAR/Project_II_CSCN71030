/*
 * Exit.c
 * -----------------------------------------------------------------------
 * Author: Sifan Terefe
 * Purpose: Implementation of the Exit/Utility Module. See Exit.h
 *          for the public interface.
 * -----------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Exit.h"
#include "match.h"

 /*
  * Author: Sifan Terefe
  * Input(s): const Team *champion - pointer to the winning team
  * Output(s): none (writes to stdout)
  * Purpose: Print the tournament champion's name, group, and final record.
  */
void printChampion(const Team* champion) {
    if (champion == NULL) {
        printf("No champion recorded.\n");
        return;
    }
    printf("=== CHAMPION: %s (Group %c) ===\n", champion->name, champion->group);
}

/*
 * Author: Sifan Terefe
 * Input(s): const Bracket *bracket - the completed tournament bracket
 * Output(s): none (writes to stdout)
 * Purpose: Print every knockout-stage matchup and its score, grouped
 *          and headed by round, in the order the bracket already
 *          stores them. Uses round_name() from bracket.h rather than
 *          maintaining a separate round-name list here.
 */
void printKnResults(const Bracket* bracket) {
    for (int r = 0; r < TOTAL_ROUNDS; r++) {
        printf("%s\n", round_name(r));
        for (int i = 0; i < bracket->rounds[r].matchCount; i++) {
            Match m = bracket->rounds[r].matches[i];
            if (m.team1 == NULL || m.team2 == NULL) continue; /* not yet filled */
            printf("%s %d - %d %s\n", m.team1->name, m.team1Score, m.team2Score, m.team2->name);
        }
        printf("\n");
    }
}

/*
 * Author: Sifan Terefe
 * Input(s): const Bracket *bracket - the completed tournament bracket,
 *           const Team *champion - the winning team
 * Output(s): ChampionshipRecord containing a malloc'd array of just the
 *            champion's matches (and a parallel array of RoundStage
 *            values), in round order
 * Purpose: Walk the bracket's round-grouped match data and extract only
 *          the champion's matches into their own heap block, recording
 *          which round each one belongs to (Match itself has no round
 *          field - round is otherwise only implicit in which
 *          bracket->rounds[r] a match lives in).
 */
ChampionshipRecord buildChampionshipPath(const Bracket* bracket, const Team* champion) {
    ChampionshipRecord record = { champion, NULL, NULL, 0 };
    int count = 0;

    for (int r = 0; r < TOTAL_ROUNDS; r++)
        for (int i = 0; i < bracket->rounds[r].matchCount; i++)
            if (bracket->rounds[r].matches[i].team1 == champion
                || bracket->rounds[r].matches[i].team2 == champion)
                count++;

    if (count == 0) {
        return record; /* champion has no recorded matches yet */
    }

    record.path = (Match *)malloc(count * sizeof(Match));
    record.pathRounds = (int *)malloc(count * sizeof(int));
    if (record.path == NULL || record.pathRounds == NULL) {
        printf("Warning: could not allocate championship path.\n");
        free(record.path);
        free(record.pathRounds);
        record.path = NULL;
        record.pathRounds = NULL;
        return record;
    }

    int j = 0;
    for (int r = 0; r < TOTAL_ROUNDS; r++) {
        for (int i = 0; i < bracket->rounds[r].matchCount; i++) {
            Match m = bracket->rounds[r].matches[i];
            if (m.team1 == champion || m.team2 == champion) {
                record.path[j] = m;
                record.pathRounds[j] = r; /* RoundStage: 0=RO32 ... 4=Final */
                j++;
            }
        }
    }
    record.pathLength = count;
    return record;
}

/*
 * Author: Sifan Terefe
 * Input(s): const ChampionshipRecord *record
 * Output(s): none (writes to stdout)
 * Purpose: Print the champion's full path to the title (round, opponent,
 *          score) for every match in record->path.
 */
void printChampionshipPath(const ChampionshipRecord* record) {
    printf("--- %s's Path to the Title ---\n", record->champion->name);
    for (int i = 0; i < record->pathLength; i++) {
        Match m = record->path[i];
        Team* opponent = (m.team1 == record->champion) ? m.team2 : m.team1;
        int myScore = (m.team1 == record->champion) ? m.team1Score : m.team2Score;
        int oppScore = (m.team1 == record->champion) ? m.team2Score : m.team1Score;
        printf("%s vs %-20s  %d - %d\n", round_name(record->pathRounds[i]), opponent->name,
            myScore, oppScore);
    }
}

/*
 * Author: Sifan Terefe
 * Input(s): ChampionshipRecord *record
 * Output(s): none
 * Purpose: Free the heap arrays allocated in buildChampionshipPath and
 *          null both pointers so they can't be freed twice.
 */
void freeMatchData(ChampionshipRecord* record) {
    free(record->path);
    free(record->pathRounds);
    record->path = NULL;
    record->pathRounds = NULL;
    record->pathLength = 0;
}

/*
 * Author: Sifan Terefe
 * Input(s): none
 * Output(s): int - 1 if the user chose to restart, 0 if they chose to exit
 * Purpose: Prompt the user to relaunch the simulation or exit, validating
 *          input until a valid choice is entered.
 */
int promptUserRestart(void) {
    int choice;
    do {
        printf("Enter 1 to run a new simulation, 0 to exit: ");
        if (scanf_s("%d", &choice) != 1) {
            while (getchar() != '\n'); /* clear bad input */
            choice = -1;
        }
    } while (choice != 0 && choice != 1);
    return choice;
}

/*
 * Author: Sifan Terefe
 * Input(s): Team teams[], int numTeams, Bracket *bracket
 * Output(s): none
 * Purpose: Zero out every team's stats and clear the bracket so a new
 *          simulation can run without restarting the program.
 */
void resetSimData(Team teams[], int numTeams, Bracket* bracket) {
    for (int i = 0; i < numTeams; i++) {
        teams[i].matches_played = 0;
        teams[i].wins = teams[i].draws = teams[i].losses = 0;
    }
    memset(bracket, 0, sizeof(Bracket));
}

/*
 * Author: Sifan Terefe
 * Input(s): Team *teams - team array, Bracket *bracket - unused, kept
 *           for interface symmetry (Bracket owns no heap memory)
 * Output(s): none
 * Purpose: Free every remaining dynamically allocated structure before
 *          the program terminates.
 */
void cleanupMemory(Team* teams, Bracket* bracket) {
    (void)bracket; /* nothing to free on Bracket - see note above */
    free(teams);
}

/*
 * Author: Sifan Terefe
 * Input(s): Team *teams, Bracket *bracket, ChampionshipRecord *record
 * Output(s): none - does not return
 * Purpose: Free all remaining memory and perform the program's clean exit.
 */
void exitSim(Team* teams, Bracket* bracket, ChampionshipRecord* record) {
    freeMatchData(record);
    cleanupMemory(teams, bracket);
    printf("Thanks for playing. Goodbye!\n");
    exit(0);
}

/*
 * Author: Sifan Terefe
 * Input(s): Team teams[], int numTeams, Bracket *bracket
 * Output(s): none
 * Purpose: Reset shared data so Main's loop can start a fresh simulation.
 */
void restartSim(Team teams[], int numTeams, Bracket* bracket) {
    resetSimData(teams, numTeams, bracket);
}
