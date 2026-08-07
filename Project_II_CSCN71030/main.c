#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "main.h"
#include "Team.h"
#include "FileIO.h"
#include "match.h"
#include "bracket.h"
//#include "Exit.h"

static SystemContext *g_ctx = NULL;

static bool initialize_system(const char* logFileName);
static void cleanup_system(void);
static void displayMainMenu(void);
static bool is_valid_selection(int selection);
static void dispatchSelection(int selection, int* running);

static void load_teams(void);
static void predict_match(void);
static void simulate_tournament(void);


int main(int argc, char *argv[])
{
    const char* logFileName = DEFAULT_LOG_FILE;

    if (argc > 1) {
        if (strcmp(argv[1], "-h") == 0 || strcmp(argv[1], "--help") == 0) {
            print_usage(argv[0]);
            return EXIT_SUCCESS;
        }
        logFileName = argv[1];
    }

    /* REQ-SYS-060: Set up resources before anything else runs */
    if (!initialize_system(logFileName)) {
        fprintf(stderr, "System initialization failed.\n");
        return EXIT_FAILURE;
    }

    bool running = true;

    while (running) {
        /* REQ-MF-020 / REQ-MI-010: show all available options every pass */
        displayMainMenu();

        int selection = get_menu_selection();

        if (!is_valid_selection(selection)) {
            printf("\n Invalid selection. Please enter a number between %1 and %3.\n");
            continue; /* Redisplay the menu and try again */
        }
        dispatch_selection(selection, &running);
    }

    cleanup_system();
    return EXIT_SUCCESS;
}


static bool initilialize_system(const char* logFileName)
{
    /* REQ-SYS-030 / REQ-SYS-050: Heap-allocate the complex system-state
     * type instead of relying on a statically allocated struct instance. */

    g_ctx = (SystemContext*)calloc(1, sizeof(SystemContext));
    if (g_ctx == NULL) {
        fprintf(stderr, "Could not allocate system context.\n");
        return false;
    }

    snprintf(g_ctx->logFileName, MAX_LOG_PATH_LEN, "%s", logFileName);

    /* REQ-SYS-060: Supports both reading and appending on one handle */
    g_ctx->logFile = fopen_s(&g_ctx->logFile, g_ctx->logFileName, "a+");
    if (g_ctx->logFile == NULL) {
        fprintf(stderr, "Could not open log file '%s'.\n", g_ctx->logFileName);
        return false;
    }

    fprintf(g_ctx->logFile, "\n=== Session started ===");
    fflush(g_ctx->logFile);

    g_ctx->teams = Load_teams(g_ctx->logFileName, &g_ctx->teamCount);
    if (g_ctx->teams == NULL) {
        fprintf(stderr, "Could not load teams from '%s'.\n", g_ctx->logFileName);
        cleanup_system();
        return false;
    }

    g_ctx->initialized = true;
    printf("Initialized successfully.\n");
    return true;

}

static void cleanup_system(void)
{
    if (g_ctx == NULL) {
        return;
    }

    if (g_ctx->logFile != NULL) {
        fprintf(g_ctx->logFile, "=== Session ended ===\n");
        fclose(g_ctx->logFile);
        g_ctx->logFile = NULL;
    }

    free_teams(g_ctx->teams);
    g_ctx->teams = NULL;
    free(g_ctx);
    g_ctx = NULL;
}



static void displayMainMenu()
{
    printf("\n------------------------------------------------\n");
    printf("            MATCH PREDICTOR - MAIN MENU         \n");
    printf("------------------------------------------------\n");
    printf("1. Manage Teams\n");
    printf("2. Predict a Match\n");
    printf("3. Simulate a Tournament\n");
    printf("4. Exit\n");
    printf("------------------------------------------------\n");
    printf("Select an Option: ");
}


int get_menu_selection(void)
{
    int selection = 0;
    if (scanf_s("%d", &selection) != 1) {
        while (getchar() != '\n') {
        }
        return -1;
    }
    while (getchar() != '\n') {
    }
    return selection;
}


static int isValidSelection(int selection)
{
    return selection >= 1 && selection <= MENU_COUNT;
}

static void load_teams(void)
{
    if (g_ctx == NULL || g_ctx->teams == NULL) {
        printf("No teams loaded.\n");
        return;
    }

    printf("\n--- Loaded Teams (%d) ---\n", g_ctx->teamCount);
    print_all(g_ctx->teams, g_ctx->teamCount);
}


static void simulate_tournament(void)
{
    Bracket bracket;
    int round;

    if (g_ctx == NULL || g_ctx->teams == NULL || g_ctx->teamCount < 2) {
        printf("Need at least 2 teams loaded to simulate a tournament.\n");
        return;
    }

    initialize_bracket(g_ctx->teams, g_ctx->teamCount, &bracket);

    for (round = ROUND_OF_32; round <= FINAL_ROUND; round++) {
        if (bracket.rounds[round].matchCount == 0) {
            continue;
        }
        play_knockout_round(&bracket, round);
    }

    display_bracket(&bracket);

    Team* champion = determine_champion(&bracket);
    if (champion == NULL) {
        printf("Tournament finished without a champion.\n");
    }
}

static void dispatchSelection(int selection, int* running)
{
    switch (selection) {
    case 1:
        load_teams();
        break;
    case 2:
        predict_match();
        break;
    case 3:
        simulate_tournament();
        break;
    case 4:
        if (confirmExit()) {
            *running = false;
        }
        break;
    default:
        break;
    }
}

bool confirmExit(void)
{
    int choice = 0;
    printf("\nAre you sure you want to exit? (1 = yes, 0 = no): ");
    if (scanf_s("%d", &choice) != 1) {
        while (getchar() != '\n') {
        }
        return false;
    }
    while (getchar() != '\n') {
    }
    return choice == 1;
}

