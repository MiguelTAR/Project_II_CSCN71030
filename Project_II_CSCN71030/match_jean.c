#include <stdio.h>
#include "match.h"

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
    print_all(group, 4);
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
    print_all(group, 4);
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
    print_all(group, 4);
}
