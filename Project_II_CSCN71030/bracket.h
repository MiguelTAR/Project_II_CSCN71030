#ifndef BRACKET_H
#define BRACKET_H

#include "Team.h"
#include "match.h"

#define MAX_TEAMS             32
#define MAX_MATCHES_PER_ROUND (64 / 2)
#define TOTAL_ROUNDS          5

typedef enum {
    ROUND_OF_32 = 0,
    ROUND_OF_16 = 1,
    QUARTERFINAL = 2,
    SEMIFINAL = 3,
    FINAL_ROUND = 4
} RoundStage;


typedef struct {
    Match matches[MAX_MATCHES_PER_ROUND];
    Team* winners[MAX_MATCHES_PER_ROUND];  /* who advances out of each match */
    int   played[MAX_MATCHES_PER_ROUND];   /* 1 once that match is resolved  */
    int   matchCount;                      /* matches in this round*/
    int   completedCount;                  /* matches so far       */
} Round;


typedef struct {
    Team  teams[MAX_TEAMS];  /* matches point in here */
    int   teamCount;
    Round rounds[TOTAL_ROUNDS];
    int   currentRound;      /* recent round reached */
    Team* champion;          /* NULL until the Final Round has been completed */
} Bracket;


void  initialize_bracket(Team teams[], int teamCount, Bracket* bracket);
void  advance_winner(Bracket* bracket, Match* match);
void  play_knockout_round(Bracket* bracket, int round);
void  display_bracket(Bracket* bracket);
Team* determine_champion(Bracket* bracket);


const char* round_name(int round);

#endif
