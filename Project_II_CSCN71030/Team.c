// Miguel Tarazona
// This program defines the Team structure and
// provides functions to initialize and print team information.
#include <stdio.h>
#include "Team.h"
#include <string.h>

// Function to create and initialize a Team structure.
void initialize_team(Team* team, const char* name, char group)
{
	strcpy(team->name, name);
	team->group = group;
	team->wins = 0;
	team->losses = 0;
	team->draws = 0;
}

// Function to print the information of a single team.
void print_team(const Team* team)
{
	printf("%s Group: %c\n", team->name, team->group);
	printf("Wins: %d\n", team->wins);
	printf("Losses: %d\n", team->losses);
	printf("Draws: %d\n", team->draws);
}

// Function to print the information of all teams in an array.
void print_all(const Team* teams, int num_teams)
{
	for (int i = 0; i < num_teams; i++)
	{
		print_team(&teams[i]);
	}

}

/* 
The reason for the team module is to provide a
clear and organized way to manage and display information about futbol teams. 
The Team structure holds essential data about each team, including its name,
group, and performance statistics (wins, losses, draws).
*/

