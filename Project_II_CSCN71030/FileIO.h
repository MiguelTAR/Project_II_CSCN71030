#ifndef fileio_h
#define fileio_h

#include "Team.h"

int count_teams(const char* filename);

Team* Load_teams(const char* filename, int* team_count);

void Save_results(const char* filename, const Team* teams, int team_count);


void free_teams(Team* teams);





#endif 
