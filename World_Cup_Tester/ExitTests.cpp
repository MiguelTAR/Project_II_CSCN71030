#include "CppUnitTest.h"
#include <cstring>
#include <cstdio>
#include "pch.h"
#include "CppUnitTest.h"


extern "C" {
    #include "Exit.h"
}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ExitUtilityTests
{
    
    static std::string captureStdout(void (*action)(const Team*), const Team* arg)
    {
        FILE* tempOut = nullptr;
        tmpfile_s(&tempOut);

        FILE* originalStdout = stdout;
        stdout = tempOut;

        action(arg);

        fflush(tempOut);
        rewind(tempOut);

        char buffer[1024] = { 0 };
        size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, tempOut);
        buffer[bytesRead] = '\0';

        stdout = originalStdout;
        fclose(tempOut);

        return std::string(buffer);
    }

    static void makeSampleTeams(Team teams[4]) {
        strcpy_s(teams[0].name, "Canada");   teams[0].roundReached = 5;
        strcpy_s(teams[1].name, "Brazil");   teams[1].roundReached = 4;
        strcpy_s(teams[2].name, "Japan");    teams[2].roundReached = 2;
        strcpy_s(teams[3].name, "Peru");     teams[3].roundReached = 0;
        for (int i = 0; i < 4; i++) {
            teams[i].group = 'A';
            teams[i].wins = teams[i].draws = teams[i].losses = 0;
            teams[i].goalsFor = teams[i].goalsAgainst = teams[i].goalDiff = 0;
        }
    }

    TEST_CLASS(ExitUtilityTests)
    {
    public:

        // -------------------------------------------------------------
        // TC-EU-01 — REQ-EU-010
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_01_PrintChampion_DisplaysNameAndRecord)
        {
            Team champion;
            strcpy_s(champion.name, "Canada");
            champion.group = 'A';
            champion.wins = 6; champion.draws = 1; champion.losses = 0;
            champion.goalsFor = 14; champion.goalsAgainst = 3; champion.goalDiff = 11;

            std::string output = captureStdout(printChampion, &champion);

            Assert::IsTrue(output.find("Canada") != std::string::npos);
            Assert::IsTrue(output.find("6") != std::string::npos);
        }

        // -------------------------------------------------------------
        // TC-EU-02 — REQ-EU-020, REQ-EI-020
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_02_SortTeamsByRound_OrdersDescendingByRoundReached)
        {
            Team teams[4];
            makeSampleTeams(teams);
            Team unordered[4] = { teams[3], teams[1], teams[0], teams[2] };

            sortTeamsByRound(unordered, 4);

            Assert::AreEqual(0, strcmp(unordered[0].name, "Canada"));
            Assert::AreEqual(0, strcmp(unordered[1].name, "Brazil"));
            Assert::AreEqual(0, strcmp(unordered[2].name, "Japan"));
            Assert::AreEqual(0, strcmp(unordered[3].name, "Peru"));
            Assert::IsTrue(unordered[0].roundReached >= unordered[1].roundReached);
            Assert::IsTrue(unordered[1].roundReached >= unordered[2].roundReached);
            Assert::IsTrue(unordered[2].roundReached >= unordered[3].roundReached);
        }

        // -------------------------------------------------------------
        // TC-EU-03 — REQ-EU-030, REQ-EU-040, REQ-EI-010
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_03_BuildDisplayFreeChampionshipPath)
        {
            Team canada, brazil, japan;
            strcpy_s(canada.name, "Canada");
            strcpy_s(brazil.name, "Brazil");
            strcpy_s(japan.name, "Japan");

            Match fullHistory[3] = {
                { &canada, &japan,  2, 1, 3 },  // Canada involved (teamA)
                { &brazil, &canada, 0, 1, 4 },  // Canada involved (teamB)
                { &brazil, &japan,  1, 0, 2 }   // Canada NOT involved
            };

            ChampionshipRecord record = buildChampionshipPath(fullHistory, 3, &canada);

            Assert::AreEqual(2, record.pathLength);
            Assert::IsNotNull(record.path);
            for (int i = 0; i < record.pathLength; i++) {
                Assert::IsTrue(record.path[i].teamA == &canada
                    || record.path[i].teamB == &canada);
            }

            printChampionshipPath(&record); // exercises display path (no crash)

            freeMatchData(&record);
            Assert::IsNull(record.path);
            Assert::AreEqual(0, record.pathLength);
        }

        // -------------------------------------------------------------
        // TC-EU-04 — REQ-EU-050 (prompt), REQ-EI-030
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_04_PromptUserRestart_SkipsInvalidInput)
        {
            FILE* fakeInput = nullptr;
            errno_t err = tmpfile_s(&fakeInput);
            Assert::AreEqual(0, (int)err);

            fputs("5\n-1\nabc\n1\n", fakeInput);
            rewind(fakeInput);

            FILE* originalStdin = stdin;
            stdin = fakeInput;

            int result = promptUserRestart();

            stdin = originalStdin;
            fclose(fakeInput);

            Assert::AreEqual(1, result);
        }

        // -------------------------------------------------------------
        // TC-EU-05 — REQ-EU-050 (clean exit), REQ-EI-050
        // NOTE: exitSim() itself calls exit(), which would terminate the
        // test runner process — so this test validates the cleanup
        // functions exitSim relies on (cleanupMemory + freeMatchData)
        // directly, rather than calling exitSim(). The exit() call itself
        // is better verified with a manual/integration test.
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_05_CleanupMemory_FreesAllAllocations)
        {
            Team* teams = (Team*)malloc(4 * sizeof(Team));
            Bracket bracket;
            bracket.champion = &teams[0];
            for (int r = 0; r < 5; r++) {
                bracket.rounds[r] = (Match*)malloc(2 * sizeof(Match));
                bracket.roundCount[r] = 2;
            }

            Team canada;
            strcpy_s(canada.name, "Canada");
            Match fullHistory[1] = { { &canada, &canada, 1, 0, 5 } };
            ChampionshipRecord record = buildChampionshipPath(fullHistory, 1, &canada);
            Assert::IsNotNull(record.path);

            cleanupMemory(teams, &bracket);
            freeMatchData(&record);

            for (int r = 0; r < 5; r++) {
                Assert::IsNull(bracket.rounds[r]);
            }
            Assert::IsNull(record.path);
            // teams pointer itself was freed inside cleanupMemory — not
            // dereferenced again here, since that would be a use-after-free.
        }

        // -------------------------------------------------------------
        // TC-EU-06 — REQ-EU-060, REQ-EI-040
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_06_RestartSim_ResetsDataAndReturnsControl)
        {
            Team teams[4];
            makeSampleTeams(teams);
            teams[0].wins = 5;
            teams[0].goalsFor = 12;

            Bracket bracket;
            bracket.champion = &teams[0];
            for (int r = 0; r < 5; r++) {
                bracket.rounds[r] = (Match*)malloc(2 * sizeof(Match));
                bracket.roundCount[r] = 2;
            }

            restartSim(teams, 4, &bracket);
            // reaching this line at all confirms the function returned
            // normally rather than calling exit()

            for (int i = 0; i < 4; i++) {
                Assert::AreEqual(0, teams[i].wins);
                Assert::AreEqual(0, teams[i].goalsFor);
                Assert::AreEqual(0, teams[i].roundReached);
            }
            for (int r = 0; r < 5; r++) {
                Assert::IsNull(bracket.rounds[r]);
                Assert::AreEqual(0, bracket.roundCount[r]);
            }
            Assert::IsNull(bracket.champion);
        }
    };
}
