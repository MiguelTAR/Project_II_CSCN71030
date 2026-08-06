// Miguel Tarazona
// File Input and Output main c file
#include "FileIO.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include"Team.h"

//count the number of teams in the file.
int count_teams(const char* filename)
{
	FILE* file = fopen(filename, "r");// Open the file for reading
	if (file == NULL) {
		printf("Error opening file %s\n", filename);// Check if the file was opened successfully
		return -1;// Return -1 to indicate an error
}

	// while loop to read each line of the file and 
	// count the number of teams at first starting with 0
	int count = 0;
	char line[100];
	while (fgets(line, sizeof(line), file)) {
		count++;
	}
	fclose(file);
	return count;
}


// Load the teams from the file into an array of Team structs
Team* Load_teams(const char* filename, int* team_count)
{
	*team_count = count_teams(filename);
	if (*team_count <= 0) {
		return NULL;
	}

	Team* teams = malloc(sizeof(Team) * (*team_count));

	if (teams == NULL) {
		printf("Error allocating memory for teams\n");
		return NULL;
	}

	FILE* file = fopen(filename, "r");

	if (file == NULL) {
		free(teams);
		return NULL;
	}

	char line[100];
	int index = 0;

	// Read each line of the file and parse the team name and group
	while (fgets(line, sizeof(line), file)) {
		char group;
		char team_name[50];

		sscanf(line, "%49[^,], %c", &group, team_name);
		strcpy(teams[index].name, team_name);
		teams[index].group = group;

		teams[index].matches_played = 0;
		teams[index].wins = 0;
		teams[index].losses = 0;
		teams[index].draws = 0;

		index++;
	}
	fclose(file);
	return teams;
}

//	Save the results of the teams to a file
void save_results(const char* filename, const Team* teams, int team_count)
{
	FILE* file = fopen(filename, "w");
	if (file == NULL) {
		printf("Error opening file %s for writing\n", filename);
		return;
	}
	fprintf(file, "TEAM, MP, W, L, D\n");
	for(int i = 0; i < team_count; i++) {
		fprintf(file, "%s, %d, %d, %d, %d\n", teams[i].name, teams[i].matches_played, 
			teams[i].wins, teams[i].losses, teams[i].draws);
	}
	fclose(file);

	printf("Results saved %s\n", filename);
}
// dynamically free the memory allocated for the teams array
void free_teams(Team* teams)
{
	if (teams != NULL) {
		free(teams);
		printf("Memory freed for teams\n");
	}
}

/*This program is a C implementation of file input and output operations for managing a list of teams.
* It defines a Team struct to hold information about each team, including its name, group, and match statistics.
*/