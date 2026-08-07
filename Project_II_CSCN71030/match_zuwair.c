#include <stdio.h>
#include "match.h"

//  Group C 
void simulate_group_c(Team group[4])
{
    Match match;

    printf("\n========== GROUP C ==========\n");

    simulate_match(&group[0], &group[1], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[2], &group[3], &match);
    print_match_results(&match);

    printf("\nFinal Group C Statistics\n");
}


// Group I 

void simulate_group_i(Team group[4])
{
    Match match;

    printf("\n========== GROUP I ==========\n");

    simulate_match(&group[0], &group[1], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[2], &group[3], &match);
    print_match_results(&match);

    printf("\nFinal Group I Statistics\n");
}

// Group L 

void simulate_group_l(Team group[4])
{
    Match match;

    printf("\n========== GROUP L ==========\n");

    simulate_match(&group[0], &group[1], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[2], &group[3], &match);
    print_match_results(&match);

    printf("\nFinal Group L Statistics\n");
}

// Group A

void simulate_group_a(Team group[4])
{
    Match match;

    printf("\n========== GROUP A ==========\n");

    simulate_match(&group[0], &group[1], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[2], &group[3], &match);
    print_match_results(&match);

    printf("\nFinal Group A Statistics\n");
}

// Group H

void simulate_group_h(Team group[4])
{
    Match match;

    printf("\n========== GROUP H ==========\n");

    simulate_match(&group[0], &group[1], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[2], &group[3], &match);
    print_match_results(&match);

    printf("\nFinal Group H Statistics\n");
}

// Group J

void simulate_group_j(Team group[4])
{
    Match match;

    printf("\n========== GROUP J ==========\n");

    simulate_match(&group[0], &group[1], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[0], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[2], &match);
    print_match_results(&match);

    simulate_match(&group[1], &group[3], &match);
    print_match_results(&match);

    simulate_match(&group[2], &group[3], &match);
    print_match_results(&match);

    printf("\nFinal Group J Statistics\n");
}