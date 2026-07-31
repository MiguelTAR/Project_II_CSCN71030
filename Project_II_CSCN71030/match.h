#ifndef MATCH_H
#define MATCH_H

#include <stdio.h>
#include "team.h"

//------------------------------------------------------------
// Match Structure
// Stores the information for one simulated match.
//------------------------------------------------------------
typedef struct Match
{
    Team* team1;
    Team* team2;

    int team1Score;
    int team2Score;

    int prediction;

} Match;
//------------------------------------------------------------
// Shared Match Simulation Functions
// These functions are used by every group simulation.
//------------------------------------------------------------

// Simulates one match between two teams
void simulate_match(Team* team1, Team* team2, Match* match);

// Updates both teams' statistics after a match
void update_team_stats(Match* match);

// Displays the result of a completed match
void print_match_results(const Match* match);

//------------------------------------------------------------
// Group Simulation Functions
// Each teammate only implements the groups assigned to them.
//------------------------------------------------------------

// Zuwair
void simulate_group_c(Team group[4]);
void simulate_group_i(Team group[4]);
void simulate_group_l(Team group[4]);

// Miguel
void simulate_group_f(Team group[4]);
void simulate_group_g(Team group[4]);
void simulate_group_k(Team group[4]);

// Jean
void simulate_group_a(Team group[4]);
void simulate_group_h(Team group[4]);
void simulate_group_j(Team group[4]);

// Sifan
void simulate_group_b(Team group[4]);
void simulate_group_d(Team group[4]);
void simulate_group_e(Team group[4]);

#endif