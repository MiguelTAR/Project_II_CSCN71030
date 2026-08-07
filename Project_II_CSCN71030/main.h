#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdbool.h>
#include "Team.h"

#define INPUT_BUFFER_SIZE  64
#define MAX_LOG_PATH_LEN   256
#define DEFAULT_LOG_FILE   "team_groups.txt"
#define MENU_COUNT         4
#define MAX_LABEL_LEN      48

typedef struct {
    FILE*   logFile;
    char    logFileName[MAX_LOG_PATH_LEN];
    bool    initialized;
    Team*   teams;
    int     teamCount;
} SystemContext;

int  get_menu_selection(void);
bool confirmExit(void);

#endif
