#include "gtest/gtest.h"
#include "src/lib/trojanmap.h"

TEST(TrojanMapStudentTest, P1_GetID) {
  TrojanMap m;
  EXPECT_EQ("", m.GetID("maidanglao"));
  EXPECT_EQ("6813379473", m.GetID("Ramen KenJo"));
  EXPECT_EQ("6808450114", m.GetID("Smoke Shop"));
}

TEST(TrojanMapStudentTest, P1_GetPosition) {
  TrojanMap m;
  auto position = m.GetPosition("Smoke Shop");
  std::pair<double,double> gt1(34.0029211,-118.3091031);
  EXPECT_EQ(gt1, position);
  
  position = m.GetPosition("MASTER BURGER");
  std::pair<double,double> gt2(34.0030909,-118.3091648);
  EXPECT_EQ(gt2, position);

  position = m.GetPosition("United Church of Christ Scientist");
  std::pair<double,double> gt3(34.0380675,-118.3075755);
  EXPECT_EQ(gt3, position);

  position = m.GetPosition("Chipotle");
  std::pair<double,double> gt4(34.0169985,-118.2822768);
  EXPECT_EQ(gt4, position);

  position = m.GetPosition("Jerry's home");
  std::pair<double,double> gt5(-1,-1);
  EXPECT_EQ(gt5, position);
}

TEST(TrojanMapStudentTest, P1_CalculateEditDistance) {
  TrojanMap m;
  EXPECT_EQ(m.CalculateEditDistance("love", "lovely"), 2);
  EXPECT_EQ(m.CalculateEditDistance("love", "uvol"), 4);
  EXPECT_EQ(m.CalculateEditDistance("sunday", "saturday"), 3);
  EXPECT_EQ(m.CalculateEditDistance("SunDay", "saturday"), 3); //not case sensitive
}

TEST(TrojanMapStudentTest, P1_FindClosestName) {
  TrojanMap m;
  EXPECT_EQ(m.FindClosestName("Chiptle"), "Chipotle");
  EXPECT_EQ(m.FindClosestName("Smoke Sh"), "Smoke Shop");
  EXPECT_EQ(m.FindClosestName("Pic"), "Pico");
  EXPECT_EQ(m.FindClosestName("c"), "KFC");
  EXPECT_EQ(m.FindClosestName("ca"), "CAVA"); //avoid choosing the empty strings or substituted by names with only numbers
}

TEST(TrojanMapStudentTest, P1_Autocomplete) {
  TrojanMap m;
  // Test the simple case
  auto names = m.Autocomplete("Expo/");
  std::unordered_set<std::string> gt = {"Expo/Vermont 2", "Expo/Western 2", "Expo/Crenshaw", "Expo/Western 1", "Expo/Crenshaw 1", "Expo/Vermont", "Expo/Western", "Expo/Vermont 1"}; // groundtruth for "Expo/"
  int success = 0;
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0){
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  // Test the lower case
  names = m.Autocomplete("expo/");
  success = 0;
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0){
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  // Test the lower and upper case 
  names = m.Autocomplete("eXPo/"); 
  success = 0;
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0){
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
  // Test the upper case 
  names = m.Autocomplete("EXPO/"); 
  success = 0;
  for (auto& n: names) {
    EXPECT_EQ(gt.count(n) > 0, true) << n + " is not in gt.";
    if (gt.count(n) > 0){
      success++;
    }
  }
  EXPECT_EQ(success, gt.size());
}