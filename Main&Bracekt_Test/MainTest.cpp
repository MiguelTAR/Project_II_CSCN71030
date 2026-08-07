#include "pch.h"
#include "CppUnitTest.h"
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <sstream>
#include <cstring>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

extern "C" {
#include "../Project_II_CSCN71030/Team.c"
#include "../Project_II_CSCN71030/FileIO.c"
#include "../Project_II_CSCN71030/match.c"
#include "../Project_II_CSCN71030/bracket.c"
#include "../Project_II_CSCN71030/Exit.c"
#include "../Project_II_CSCN71030/main.c"
}

namespace MainTest
{
    static std::string captureStdout(void (*fn)(void))
    {
        const char* tmpPath = "stdout_capture.tmp";
        FILE* redirected = nullptr;
        FILE* originalStdout = stdout;

        freopen_s(&redirected, tmpPath, "w", stdout);
        fn();
        fflush(stdout);
        fclose(stdout);

        // Restore stdout so subsequent tests / the runner still work.
        FILE* restored = nullptr;
        freopen_s(&restored, "CONOUT$", "w", stdout);
        (void)originalStdout;

        std::ifstream in(tmpPath);
        std::ostringstream buffer;
        buffer << in.rdbuf();
        in.close();
        remove(tmpPath);

        return buffer.str();
    }

    static void resetGlobalContext()
    {
        if (g_ctx != nullptr)
        {
            cleanup_system();
        }
        g_ctx = nullptr;
    }

    static void removeSessionLog()
    {
        remove("session.log");
    }

    TEST_CLASS(MainTest)
    {
    public:

        TEST_METHOD(TestInitializeSystem)
        {
            resetGlobalContext();
            removeSessionLog();

            // This is the TEAM DATA path, not a log path --
            // the log file itself is the hardcoded "session.log".
            bool result = initialize_system("test_teams.txt");

            Assert::IsTrue(result, L"initialize_system should return true.");
            Assert::IsNotNull(g_ctx, L"g_ctx should be allocated after successful initialization.");
            if (g_ctx != nullptr)
            {
                Assert::IsTrue(g_ctx->initialized, L"g_ctx->initialized should be true.");
                Assert::AreEqual(std::string("test_teams.csv"), std::string(g_ctx->logFileName));
                Assert::IsNotNull(g_ctx->teams, L"Teams should be loaded into g_ctx->teams.");
                Assert::IsTrue(g_ctx->teamCount > 0, L"teamCount should be greater than zero.");
                Assert::IsNotNull(g_ctx->logFile, L"g_ctx->logFile should be an open handle.");
            }

            resetGlobalContext();
        }

        
        TEST_METHOD(TestDisplayMainMenu)
        {
            std::string output = captureStdout(&displayMainMenu);

            Assert::IsTrue(output.find("MATCH PREDICTOR - MAIN MENU") != std::string::npos,
                L"Menu banner should be displayed.");
            Assert::IsTrue(output.find("1. Manage Teams") != std::string::npos,
                L"Option 1 should be displayed.");
            Assert::IsTrue(output.find("2. Predict a Match") != std::string::npos,
                L"Option 2 should be displayed.");
            Assert::IsTrue(output.find("3. Simulate a Tournament") != std::string::npos,
                L"Option 3 should be displayed.");
            Assert::IsTrue(output.find("4. Exit") != std::string::npos,
                L"Option 4 should be displayed.");
            Assert::IsTrue(output.find("Select an Option:") != std::string::npos,
                L"Selection prompt should be displayed.");
        }

    };
} 