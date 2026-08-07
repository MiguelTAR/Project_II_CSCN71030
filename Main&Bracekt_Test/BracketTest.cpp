#include "pch.h"
#include "CppUnitTest.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <functional>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" {
#include "../Project_II_CSCN71030/bracket.h"
#include "../Project_II_CSCN71030/Team.h"
}

namespace BracketTest
{
    static std::string captureStdout(const std::function<void()>& fn)
    {
        const char* tmpPath = "bracket_stdout_capture.tmp";
        FILE* redirected = nullptr;

        freopen_s(&redirected, tmpPath, "w", stdout);
        fn();
        fflush(stdout);
        fclose(stdout);

        FILE* restored = nullptr;
        freopen_s(&restored, "CONOUT$", "w", stdout);

        std::ifstream in(tmpPath);
        std::ostringstream buffer;
        buffer << in.rdbuf();
        in.close();
        remove(tmpPath);

        return buffer.str();
    }

    static void buildTeams(Team teams[], int count)
    {
        for (int i = 0; i < count; i++) {
            char name[32];
            printf_s(name, sizeof(name), "Team%02d", i + 1);
            initialize_team(&teams[i], name, 'A');
        }
    }

    static Bracket initializedBracket(int teamCount)
    {
        Team teams[MAX_TEAMS];
        Bracket bracket{};

        buildTeams(teams, teamCount);
        initialize_bracket(teams, teamCount, &bracket);
        return bracket;
    }

    static bool isRoundOf32Winner(const Bracket* bracket, Team* team)
    {
        const Round* r32 = &bracket->rounds[ROUND_OF_32];
        for (int i = 0; i < r32->matchCount; i++) {
            if (r32->winners[i] == team) {
                return true;
            }
        }
        return false;
    }

    TEST_CLASS(BracketTest)
    {
    public:

        TEST_CLASS_INITIALIZE(SeedRandom)
        {
            srand(42);
        }

        TEST_METHOD(UpdatesNextRoundPairings)
        {
            Bracket bracket = initializedBracket(8);
            play_knockout_round(&bracket, ROUND_OF_32);

            Round* r16 = &bracket.rounds[ROUND_OF_16];

            Assert::AreEqual(4, r16->matchCount,
                L"Round of 16 should contain half as many matches as Round of 32.");

            for (int i = 0; i < r16->matchCount; i++) {
                Match* nextMatch = &r16->matches[i];
                Round* r32 = &bracket.rounds[ROUND_OF_32];

                Assert::IsNotNull(nextMatch->team1,
                    L"Even-index winners should populate team1 in the next round.");
                Assert::IsNotNull(nextMatch->team2,
                    L"Odd-index winners should populate team2 in the next round.");
                Assert::IsTrue(isRoundOf32Winner(&bracket, nextMatch->team1),
                    L"Round of 16 team1 must be a Round of 32 winner.");
                Assert::IsTrue(isRoundOf32Winner(&bracket, nextMatch->team2),
                    L"Round of 16 team2 must be a Round of 32 winner.");
                Assert::IsTrue(r32->winners[i * 2] == nextMatch->team1,
                    L"Match slot 0 winner should feed team1 of the paired next-round match.");
                Assert::IsTrue(r32->winners[i * 2 + 1] == nextMatch->team2,
                    L"Match slot 1 winner should feed team2 of the paired next-round match.");
            }
        }        

        TEST_METHOD(ShowScoresAndWinners)
        {
            Bracket bracket = initializedBracket(8);
            play_knockout_round(&bracket, ROUND_OF_32);

            std::string output = captureStdout([&]() {
                display_bracket(&bracket);
            });

            Assert::IsTrue(output.find("winner:") != std::string::npos,
                L"Completed matches should list the advancing team.");
            Assert::IsTrue(output.find(" - ") != std::string::npos,
                L"Completed matches should show numeric scores.");
        }

        
    };
}
