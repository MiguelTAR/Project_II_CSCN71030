#include <stdio.h>
#include "Team.h"
#include <string.h>

void initialize_team(Team* team, const char* name, char group)
{
	strcpy(team->name, name);
	team->group = group;
	team->wins = 0;
	team->losses = 0;
	team->draws = 0;
	team->goal_for = 0;
	team->goal_against = 0;
}

