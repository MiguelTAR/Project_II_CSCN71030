#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "match.h"

//------------------------------------------------------------
// Simulates one match between two teams by generating
// random scores and storing the result in the Match structure.
//------------------------------------------------------------
void simulate_match(Team* team1, Team* team2, Match* match)
{
    match->team1 = team1;
    match->team2 = team2;

    printf("\n%s vs %s\n", team1->name, team2->name);

    printf("Who do you predict will win?\n");
    printf("1. %s\n", team1->name);
    printf("2. %s\n", team2->name);
    printf("3. Draw\n");

    do
    {
        printf("Enter your choice: ");

        if (scanf_s("%d", &match->prediction) != 1)
        {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");

            while (getchar() != '\n');

            match->prediction = 0;
        }
        else if (match->prediction < 1 || match->prediction > 3)
        {
            printf("Invalid choice. Please enter 1, 2, or 3.\n");
        }

    } while (match->prediction < 1 || match->prediction > 3);

    if (match->prediction == 1)
    {
        // Team 1 wins
        match->team1Score = (rand() % 7) + 1;
        match->team2Score = rand() % match->team1Score;
    }
    else if (match->prediction == 2)
    {
        // Team 2 wins
        match->team2Score = (rand() % 7) + 1;
        match->team1Score = rand() % match->team2Score;
    }
    else
    {
        // Draw
        int score = rand() % 8;
        match->team1Score = score;
        match->team2Score = score;
    }

    update_team_stats(match);
}

//------------------------------------------------------------
// Updates the statistics for both teams after a match.
//------------------------------------------------------------
void update_team_stats(Match* match)
{
    match->team1->matches_played++;
    match->team2->matches_played++;

    if (match->team1Score > match->team2Score)
    {
        match->team1->wins++;
        match->team2->losses++;
    }
    else if (match->team2Score > match->team1Score)
    {
        match->team2->wins++;
        match->team1->losses++;
    }
    else
    {
        match->team1->draws++;
        match->team2->draws++;
    }
}

//------------------------------------------------------------
// Prints the completed match result.
//------------------------------------------------------------
void print_match_results(const Match* match)
{
    printf("\n====================================\n");
    printf("%s %d - %d %s\n",
        match->team1->name,
        match->team1Score,
        match->team2Score,
        match->team2->name);

    if (match->team1Score > match->team2Score)
    {
        printf("Winner: %s\n", match->team1->name);
    }
    else if (match->team2Score > match->team1Score)
    {
        printf("Winner: %s\n", match->team2->name);
    }
    else
    {
        printf("Result: Draw\n");
    }

    printf("====================================\n");
}