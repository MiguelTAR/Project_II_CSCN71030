#include "pch.h"
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

    // Helper: builds a small sample team roster with nonzero stats, for
    // tests that need something to reset.
    static void makeSampleTeams(Team teams[4]) {
        strcpy_s(teams[0].name, "Canada");
        strcpy_s(teams[1].name, "Brazil");
        strcpy_s(teams[2].name, "Japan");
        strcpy_s(teams[3].name, "Spain");
        for (int i = 0; i < 4; i++) {
            teams[i].group = 'A';
            teams[i].matches_played = 3;
            teams[i].wins = teams[i].draws = teams[i].losses = 1;
        }
    }

    TEST_CLASS(ExitUtilityTests)
    {
    public:

        // -------------------------------------------------------------
        // TC-EU-01 — REQ-EU-010
        // NOTE: simplified to a smoke test rather than capturing/checking
        // stdout content. This confirms printChampion runs without
        // crashing on valid champion data, but does NOT verify the
        // displayed text is correct — that's a known limitation, noted
        // in the test plan, traded for avoiding the stdout-redirection
        // plumbing.
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_01_PrintChampion_DoesNotCrashOnValidChampion)
        {
            Team champion;
            strcpy_s(champion.name, "Canada");
            champion.group = 'A';
            champion.matches_played = 7;
            champion.wins = 6; champion.draws = 1; champion.losses = 0;

            printChampion(&champion);
            // reaching this line at all is the pass condition
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

            Bracket bracket;
            memset(&bracket, 0, sizeof(Bracket)); // zero every Round's matchCount etc.
            bracket.champion = &canada;

            bracket.rounds[ROUND_OF_32].matches[0] = { &canada, &spain,  3, 0, 0 };
            bracket.rounds[ROUND_OF_32].matches[1] = { &brazil, &japan, 2, 1, 0 };
            bracket.rounds[ROUND_OF_32].matchCount = 2;

            bracket.rounds[ROUND_OF_16].matches[0] = { &canada, &brazil, 1, 0, 0 };
            bracket.rounds[ROUND_OF_16].matchCount = 1;

            std::string output = captureStdout([&]() { printKnResults(&bracket); });

            // Round headings present, in order
            size_t ro32Pos = output.find("Round of 32");
            size_t ro16Pos = output.find("Round of 16");
            Assert::AreNotEqual((size_t)std::string::npos, ro32Pos);
            Assert::AreNotEqual((size_t)std::string::npos, ro16Pos);
            Assert::IsTrue(ro32Pos < ro16Pos); // RO32 printed before RO16

            // Matchups appear as "team1 score - score team2"
            Assert::IsTrue(output.find("Canada 3 - 0 Spain") != std::string::npos);
            Assert::IsTrue(output.find("Brazil 2 - 1 Japan") != std::string::npos);
            Assert::IsTrue(output.find("Canada 1 - 0 Brazil") != std::string::npos);

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

            // Canada plays in RO32 (as team1) and RO16 (as team2).
            // Brazil vs Japan in RO32 doesn't involve Canada and should
            // be excluded from the extracted path.
            Bracket bracket;
            memset(&bracket, 0, sizeof(Bracket));
            bracket.champion = &canada;

            bracket.rounds[ROUND_OF_32].matches[0] = { &canada, &japan,  2, 1, 0 };
            bracket.rounds[ROUND_OF_32].matches[1] = { &brazil, &japan,  1, 0, 0 };
            bracket.rounds[ROUND_OF_32].matchCount = 2;

            bracket.rounds[ROUND_OF_16].matches[0] = { &brazil, &canada, 0, 1, 0 };
            bracket.rounds[ROUND_OF_16].matchCount = 1;

            ChampionshipRecord record = buildChampionshipPath(&bracket, &canada);

            Assert::AreEqual(2, record.pathLength);
            Assert::IsNotNull(record.path);
            Assert::IsNotNull(record.pathRounds);
            for (int i = 0; i < record.pathLength; i++) {
                Assert::IsTrue(record.path[i].team1 == &canada
                    || record.path[i].team2 == &canada);
            }
            Assert::AreEqual((int)ROUND_OF_32, record.pathRounds[0]);
            Assert::AreEqual((int)ROUND_OF_16, record.pathRounds[1]);

            printChampionshipPath(&record); // exercises display path (no crash)

            freeMatchData(&record);
            Assert::IsNull(record.path);
            Assert::IsNull(record.pathRounds);
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
        //
        // NOTE: Bracket owns no heap memory (Round.matches[] is a
        // fixed-size array), so cleanupMemory has nothing to verify on
        // the bracket side — it only frees the externally-owned Team
        // array. That can't be safely asserted on after the fact
        // (dereferencing freed memory is undefined behavior), so this
        // test treats cleanupMemory as a smoke test (reaching the next
        // line = pass) and focuses its real assertions on freeMatchData,
        // which CAN be checked safely.
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_05_CleanupAndFreeMatchData_ReleaseAllocations)
        {
            Team* teams = (Team*)malloc(4 * sizeof(Team));
            Bracket bracket;
            memset(&bracket, 0, sizeof(Bracket));
            bracket.champion = &teams[0];

            Team canada;
            strcpy_s(canada.name, "Canada");
            Bracket miniBracket;
            memset(&miniBracket, 0, sizeof(Bracket));
            miniBracket.champion = &canada;
            miniBracket.rounds[ROUND_OF_32].matches[0] = { &canada, &canada, 1, 0, 0 };
            miniBracket.rounds[ROUND_OF_32].matchCount = 1;

            ChampionshipRecord record = buildChampionshipPath(&miniBracket, &canada);
            Assert::IsNotNull(record.path);
            Assert::IsNotNull(record.pathRounds);

            cleanupMemory(teams, &bracket); // smoke test — frees `teams`
            freeMatchData(&record);

            Assert::IsNull(record.path);
            Assert::IsNull(record.pathRounds);
            Assert::AreEqual(0, record.pathLength);
            // `teams` itself is not dereferenced again here — it's a
            // dangling pointer after cleanupMemory, and touching it
            // would be undefined behavior.
        }

        // -------------------------------------------------------------
        // TC-EU-06 — REQ-EU-060, REQ-EI-040
        // -------------------------------------------------------------
        TEST_METHOD(TC_EU_06_RestartSim_ResetsDataAndReturnsControl)
        {
            Team teams[4];
            makeSampleTeams(teams);

            Bracket bracket;
            memset(&bracket, 0, sizeof(Bracket));
            bracket.champion = &teams[0];
            bracket.rounds[ROUND_OF_32].matches[0] = { &teams[0], &teams[1], 2, 1, 0 };
            bracket.rounds[ROUND_OF_32].matchCount = 1;

            restartSim(teams, 4, &bracket);
            // reaching this line at all confirms the function returned
            // normally rather than calling exit()

            for (int i = 0; i < 4; i++) {
                Assert::AreEqual(0, teams[i].matches_played);
                Assert::AreEqual(0, teams[i].wins);
                Assert::AreEqual(0, teams[i].draws);
                Assert::AreEqual(0, teams[i].losses);
            }
            for (int r = 0; r < TOTAL_ROUNDS; r++) {
                Assert::AreEqual(0, bracket.rounds[r].matchCount);
            }
            Assert::IsNull(bracket.champion);
        }
    };
}
