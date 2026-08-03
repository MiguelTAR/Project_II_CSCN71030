#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "main.h"
#include "Team.h"

static SystemContext *g_ctx = NULL;

static void print_usage(const char* programName);
static bool initialize_system(const char* logFileName);
static void displayMainMenu(void);
static int  get_menu_selection(void);
static bool is_valid_selection(int selection);




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

    /* REQ-MF-010 / REQ-SYS-060: set up resources before anything else runs */
    if (!initialize_system(logFileName)) {
        fprintf(stderr, "[FATAL] System initialization failed. Cannot continue.\n");
        return EXIT_FAILURE;
    }

    bool running = true;

    while (running) {
        /* REQ-MF-020 / REQ-MI-010: show all available options every pass */
        displayMainMenu();

        /* REQ-MF-030: accept the user's choice via a validated input method */
        int selection = get_menu_selection();

        /* REQ-MF-040 / REQ-MI-030: reject bad input and let the user retry */
        if (!is_valid_selection(selection)) {
            printf("\n Invalid selection. Please enter a number between %1 and %3.\n");
            continue; /* redisplay the menu and try again */
        }

        /* REQ-MF-050 / REQ-MI-020: hand control to the chosen module */
        dispatch_selection(selection, &running);
    }

    return;

}


static void print_usage(const char* programName)
{
    printf("Usage: %s [log_file_path]\n", programName);
    printf("  log_file_path   Optional path for the session log (default: %s)\n",
        DEFAULT_LOG_FILE);
}




static bool initilialize_system(const char* logFileName)
{
    /* REQ-SYS-030 / REQ-SYS-050: heap-allocate the complex system-state
     * type instead of relying on a statically allocated struct instance. */
    g_ctx = (SystemContext*)calloc(1, sizeof(SystemContext));
    if (g_ctx == NULL) {
        fprintf(stderr, "[ERROR] Could not allocate system context.\n");
        return false;
    }


    snprintf(g_ctx->logFileName, MAX_LOG_PATH_LEN, "%s", logFileName);

    /* REQ-SYS-060: "a+" supports both reading and appending on one handle */
    g_ctx->logFile = fopen(g_ctx->logFileName, "a+");
    if (g_ctx->logFile == NULL) {
        fprintf(stderr, "[ERROR] Could not open log file '%s'.\n", g_ctx->logFileName);
        return false; /* cleanup_resources() (already registered) frees g_ctx */
    }

    /* REQ-SYS-060: read the file back to prove read + write both work */
    printf("Previous session log (%s):\n", g_ctx->logFileName);
    rewind(g_ctx->logFile);
    char line[INPUT_BUFFER_SIZE];
    bool anyLines = false;
    while (fgets(line, sizeof(line), g_ctx->logFile) != NULL) {
        printf("  %s", line);
        anyLines = true;
    }
    if (!anyLines) {
        printf("  (no previous sessions found)\n");
    }
    fseek(g_ctx->logFile, 0, SEEK_END); /* required before switching read->write */


    fprintf(g_ctx->logFile, "\n=== Session started ===");
    fflush(g_ctx->logFile);

    g_ctx->initialized = true;
    printf("System initialized successfully.\n");
    return true;
}






static void displayMainMenu()
{

    printf("------------------------------------------------\n");
    printf("            MATCH PREDICTOR - MAIN MENU         \n");
    printf("________________________________________________\n");
    printf("1. Manage Teams\n");
    printf("2. Predict a Match\n");
    printf("3. Simulate a Tournament\n");
    printf("Select an Option: \n");
  
    printf("_________________________________________________\n");
}




static int isValidSelection(int selection)
{
    return selection >= 1 && selection <= MENU_COUNT;
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
            *running = 0;
        }
        break;
    default:
        break;
    }
}


