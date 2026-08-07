/*
 * Exit.h
 * -----------------------------------------------------------------------
 * Author: Sifan Terefe
 * Purpose: Public interface for the Exit/Utility Module. Handles display
 *          of final simulation results (champion, knockout results,
 *          championship path), the restart/exit prompt, and all memory
 *          reset/cleanup tied to that lifecycle.
 *
 * Depends on: Team.h (Team), bracket.h (Bracket, Round, RoundStage,
 *             round_name), match.h (Match) - bracket.h already pulls in
 *             both Team.h and match.h, so this file only needs to
 *             include bracket.h directly.
 * -----------------------------------------------------------------------
 */

#ifndef EXIT_H
#define EXIT_H

#include "bracket.h" /* brings in Team.h and match.h transitively */
#include "Team.h"

 /* ---------------------------------------------------------------------
  * Complex data type owned by this module (REQ-SYS-030)
  * Holds the champion's extracted match history separately from the
  * bracket's data, so this module can allocate/free it on its own.
  *
  * pathRounds is a parallel array to path: pathRounds[i] is the
  * RoundStage value (ROUND_OF_32 .. FINAL_ROUND, i.e. 0-4) for path[i].
  * Tracked here rather than as a field on Match, since the real Match
  * struct (owned by the Match Simulation module) has no round field -
  * round is otherwise only implicit in which bracket->rounds[r] a match
  * lives in. Use round_name() from bracket.h to turn a pathRounds value
  * into a display string, same as printKnockoutResults does.
  * ------------------------------------------------------------------- */
typedef struct ChampionshipRecord {
    const Team* champion;  /* pointer only - not owned/freed here */
    Match* path;      /* heap array, owned by this module     */
    int* pathRounds;/* heap array, parallel to path         */
    int         pathLength;/* number of matches in path            */
} ChampionshipRecord;


/*
 * Author: Sifan Terefe
 * Input(s): const Team *champion - pointer to the winning team
 * Output(s): none (writes to stdout)
 * Purpose: Print the tournament champion's name, group, and final record.
 */
void printChampion(const Team* champion);

/*
 * Author: Sifan Terefe
 * Input(s): const Bracket *bracket - the completed tournament bracket
 * Output(s): none (writes to stdout)
 * Purpose: Print every knockout-stage matchup and its score, grouped
 *          and headed by round (Round of 32, Round of 16, Quarterfinal,
 *          Semifinal, Final - TOTAL_ROUNDS in this bracket is 5; there
 *          is no Third-Place Match in the current Bracket module,
 *          though the SRS lists one - worth raising with the team),
 *          in the order the bracket already stores them.
 */
void printKnResults(const Bracket* bracket);

/*
 * Author: Sifan Terefe
 * Input(s): const Bracket *bracket (the completed tournament bracket),
 *           const Team *champion (the winner)
 * Output(s): ChampionshipRecord containing a malloc'd array of just the
 *            champion's matches (and their RoundStage values), in round
 *            order
 * Purpose: Extract the champion's matches out of the bracket's
 *          round-grouped match data into their own heap block sized to
 *          only what's needed.
 */
ChampionshipRecord buildChampionshipPath(const Bracket* bracket,
    const Team* champion);

/*
 * Author: Sifan Terefe
 * Input(s): const ChampionshipRecord *record
 * Output(s): N/A
 * Purpose: Print the champion's full path to the title (round, opponent,
 *          score) for every match in record->path.
 */
void printChampionshipPath(const ChampionshipRecord* record);

/*
 * Author: Sifan Terefe
 * Input(s): ChampionshipRecord *record
 * Output(s): N/A
 * Purpose: Free the heap arrays allocated in buildChampionshipPath and
 *          null both pointers so they can't be freed twice.
 */
void freeMatchData(ChampionshipRecord* record);

/*
 * Author: Sifan Terefe
 * Input(s): N/A
 * Output(s): int - 1 if the user chose to restart, 0 if they chose to exit
 * Purpose: Prompt the user to relaunch the simulation or exit, validating
 *          input until a valid choice is entered.
 */
int promptUserRestart(void);

/*
 * Author: Sifan Terefe
 * Input(s): Team teams[] - array of all teams, int numTeams - array
 *           length, Bracket *bracket - the tournament bracket
 * Output(s): none
 * Purpose: Zero out every team's stats and clear the bracket so a new
 *          simulation can run without restarting the program. The
 *          Bracket struct owns no heap memory (its Round/Match arrays
 *          are fixed-size, stored by value), so this is a straight
 *          memset rather than a series of frees.
 */
void resetSimData(Team teams[], int numTeams, Bracket* bracket);

/*
 * Author: Sifan Terefe
 * Input(s): Team *teams - team array, Bracket *bracket - the bracket
 *           (unused - kept for interface symmetry with resetSimData/
 *           exitSim; Bracket owns no heap memory to free)
 * Output(s): none
 * Purpose: Free every remaining dynamically allocated structure before
 *          the program terminates.
 */
void cleanupMemory(Team* teams, Bracket* bracket);

/*
 * Author: Sifan Terefe
 * Input(s): Team *teams, Bracket *bracket, ChampionshipRecord *record
 * Output(s): none - does not return
 * Purpose: Free all remaining memory and perform the program's clean
 *          exit via exit().
 */
void exitSim(Team* teams, Bracket* bracket, ChampionshipRecord* record);

/*
 * Author: Sifan Terefe
 * Input(s): Team teams[] - array of all teams, int numTeams - array
 *           length, Bracket *bracket - the tournament bracket
 * Output(s): none
 * Purpose: Reset shared data so Main's loop can start a fresh simulation.
 */
void restartSim(Team teams[], int numTeams, Bracket* bracket);

#endif /* EXIT_H */
