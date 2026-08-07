#include <stdio.h>
#include "Team.h"
#include <string.h>

void initialize_team(Team* team, const char* name, char group)
{
	strcpy_s(team->name, sizeof(team->name), name);
	team->group = group;
	team->wins = 0;
	team->losses = 0;
	team->draws = 0;
	team->goal_for = 0;
	team->goal_against = 0;
}

void print_team(const Team* team)
{
	printf("%s Group: %c\n", team->name, team->group);
	printf("Wins: %d\n", team->wins);
	printf("Losses: %d\n", team->losses);
	printf("Draws: %d\n", team->draws);
	printf("Goals For: %d\n", team->goal_for);
	printf("Goals Against: %d\n", team->goal_against);

}


void print_all(const Team* teams, int num_teams)
{
	for (int i = 0; i < num_teams; i++)
	{
		print_team(&teams[i]);
	}

}


