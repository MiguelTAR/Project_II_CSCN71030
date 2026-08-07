#include "pch.h"
#include "CppUnitTest.h"
#include <string>
#include <iostream>
#include <cstring>

extern "C" {
#include "../Project_II_CSCN71030/Team.h"
#include "../Project_II_CSCN71030/FileIO.h"
}


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace WorldCup_Team_Tester
{
	TEST_CLASS(WorldCup_team)
	{
	public:

		TEST_METHOD(Test_initialize_team)
		{
			Team team;

			initialize_team(&team, "Canada", 'A');
			Assert::AreEqual(0, team.matches_played);
			Assert::AreEqual(0, team.wins);
			Assert::AreEqual(0, team.losses);
			Assert::AreEqual(0, team.draws);


			Assert::IsTrue(strcmp(team.name, "Canada") == 0);
		}
		TEST_METHOD(Test_count_team){
			int count = count_teams("../Project_II_CSCN71030/teams.txt");
			Assert::AreEqual(48, count);

		}
		TEST_METHOD(Test_Load_teams) {
			int team_count = 0;
			Team* teams = Load_teams("../Project_II_CSCN71030/teams.txt", &team_count);
			Assert::IsNotNull(teams);
			Assert::AreEqual(48, team_count);
			free_teams(teams);
		}

		TEST_METHOD(Test_load_teams) {
			int team_count = 0;
			Team* teams = Load_teams("../Project_II_CSCN71030/teams.txt", &team_count);
			Assert::IsNotNull(teams);
			Assert::IsTrue(strcmp(teams[0].name, "Canada") == 0);
			Assert::AreEqual('A', teams[0].group);
			free_teams(teams);
		}	

		TEST_METHOD(STATS)
		{
			int team_count = 0;
			Team* teams = Load_teams("../Project_II_CSCN71030/teams.txt", &team_count);
			Assert::AreEqual(0, teams[0].wins);
			Assert::AreEqual(0, teams[0].losses);
			Assert::AreEqual(0, teams[0].draws);

			free_teams(teams);
		}

		TEST_METHOD(INVALID)
		{
			int team_count = 0;
			Team* teams = Load_teams("../Project_II_CSCN71030/invalid.txt", &team_count);
			Assert::IsNull(teams);
		}

		TEST_METHOD(VALID_GROUPS)		{
			int team_count = 0;

			Team* teams = Load_teams("../Project_II_CSCN71030/teams.txt", &team_count);
			for (int i = 0; i < team_count; i++)
			{
				bool vaild = (teams[i].group >= 'A' && teams[i].group <= 'L');
				Assert::IsTrue(vaild);

				free_teams(teams);
			}


		}

		TEST_METHOD(FREE_TEAMS)
		{
			int team_count = 0;
			Team* teams = Load_teams("../Project_II_CSCN71030/teams.txt", &team_count);
			Assert::IsNotNull(teams);
			free_teams(teams);

			Assert::IsTrue(true);
		}
	};

}

