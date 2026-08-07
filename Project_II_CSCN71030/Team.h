#ifndef Team_h
#define Team_h	

typedef struct Team {
	char name[50];
	char group;

	int matches_played;
	int wins;
	int losses;
	int draws;

	int goal_for;
	int goal_against;
} Team;

void initialize_team(Team* team, const char* name, char group);

void print_team(const Team* team);

void print_all(const Team* teams, int team_count);

#endif	