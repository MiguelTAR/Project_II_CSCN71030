#pragma once
#ifndef EXIT_H
#define EXIT_H

#include "team.h"
//#include "bracket.h"
#include "match.h"

void printChampion(Team* champion);

void printKnResults(Team teams[], int numTeams);

void printChampionshipPath(Match history[], int numMatches);

int promptUserRestart(void);

void resetSimData(Team teams[],
    int numTeams,
    //Bracket* bracket,
    Match history[],
    int* numMatches);

void freeMatchData(Match* history);

void cleanupMemory(Team* teams,
    Match* history
    //,Bracket* bracket
    );

void exitSim(Team* teams,
    Match* history
    //,Bracket* bracket
    );

void restartSim(Team teams[],
    int numTeams,
    //Bracket* bracket,
    Match history[],
    int* numMatches);

#endif