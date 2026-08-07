
#define INPUT_BUFFER_SIZE  64
#define MAX_LOG_PATH_LEN   256
#define DEFAULT_LOG_FILE   "Team_groups.txt"
#define MENU_COUNT         4
#define MAX_LABEL_LEN      48


typedef struct {
    FILE*   logFile;                       /* handle to the session log     */
    char    logFileName[MAX_LOG_PATH_LEN]; /* path used for this run        */
    bool    initialized;                   /* true once start-up succeeded  */\

} SystemContext;