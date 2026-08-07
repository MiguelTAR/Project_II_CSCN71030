#include <stdio.h>
#include "match.h"

//  Group B 
void simulate_group_b(Team group[4])
{
    Match match;

    printf("\n========== GROUP B ==========\n");

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

    printf("\nFinal Group B Statistics\n");
    print_all(group, 4);
}


// Group D

void simulate_group_d(Team group[4])
{
    Match match;

    printf("\n========== GROUP D ==========\n");

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

    printf("\nFinal Group D Statistics\n");
    print_all(group, 4);
}

// Group E 

void simulate_group_e(Team group[4])
{
    Match match;

    printf("\n========== GROUP E ==========\n");

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

    printf("\nFinal Group E Statistics\n");
    print_all(group, 4);
}