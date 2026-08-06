#include "CppUnitTest.h"
#include <cstring>
#include <cstdio>
#include <io.h>
#include <functional>


extern "C" {
    #include "../Project_II_CSCN71030/Exit.h"
    #include "../Project_II_CSCN71030/Team.h"

}

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ExitUtilityTests
{
    // Helper: redirects stdout to a temp file at the OS file-descriptor
    // level (stdout itself is a non-modifiable macro under MSVC's CRT,
    // so it can't be reassigned directly), runs the given action, then
    // returns the captured text so print-only functions (which have no
    // return value) can still be asserted on.
    static std::string captureStdout(std::function<void()> action)
    {
        FILE* tempOut = nullptr;
        tmpfile_s(&tempOut);

        fflush(stdout);
        int stdoutFd = _fileno(stdout);
        int savedFd = _dup(stdoutFd);
        _dup2(_fileno(tempOut), stdoutFd);

        action();

        fflush(stdout);
        _dup2(savedFd, stdoutFd);
        _close(savedFd);

        rewind(tempOut);
        char buffer[1024] = { 0 };
        size_t bytesRead = fread(buffer, 1, sizeof(buffer) - 1, tempOut);
        buffer[bytesRead] = '\0';
        fclose(tempOut);

        return std::string(buffer);
    }

    static void makeSampleTeams(Team teams[4]) {
        strcpy_s(teams[0].name, "Canada");
        strcpy_s(teams[1].name, "Brazil");
        strcpy_s(teams[2].name, "Japan");
        strcpy_s(teams[3].name, "Spain");
        for (int i = 0; i < 4; i++) {
            teams[i].group = 'A';
            teams[i].wins = teams[i].draws = teams[i].losses = 0;
            teams[i].goal_for = teams[i].goal_against = 0;
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

            std::string output = captureStdout([&]() { printChampion(&champion); });

            Assert::IsTrue(output.find("Canada") != std::string::npos);
            Assert::IsTrue(output.find("6") != std::string::npos);
        }

        // -------------------------------------------------------------
        // TC-EU-02 — REQ-EU-020, REQ-EI-020
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_02_PrintKnockoutResults_GroupsMatchupsByRound)
        {
            Team canada, brazil, japan, spain;
            strcpy_s(canada.name, "Canada");
            strcpy_s(brazil.name, "Brazil");
            strcpy_s(japan.name, "Japan");
            strcpy_s(spain.name, "Spain");

            Match ro32[2] = {
                { &canada, &spain,  3, 0, 1 },
                { &brazil, &japan, 2, 1, 1 }
            };
            Match ro16[1] = {
                { &canada, &brazil, 1, 0, 2 }
            };

            Bracket bracket;
            bracket.champion = &canada;
            bracket.rounds[0] = ro32;      bracket.roundCount[0] = 2; // Round of 32
            bracket.rounds[1] = ro16;      bracket.roundCount[1] = 1; // Round of 16
            bracket.rounds[2] = nullptr;   bracket.roundCount[2] = 0; // Quarterfinal
            bracket.rounds[3] = nullptr;   bracket.roundCount[3] = 0; // Semifinal
            bracket.rounds[4] = nullptr;   bracket.roundCount[4] = 0; // Final

            std::string output = captureStdout([&]() { printKnResults(&bracket); });

            // Round headings present, in order
            size_t ro32Pos = output.find("Round of 32");
            size_t ro16Pos = output.find("Round of 16");
            Assert::AreNotEqual((size_t)std::string::npos, ro32Pos);
            Assert::AreNotEqual((size_t)std::string::npos, ro16Pos);
            Assert::IsTrue(ro32Pos < ro16Pos); // RO32 printed before RO16

            // Matchups appear as "TeamA score - score TeamB"
            Assert::IsTrue(output.find("Canada 3 - 0 Spain") != std::string::npos);
            Assert::IsTrue(output.find("Brazil 2 - 1 Japan") != std::string::npos);
            Assert::IsTrue(output.find("Canada 1 - 0 Brazil") != std::string::npos);

            // Both matchup lines fall after the RO32 heading and before/around
            // the RO16 heading as appropriate
            size_t canSpainPos = output.find("Canada 3 - 0 Spain");
            Assert::IsTrue(canSpainPos > ro32Pos && canSpainPos < ro16Pos);
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
                Assert::IsTrue(record.path[i].team1 == &canada
                    || record.path[i].team2 == &canada);
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

            fflush(stdin);
            int stdinFd = _fileno(stdin);
            int savedStdinFd = _dup(stdinFd);
            _dup2(_fileno(fakeInput), stdinFd);

            int result = promptUserRestart();

            _dup2(savedStdinFd, stdinFd);
            _close(savedStdinFd);
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
            teams[0].goal_for = 12;

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
                Assert::AreEqual(0, teams[i].goal_for);
            }
            for (int r = 0; r < 5; r++) {
                Assert::IsNull(bracket.rounds[r]);
                Assert::AreEqual(0, bracket.roundCount[r]);
            }
            Assert::IsNull(bracket.champion);
        }
    };
}
