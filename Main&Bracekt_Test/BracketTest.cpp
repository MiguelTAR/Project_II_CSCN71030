#include "pch.h"
#include "CppUnitTest.h"
#include <cstdio>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" {
#include "../Project_II_CSCN71030/bracket.h"
#include "../Project_II_CSCN71030/Team.h"
}

namespace BracketTest
{
    class StdoutCapture
    {
    public:
        StdoutCapture()
        {
            fflush(stdout);
            tmpPath = "stdout_capture.tmp";
        }

        std::string Text()
        {
            fflush(stdout);
            std::ifstream in(tmpPath);
            std::stringstream ss;
            ss << in.rdbuf();
            return ss.str();
        }

        ~StdoutCapture()
        {
            fflush(stdout);
        }

    private:
        FILE* file = nullptr;
        std::string tmpPath;
    };

    static void MakeTeam(Team* t, const char* name)
    {
        memset(t, 0, sizeof(Team));
#if defined(_MSC_VER)
        strcpy_s(t->name, sizeof(t->name), name);
#else
        strncpy(t->name, name, sizeof(t->name) - 1);
#endif
    }

    /*Builds a bracket with 'n' teams(n must be even) and leaves every round
    beyond R32 with matchCount == 0, exactly as initialize_bracket() does. */
    static void BuildBracket(Bracket* b, int n)
    {
        Team teams[MAX_TEAMS];
        for (int i = 0; i < n; i++)
        {
            char name[32];
            sprintf(name, "Team%02d", i + 1);
            MakeTeam(&teams[i], name);
        }
        initialize_bracket(teams, n, b);
    }




    TEST_CLASS(BracketTest)
    {
    public:

        TEST_METHOD(AdvancesWinnersToNextRound)
        {
            Bracket bracket;
            BuildBracket(&bracket, 32); 

            play_knockout_round(&bracket, ROUND_OF_32);

            Round* r32 = &bracket.rounds[ROUND_OF_32];
            Round* r16 = &bracket.rounds[ROUND_OF_16];

            Assert::AreEqual(16, r32->matchCount);
            Assert::AreEqual(16, r32->completedCount);

            for (int i = 0; i < r32->matchCount; i++)
            {
                Assert::IsTrue(r32->played[i] != 0, L"Every R32 match should be marked played");
                Assert::IsNotNull(r32->winners[i], L"Every R32 match must have a recorded winner");
            }
            
            Assert::AreEqual(8, r16->matchCount);

            // Winners must have been placed into the correct next-round slot
            Assert::IsTrue(r16->matches[0].team1 == r32->winners[0]);
            Assert::IsTrue(r16->matches[0].team2 == r32->winners[1]);
            Assert::IsTrue(r16->matches[1].team1 == r32->winners[2]);
            Assert::IsTrue(r16->matches[1].team2 == r32->winners[3]);
            Assert::IsTrue(ROUND_OF_16 == bracket.currentRound);
        }

              
        TEST_METHOD(ShowsScoreAndWinnerPlayedRound)
        {
            Bracket bracket;
            BuildBracket(&bracket, 8);
            play_knockout_round(&bracket, ROUND_OF_32);

            StdoutCapture capture;
            display_bracket(&bracket);
            std::string out = capture.Text();

            Round* r32 = &bracket.rounds[ROUND_OF_32];
            Assert::IsTrue(out.find("winner:") != std::string::npos);
            Assert::IsTrue(out.find(r32->winners[0]->name) != std::string::npos);
        }

        
    };
}
