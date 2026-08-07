#pragma once
#ifndef EXIT_H
#define EXIT_H

#include "team.h"
#include "bracket.h"
#include "match.h"

typedef struct {
    const Team* champion; 
    Match* path;      
    int pathLength;
} ChampionshipRecord;


void printChampion(const Team* champion);

void printKnResults(const Bracket *bracket);

ChampionshipRecord buildChampionshipPath(Match fullHistory[], int numMatches,
    const Team* champion);


void printChampionshipPath(const ChampionshipRecord *record);

void freeMatchData(ChampionshipRecord* record);

int promptUserRestart(void);

void resetSimData(Team teams[],
    int numTeams,
    Bracket* bracket
   );

void cleanupMemory(Team* teams,
    Bracket* bracket
    );

void exitSim(Team* teams,
    Bracket* bracket,
    ChampionshipRecord *record
    );

void restartSim(Team teams[],
    int numTeams,
    Bracket* bracket
   );

#endif
