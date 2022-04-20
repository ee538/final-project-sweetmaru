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




TEST(TrojanMapStudentTest, P2_ReadLocations) {
  TrojanMap m;
  std::vector<std::string> gt1 ={"Ralphs", "KFC","Chick-fil-A"};
  auto result1 = m.ReadLocationsFromCSVFile("/Users/luoxiaofeng/Desktop/EE_538/2022sp_ee538_project/final-project-sweetmaru/input/topologicalsort_locations.csv");
  EXPECT_EQ(result1, gt1);

  std::vector<std::string> gt2 ={"Target", "Chipotle","Ralphs","Leavey Library"};
  auto result2 = m.ReadLocationsFromCSVFile("/Users/luoxiaofeng/Desktop/EE_538/2022sp_ee538_project/final-project-sweetmaru/input/my_loc1.csv");
  EXPECT_EQ(result2, gt2);

  std::vector<std::string> gt3 ={"Target", "Chipotle","Ralphs","Leavey Library","Dulce"};
  auto result3 = m.ReadLocationsFromCSVFile("/Users/luoxiaofeng/Desktop/EE_538/2022sp_ee538_project/final-project-sweetmaru/input/my_loc2.csv");
  EXPECT_EQ(result3, gt3);
}

TEST(TrojanMapStudentTest, P2_ReadDependencies) {
  TrojanMap m;
  std::vector<std::vector<std::string>> gt1 ={{"Ralphs","Chick-fil-A"}, {"Ralphs","KFC"}, {"Chick-fil-A","KFC"}};
  auto result1 = m.ReadDependenciesFromCSVFile("/Users/luoxiaofeng/Desktop/EE_538/2022sp_ee538_project/final-project-sweetmaru/input/topologicalsort_dependencies.csv");
  EXPECT_EQ(result1, gt1);

  std::vector<std::vector<std::string>> gt2 ={{"Target","Chipotle"}, {"Target","Leavey Library"}, {"Chipotle","Ralphs"},{"Ralphs","Target"},{"Ralphs","Leavey Library"}};
  auto result2 = m.ReadDependenciesFromCSVFile("/Users/luoxiaofeng/Desktop/EE_538/2022sp_ee538_project/final-project-sweetmaru/input/my_dep1.csv");
  EXPECT_EQ(result2, gt2);

  std::vector<std::vector<std::string>> gt3 ={{"Target","Chipotle"}, {"Target","Leavey Library"}, {"Chipotle","Ralphs"},{"Ralphs","Leavey Library"}};
  auto result3 = m.ReadDependenciesFromCSVFile("/Users/luoxiaofeng/Desktop/EE_538/2022sp_ee538_project/final-project-sweetmaru/input/my_dep2.csv");
  EXPECT_EQ(result3, gt3);
}

TEST(TrojanMapStudentTest, P2_DeliveringTrojan) {
  TrojanMap m;

  //impossible
  std::vector<std::string> loc1 = {"Target","Chipotle","Ralphs","Leavey Library"};
  std::vector<std::vector<std::string>> dep1 = {{"Target","Chipotle"}, {"Target","Leavey Library"}, {"Chipotle","Ralphs"},{"Ralphs","Target"},{"Ralphs","Leavey Library"}};
  auto res1 = m.DeliveringTrojan(loc1, dep1);
  EXPECT_EQ(res1.size(), 0);

  //sort possible
  std::vector<std::string> loc2 = {"Target", "Chipotle","Ralphs","Leavey Library","Dulce"};
  std::vector<std::vector<std::string>> dep2 = {{"Target","Chipotle"}, {"Target","Leavey Library"}, {"Chipotle","Ralphs"},{"Ralphs","Leavey Library"}};
  auto res2 = m.DeliveringTrojan(loc2, dep2);
  std::vector<std::string> gt2 ={"Dulce","Target","Chipotle","Ralphs","Leavey Library"};
  EXPECT_EQ(res2, gt2);
}

TEST(TrojanMapStudentTest, P2_CalculateShortestPath_Dijkstra) {
  TrojanMap m;
  
  // Test from Ralphs to Target
  auto path = m.CalculateShortestPath_Dijkstra("Ralphs", "Target");
  std::vector<std::string> gt{
      "2578244375","4380040154","4380040158","4380040167","6805802087",
      "8410938469","6813416131","7645318201","6813416130","6813416129",
      "123318563","452688940","6816193777","123408705","6816193774","452688933",
      "452688931","123230412","6816193770","6787470576","4015442011","6816193692",
      "6816193693","6816193694","4015377691","544693739","6816193696","6804883323",
      "6807937309","6807937306","6816193698","4015377690","4015377689","122814447",
      "6813416159","6813405266","4015372488","4015372487","6813405229","122719216",
      "6813405232","4015372486","7071032399","4015372485","6813379479","6813379584","6814769289","5237417650"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
  
  // Reverse the input from Ralphs to Target
  path = m.CalculateShortestPath_Dijkstra("Target", "Ralphs");
  std::reverse(gt.begin(),gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

TEST(TrojanMapStudentTest, P2_CalculateShortestPath_Bellman_Ford) {
  TrojanMap m;
  
  // Test from Ralphs to Target
  auto path = m.CalculateShortestPath_Bellman_Ford("Ralphs", "Target");
  std::vector<std::string> gt{
      "2578244375","4380040154","4380040158","4380040167","6805802087",
      "8410938469","6813416131","7645318201","6813416130","6813416129",
      "123318563","452688940","6816193777","123408705","6816193774","452688933",
      "452688931","123230412","6816193770","6787470576","4015442011","6816193692",
      "6816193693","6816193694","4015377691","544693739","6816193696","6804883323",
      "6807937309","6807937306","6816193698","4015377690","4015377689","122814447",
      "6813416159","6813405266","4015372488","4015372487","6813405229","122719216",
      "6813405232","4015372486","7071032399","4015372485","6813379479","6813379584","6814769289","5237417650"}; // Expected path
  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
  
  // Reverse the input from Ralphs to Target
  path = m.CalculateShortestPath_Bellman_Ford("Target", "Ralphs");
  std::reverse(gt.begin(),gt.end()); // Reverse the path

  // Print the path lengths
  std::cout << "My path length: "  << m.CalculatePathLength(path) << "miles" << std::endl;
  std::cout << "GT path length: " << m.CalculatePathLength(gt) << "miles" << std::endl;
  EXPECT_EQ(path, gt);
}

TEST(TrojanMapStudentTest, P2_CycleDetection) {
  TrojanMap m;

  // Test case 1
  std::vector<double> square1 = {-118.298, -118.270, 34.032, 34.011};
  auto sub1 = m.GetSubgraph(square1);
  bool result1 = m.CycleDetection(sub1, square1);
  EXPECT_EQ(result1, true);

  // Test case 2
  std::vector<double> square2 = {-118.290, -118.289, 34.025, 34.020};
  auto sub2 = m.GetSubgraph(square2);
  bool result2 = m.CycleDetection(sub2, square2);
  EXPECT_EQ(result2, false);
}