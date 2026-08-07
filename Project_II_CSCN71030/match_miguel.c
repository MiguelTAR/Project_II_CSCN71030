#include <stdio.h>
#include "match.h"

// Group F

void simulate_group_f(Team group[4])
{
    Match match;

    printf("\n========== GROUP F ==========\n");

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

    printf("\nFinal Group F Statistics\n");
    print_all(group, 4);
}

// Group G

void simulate_group_g(Team group[4])
{
    Match match;

    printf("\n========== GROUP G ==========\n");

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

    printf("\nFinal Group G Statistics\n");
    print_all(group, 4);
}

// Group K

void simulate_group_k(Team group[4])
{
    Match match;

    printf("\n========== GROUP K ==========\n");

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

    printf("\nFinal Group K Statistics\n");
    print_all(group, 4);
}
