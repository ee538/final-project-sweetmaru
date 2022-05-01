#include "trojanmap.h"


//-----------------------------------------------------
// TODO: Student should implement the following:
//-----------------------------------------------------
/**
 * GetLat: Get the latitude of a Node given its id. If id does not exist, return -1.
 * 
 * @param  {std::string} id : location id
 * @return {double}         : latitude
 */
double TrojanMap::GetLat(const std::string& id) {
    return data[id].lat;
}

/**
 * GetLon: Get the longitude of a Node given its id. If id does not exist, return -1.
 * 
 * @param  {std::string} id : location id
 * @return {double}         : longitude
 */
double TrojanMap::GetLon(const std::string& id) { 
    return data[id].lon;
}

/**
 * GetName: Get the name of a Node given its id. If id does not exist, return "NULL".
 * 
 * @param  {std::string} id : location id
 * @return {std::string}    : name
 */
std::string TrojanMap::GetName(const std::string& id) { 
    return data[id].name;
}

/**
 * GetNeighborIDs: Get the neighbor ids of a Node. If id does not exist, return an empty vector.
 * 
 * @param  {std::string} id            : location id
 * @return {std::vector<std::string>}  : neighbor ids
 */
std::vector<std::string> TrojanMap::GetNeighborIDs(const std::string& id) {
    return data[id].neighbors;
}

/**
 * GetID: Given a location name, return the id. 
 * If the node does not exist, return an empty string. 
 *
 * @param  {std::string} name          : location name
 * @return {int}  : id
 */
std::string TrojanMap::GetID(const std::string& name) {
  std::string res = "";
  for (auto cur : data){
    if (cur.second.name == name){
      return cur.second.id;
    }
  }
  return res;
}

/**
 * GetPosition: Given a location name, return the position. If id does not exist, return (-1, -1).
 *
 * @param  {std::string} name          : location name
 * @return {std::pair<double,double>}  : (lat, lon)
 */
std::pair<double, double> TrojanMap::GetPosition(std::string name) {
  std::pair<double, double> results(-1, -1);
  std::string id = GetID(name);
  if (id.length() > 0){
    double lat = data[id].lat;
    double lon = data[id].lon;
    results.first = lat;
    results.second = lon;
  }
  return results;
}


/**
 * CalculateEditDistance: Calculate edit distance between two location names
 * 
 */
// Use a helper to realize a dynamic programing solution
int helper(std::string &a, std::string &b, int l1, int l2, std::vector<std::vector<int>> &memo);

int TrojanMap::CalculateEditDistance(std::string a, std::string b){
    int len1 = a.length();
    int len2 = b.length();
    for (int i = 0; i < a.length(); i++){
      a[i] = tolower(a[i]);
    }
    for (int i = 0; i < b.length(); i++){
      b[i] = tolower(b[i]);
    }
    std::vector<std::vector<int>> memo(len1 + 1, std::vector<int>(len2 + 1, -1));
    int ans = helper(a, b, len1, len2, memo);
    return ans;
}

int helper(std::string &a, std::string &b, int l1, int l2, std::vector<std::vector<int>> &memo){
  if (l1 == 0){
    return l2;
  }
  if (l2 == 0){
    return l1;
  }
  if (memo[l1][l2] >= 0){
    return memo[l1][l2];
  }
  int ans = 0;
  if (a[l1 - 1] == b[l2 - 1]){
    ans = helper(a, b, l1 - 1, l2 -1, memo);
  } else {
    ans = 1 + std::min(helper(a, b, l1 - 1, l2, memo), std::min(helper(a, b, l1 - 1, l2 -1, memo), helper(a, b, l1, l2 -1, memo)));
  }
  memo[l1][l2] = ans;
  return ans;
}

/**
 * FindClosestName: Given a location name, return the name with smallest edit distance.
 *
 * @param  {std::string} name          : location name
 * @return {std::string} tmp           : similar name
 */
std::string TrojanMap::FindClosestName(std::string name) {
  std::string tmp = "";
  int minValue = INT_MAX/2;
  for (auto cur : data){
    std::string temp = cur.second.name;
    if (temp.length() == 0){
      continue;
    }
    int dist = CalculateEditDistance(temp, name);
    if (dist < minValue){
      minValue = dist;
      tmp = temp;
    }
    if (dist == minValue && temp.length() > tmp.length()){
      tmp = temp;
    }
  }
  return tmp;
}


/**
 * Autocomplete: Given a parital name return all the possible locations with
 * partial name as the prefix. The function should be case-insensitive.
 *
 * @param  {std::string} name          : partial name
 * @return {std::vector<std::string>}  : a vector of full names
 */
std::vector<std::string> TrojanMap::Autocomplete(std::string name){
  std::vector<std::string> results;
  for (int i = 0; i < name.length(); i++){
    name[i] = tolower(name[i]);
  }
  int size = name.length();
  for (auto cur : data){
    std::string temp = cur.second.name;
    std::string lowercaseTemp = temp;
    if (temp.length() < size){
      continue;
    }
    for (int i = 0; i < temp.length(); i++){
      lowercaseTemp[i] = tolower(temp[i]);
    }
    int ans = name.compare(0, name.length(), lowercaseTemp, 0, name.length());
    if (ans == 0){
      results.push_back(temp);
    }
  }
  return results;
}

/**
 * CalculateDistance: Get the distance between 2 nodes. 
 * 
 * @param  {std::string} a  : a_id
 * @param  {std::string} b  : b_id
 * @return {double}  : distance in mile
 */
double TrojanMap::CalculateDistance(const std::string &a_id, const std::string &b_id) {
  // Do not change this function
  Node a = data[a_id];
  Node b = data[b_id];
  double dlon = (b.lon - a.lon) * M_PI / 180.0;
  double dlat = (b.lat - a.lat) * M_PI / 180.0;
  double p = pow(sin(dlat / 2),2.0) + cos(a.lat * M_PI / 180.0) * cos(b.lat * M_PI / 180.0) * pow(sin(dlon / 2),2.0);
  double c = 2 * asin(std::min(1.0,sqrt(p)));
  return c * 3961;
}

/**
 * CalculatePathLength: Calculates the total path length for the locations inside the vector.
 * 
 * @param  {std::vector<std::string>} path : path
 * @return {double}                        : path length
 */
double TrojanMap::CalculatePathLength(const std::vector<std::string> &path) {
  // Do not change this function
  double sum = 0;
  for (int i = 0; i < int(path.size())-1; i++) {
    sum += CalculateDistance(path[i], path[i+1]);
  }
  return sum;
}

/**
 * CalculateShortestPath_Dijkstra: Given 2 locations, return the shortest path which is a
 * list of id. Hint: Use priority queue.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */



std::vector<std::string> TrojanMap::CalculateShortestPath_Dijkstra(
    std::string location1_name, std::string location2_name) {

  std::vector<std::string> path;
  // if name uncorrect.
  std::string sourceID = GetID(location1_name);
  std::string destID = GetID(location2_name);
  if (sourceID.size() == 0 || destID.size() == 0){
    return path;
  }
  // if source = destination
  if (sourceID == destID){
    return path;
  }
  // if name correct. Initialization.
  std::unordered_set<std::string> visited;
  std::unordered_map<std::string, double> d;
  std::unordered_map<std::string, std::string> p;
  std::priority_queue<std::pair<double, std::string>,
                      std::vector<std::pair<double, std::string>>, cmp> pq;  

  for (auto cur : data){
    d[cur.first] = INT_MAX/2.0;
    p[cur.first] = cur.first;
  }

  // Start from source
  d[sourceID] = 0;
  pq.push(std::make_pair(d[sourceID], sourceID));

  while (!pq.empty()){
    std::pair<double, std::string> cur = pq.top();
    pq.pop();
    std::string id = cur.second;
    double dist = cur.first;
    if (visited.count(id) > 0){
      continue;
    }
    visited.insert(id);
    if (data[id].neighbors.empty()){
      continue;
    }
    // check if current node's distance is greater than ...
    if (dist > d[id]){
      continue;
    }
    // for each neighbour
    std::vector<std::string> neis = data[id].neighbors;
    for (std::string nei : neis){
      double weight = CalculateDistance(id, nei);
      if (d[nei] <= d[id] + weight){
        continue;
      }
      d[nei] = d[id] + weight;
      p[nei] = id;
      pq.push(std::make_pair(d[nei], nei));
    }
  }
  
  if (p[destID] == destID){
    return path;
  }
  std::string curID = destID;
  while (curID != sourceID){
    path.push_back(curID);
    curID = p[curID];
  }
  path.push_back(sourceID);
  std::reverse(path.begin(), path.end());
  
  return path;
}

/**
 * CalculateShortestPath_Bellman_Ford: Given 2 locations, return the shortest path which is a
 * list of id. Hint: Do the early termination when there is no change on distance.
 *
 * @param  {std::string} location1_name     : start
 * @param  {std::string} location2_name     : goal
 * @return {std::vector<std::string>}       : path
 */
std::vector<std::string> TrojanMap::CalculateShortestPath_Bellman_Ford(
    std::string location1_name, std::string location2_name){
  std::vector<std::string> path;
  // if name uncorrect.
  std::string sourceID = GetID(location1_name);
  std::string destID = GetID(location2_name);
  if (sourceID.size() == 0 || destID.size() == 0){
    return path;
  }
  // if name is correct, initialization.
  std::unordered_map<std::string, double> d;
  std::unordered_map<std::string, std::string> p;
  bool stop = true;

  for (auto cur : data){
    d[cur.first] = INT_MAX/2.0;
    p[cur.first] = cur.first;
  }
  // Initialzie source
  d[sourceID] = 0;

  for (int i = 0; i < data.size() - 1; i++){
    stop = true;
    // for each edge
    for (auto cur : data){
      std::string id = cur.first;
      std::vector<std::string> neis = data[id].neighbors;
      for (std::string nei : neis){
        double dist = CalculateDistance(id, nei);
        if (d[id] + dist < d[nei]){
          d[nei] = d[id] + dist;
          p[nei] = id;
          stop = false;
        }
      }
    }
    if (stop == true){
      break;
    }
  }
  
  if (p[destID] == destID){
    return path;
  }
  std::string curID = destID;
  while (curID != sourceID){
    path.push_back(curID);
    curID = p[curID];
  }
  path.push_back(sourceID);
  std::reverse(path.begin(), path.end());

  return path;
}

/**
 * Travelling salesman problem: Given a list of locations, return the shortest
 * path which visit all the places and back to the start point.
 *
 * @param  {std::vector<std::string>} input : a list of locations needs to visit
 * @return {std::pair<double, std::vector<std::vector<std::string>>} : a pair of total distance and the all the progress to get final path
 */
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_Brute_force(
                                    std::vector<std::string> location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> records;
  records.first = INT_MAX / 2;
  double curDist = 0;
  std::vector<std::string> curRoute;
  backtrackHelper(curDist, curRoute, records, location_ids);

  return records;
}

void TrojanMap::backtrackHelper(double curDist, std::vector<std::string> &curRoute, 
                     std::pair<double, std::vector<std::vector<std::string>>> &records,
                     std::vector<std::string> &location_ids){
  // if it's a leaf, update
  if (curRoute.size() == location_ids.size()){
    curDist = curDist + CalculateDistance(curRoute[0], curRoute[location_ids.size() - 1]);
    curRoute.push_back(curRoute[0]);
    if (curDist < records.first){
      records.first = curDist;
      records.second.push_back(curRoute);
    }
    curRoute.pop_back(); //pop the duplicate destination
    return;
  }
  // general case
  for (int i = 0; i < location_ids.size(); i++){
    if (find(curRoute.begin(), curRoute.end(), location_ids[i]) != curRoute.end()){
      continue;
    }
    curRoute.push_back(location_ids[i]);
    std::string formerLocation = "";
    // first location
    if (curRoute.size() == 1){
      formerLocation = location_ids[i];
    } else {
      formerLocation = curRoute[curRoute.size() - 2];
    }
    double tempDist = CalculateDistance(formerLocation, location_ids[i]);
    backtrackHelper(curDist + tempDist, curRoute, records, location_ids);
    curRoute.pop_back();
  }
}

std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_Backtracking(
                                    std::vector<std::string> location_ids) {
  std::pair<double, std::vector<std::vector<std::string>>> records;
  records.first = INT_MAX / 2;
  double curDist = 0;
  std::vector<std::string> curRoute;
  earlyBacktrackHelper(curDist, curRoute, records, location_ids);
  return records;
}

void TrojanMap::earlyBacktrackHelper(double curDist, std::vector<std::string> &curRoute, 
                        std::pair<double, std::vector<std::vector<std::string>>> &records,
                        std::vector<std::string> &location_ids){
  // if it's a leaf
  if (curRoute.size() == location_ids.size()){
    curDist = curDist + CalculateDistance(curRoute[0], curRoute[location_ids.size() - 1]);
    curRoute.push_back(curRoute[0]);
    if (curDist < records.first){
      records.first = curDist;
      records.second.push_back(curRoute);
    }
    curRoute.pop_back();
    return;
  }
  // general case
  for (int i = 0; i < location_ids.size(); i++){
    if (find(curRoute.begin(), curRoute.end(), location_ids[i]) != curRoute.end()){
      continue;
    }
    curRoute.push_back(location_ids[i]);
    std::string formerLocation = "";
    // first location
    if (curRoute.size() == 1){
      formerLocation = location_ids[i];
    } else {
      formerLocation = curRoute[curRoute.size() - 2];
    }
    double tempDist = CalculateDistance(formerLocation, location_ids[i]);
    // early backtrack
    if (curDist + tempDist > records.first){
      curRoute.pop_back();
      continue;
    }
    earlyBacktrackHelper(curDist + tempDist, curRoute, records, location_ids);
    curRoute.pop_back();
  }
}

std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_2opt(
      std::vector<std::string> location_ids){
  std::pair<double, std::vector<std::vector<std::string>>> records;
  std::vector<std::string> curRoute = location_ids;
  double curDist = calculateTotalDistance(curRoute);
  records.first = curDist;
  curRoute.push_back(curRoute[0]);
  records.second.push_back(curRoute);
  curRoute.pop_back();
  twoOptHelper(curDist, curRoute, records, location_ids);

  return records;
}

void TrojanMap::twoOptHelper(double curDist, std::vector<std::string> &curRoute, 
                    std::pair<double, std::vector<std::vector<std::string>>> &records,
                    std::vector<std::string> &location_ids){
  
  for (int i = 1; i < curRoute.size(); i++){
    for (int j = i + 1; j < curRoute.size(); j++){
      std::vector<std::string> tempRoute = twoOptSwap(curRoute, i, j);
      double tempDistance = calculateTotalDistance(tempRoute);
      if (tempDistance < records.first){
        records.first = tempDistance;
        tempRoute.push_back(tempRoute[0]);
        records.second.push_back(tempRoute);
        tempRoute.pop_back();
        twoOptHelper(tempDistance, tempRoute, records, location_ids);
      }
    }
  }                    
}

// calculate the distance of a path
double TrojanMap::calculateTotalDistance(std::vector<std::string> &curRoute){
  curRoute.push_back(curRoute[0]); //add the start point as the destination
  double res = CalculatePathLength(curRoute);
  curRoute.pop_back();
  return res;
}

// 2 opt swap
std::vector<std::string> TrojanMap::twoOptSwap(std::vector<std::string> &curRoute, int l, int r){
  std::vector<std::string> newRoute;
  for (int i = 0; i < l; i++){
    newRoute.push_back(curRoute[i]);
  }
  for (int i = r; i >= l; i--){
    newRoute.push_back(curRoute[i]);
  }
  for (int i = r + 1; i < curRoute.size(); i++){
    newRoute.push_back(curRoute[i]);
  }
  return newRoute;
}

// 3 opt
std::pair<double, std::vector<std::vector<std::string>>> TrojanMap::TravellingTrojan_3opt(
      std::vector<std::string> location_ids){
  std::pair<double, std::vector<std::vector<std::string>>> records;
  std::vector<std::string> curRoute = location_ids;
  double curDist = calculateTotalDistance(curRoute);
  records.first = curDist;
  curRoute.push_back(curRoute[0]);
  records.second.push_back(curRoute);
  curRoute.pop_back();
  threeOptHelper(curDist, curRoute, records, location_ids);

  return records;
}

void TrojanMap::threeOptHelper(double curDist, std::vector<std::string> &curRoute, 
                    std::pair<double, std::vector<std::vector<std::string>>> &records,
                    std::vector<std::string> &location_ids){
  
  for (int i = 1; i < curRoute.size(); i++){
    for (int j = i + 1; j < curRoute.size(); j++){
      for (int k = j + 1; k < curRoute.size(); k++){
        std::vector<std::string> tempRoute = threeOptSwap(curRoute, i, j, k);
        double tempDistance = calculateTotalDistance(tempRoute);
        if (tempDistance < records.first){
          records.first = tempDistance;
          tempRoute.push_back(tempRoute[0]);
          records.second.push_back(tempRoute);
          tempRoute.pop_back();
          threeOptHelper(tempDistance, tempRoute, records, location_ids);
        }
      }
    }
  }                    
}

std::vector<std::string> TrojanMap::threeOptSwap(std::vector<std::string> &curRoute, int l1, int l2, int l3){
  std::vector<std::string> bestRoute;
  double min = INT_MAX / 2;
  double tempDist;
  std::vector<std::string> newRoute;
  // // case 1
  // newRoute.clear();
  // for (int i = 0; i < l1; i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l2 - 1; i >= l1; i--){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l2; i < curRoute.size(); i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // tempDist = calculateTotalDistance(newRoute);
  // if (tempDist < min){
  //   min = tempDist;
  //   bestRoute = newRoute;
  // }
  // // case 2
  // newRoute.clear();
  // for (int i = 0; i < l1; i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l3 - 1; i >= l1; i--){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l3; i < curRoute.size(); i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // tempDist = calculateTotalDistance(newRoute);
  // if (tempDist < min){
  //   min = tempDist;
  //   bestRoute = newRoute;
  // }
  // // case 3
  // newRoute.clear();
  // for (int i = 0; i < l2; i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l3 - 1; i >= l1; i--){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l3; i < curRoute.size(); i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // tempDist = calculateTotalDistance(newRoute);
  // if (tempDist < min){
  //   min = tempDist;
  //   bestRoute = newRoute;
  // }
  // // case 4
  // newRoute.clear();
  // for (int i = 0; i < l1; i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l2; i < l3; i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l2 - 1; i >= l1; i--){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l3; i < curRoute.size(); i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // tempDist = calculateTotalDistance(newRoute);
  // if (tempDist < min){
  //   min = tempDist;
  //   bestRoute = newRoute;
  // }
  // // case 5
  // newRoute.clear();
  // for (int i = 0; i < l1; i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l2 - 1; i >= l1; i--){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l3 - 1; i >= l2; i--){
  //   newRoute.push_back(curRoute[i]);
  // }
  // for (int i = l3; i < curRoute.size(); i++){
  //   newRoute.push_back(curRoute[i]);
  // }
  // tempDist = calculateTotalDistance(newRoute);
  // if (tempDist < min){
  //   min = tempDist;
  //   bestRoute = newRoute;
  // }
  // case 6
  newRoute.clear();
  for (int i = 0; i < l1; i++){
    newRoute.push_back(curRoute[i]);
  }
  for (int i = l3 - 1; i >= l2; i--){
    newRoute.push_back(curRoute[i]);
  }
  for (int i = l1; i < l2; i++){
    newRoute.push_back(curRoute[i]);
  }
  for (int i = l3; i < curRoute.size(); i++){
    newRoute.push_back(curRoute[i]);
  }
  tempDist = calculateTotalDistance(newRoute);
  if (tempDist < min){
    min = tempDist;
    bestRoute = newRoute;
  }
  // case 7
  newRoute.clear();
  for (int i = 0; i < l1; i++){
    newRoute.push_back(curRoute[i]);
  }
  for (int i = l2; i < l3; i++){
    newRoute.push_back(curRoute[i]);
  }
  for (int i = l1; i < l2; i++){
    newRoute.push_back(curRoute[i]);
  }
  for (int i = l3; i < curRoute.size(); i++){
    newRoute.push_back(curRoute[i]);
  }
  tempDist = calculateTotalDistance(newRoute);
  if (tempDist < min){
    min = tempDist;
    bestRoute = newRoute;
  }
  // return result
  return newRoute;
}

// std::vector<std::string> TrojanMap::threeOptSwap2(std::vector<std::string> &curRoute, int l1, int l2, int l3){
//   std::vector<std::string> newRoute;
//   for (int i = 0; i < l1; i++){
//     newRoute.push_back(curRoute[i]);
//   }
//   for (int i = l3 - 1; i >= l2; i--){
//     newRoute.push_back(curRoute[i]);
//   }
//   for (int i = l1; i < l2; i++){
//     newRoute.push_back(curRoute[i]);
//   }
//   for (int i = l3; i < curRoute.size(); i++){
//     newRoute.push_back(curRoute[i]);
//   }
//   return newRoute;
// }

/**
 * Given CSV filename, it read and parse locations data from CSV file,
 * and return locations vector for topological sort problem.
 *
 * @param  {std::string} locations_filename     : locations_filename
 * @return {std::vector<std::string>}           : locations 
 */
std::vector<std::string> TrojanMap::ReadLocationsFromCSVFile(std::string locations_filename){
  std::vector<std::string> location_names_from_csv;

  std::fstream fin;
  fin.open(locations_filename, std::ios::in);
  std::string line;

  // remove first line
  getline(fin, line);

  while (getline(fin, line)) {
    location_names_from_csv.push_back(line);
  }
  fin.close();

  return location_names_from_csv;
}

/**
 * Given CSV filenames, it read and parse dependencise data from CSV file,
 * and return dependencies vector for topological sort problem.
 *
 * @param  {std::string} dependencies_filename     : dependencies_filename
 * @return {std::vector<std::vector<std::string>>} : dependencies
 */
std::vector<std::vector<std::string>> TrojanMap::ReadDependenciesFromCSVFile(std::string dependencies_filename){
  std::vector<std::vector<std::string>> dependencies_from_csv;

  std::fstream fin;
  fin.open(dependencies_filename, std::ios::in);
  std::string line, word;

  // remove first line
  getline(fin, line);

  while (getline(fin, line)) {
    std::stringstream s(line);
    std::vector<std::string> dependencies;
    while (getline(s, word, ',')) {
      dependencies.push_back(word);
    }
    dependencies_from_csv.push_back(dependencies);
  }
  fin.close();

  return dependencies_from_csv;
}

//helper function for DeliveringTrojan, to build a graph based on the dependencies
//the graph is a 2D vector, we can access the dependency of a location using the same index as the one in the vector locations
std::vector<std::vector<std::string>> buildGraph(std::vector<std::string> &locations,
                                                     std::vector<std::vector<std::string>> &dependencies);
//helper function for DeliveringTrojan, to traverse the graph we build
void traverseH(std::vector<std::string> &locations, std::vector<std::vector<std::string>> &graph, int i, std::vector<bool> &onPath, std::vector<bool> &visited, bool &hasCycle, std::vector<std::string> &postOrder);
/**
 * DeliveringTrojan: Given a vector of location names, it should return a sorting of nodes
 * that satisfies the given dependencies. If there is no way to do it, return a empty vector.
 *
 * @param  {std::vector<std::string>} locations                     : locations
 * @param  {std::vector<std::vector<std::string>>} dependencies     : prerequisites
 * @return {std::vector<std::string>} results                       : results
 */
std::vector<std::string> TrojanMap::DeliveringTrojan(std::vector<std::string> &locations,
                                                     std::vector<std::vector<std::string>> &dependencies){
  std::vector<std::string> result;
  std::vector<std::vector<std::string>> graph = buildGraph(locations, dependencies);
  std::vector<bool> onPath(locations.size(), false);
  std::vector<bool> visited(locations.size(), false);
  bool hasCycle = false;

  for(int i = 0; i < locations.size(); i++) { //try to start from every location to see if possible
    traverseH(locations, graph, i, onPath, visited, hasCycle, result);
  }

  if(hasCycle) { //it is impossible to sort it as required if there is a cycle
    std::vector<std::string> impossible;
    return impossible;
  }

  std::reverse(result.begin(), result.end());

  return result;                                                     
}

std::vector<std::vector<std::string>> buildGraph(std::vector<std::string> &locations,
                                                     std::vector<std::vector<std::string>> &dependencies){
  std::vector<std::vector<std::string>> graph(locations.size());
  for(auto arr: dependencies) {
    auto it = find(locations.begin(),locations.end(),arr[0]);
    graph[it-locations.begin()].push_back(arr[1]); //use the same index to find locations' dependencies in the graph
  } 
  return graph;                                                    
}
void traverseH(std::vector<std::string> &locations, std::vector<std::vector<std::string>> &graph, int i, std::vector<bool> &onPath, std::vector<bool> &visited, bool &hasCycle, std::vector<std::string> &postOrder){
  if(onPath[i]) hasCycle = true; //if traverse to some location which is still on the current path

  if(visited[i] || hasCycle) return; // visited or cycle detected

  visited[i] = true;
  onPath[i] = true;
  for(auto dependency: graph[i]) {
    auto it = find(locations.begin(),locations.end(),dependency);
    int idx = it-locations.begin();
    traverseH(locations,graph,idx,onPath,visited,hasCycle,postOrder);
  }
  postOrder.push_back(locations[i]); //push the location we visited to the vector in the reverse order of the topological order
  onPath[i] = false;
}

/**
 * inSquare: Give a id retunr whether it is in square or not.
 *
 * @param  {std::string} id            : location id
 * @param  {std::vector<double>} square: four vertexes of the square area
 * @return {bool}                      : in square or not
 */
bool TrojanMap::inSquare(std::string id, std::vector<double> &square) {
  double lat = GetLat(id);
  double lon = GetLon(id);
  if(lon > square[0] && lon < square[1] && lat > square[3] && lat < square[2]) return true;
  return false;
}

/**
 * GetSubgraph: Give four vertexes of the square area, return a list of location ids in the squares
 *
 * @param  {std::vector<double>} square         : four vertexes of the square area
 * @return {std::vector<std::string>} subgraph  : list of location ids in the square
 */
std::vector<std::string> TrojanMap::GetSubgraph(std::vector<double> &square) {
  // include all the nodes in subgraph
  std::vector<std::string> subgraph;

  for(auto loc: data){
    if(inSquare(loc.first, square)) {
      subgraph.push_back(loc.first);
    }
  }

  return subgraph;
}

/**
 * Cycle Detection: Given four points of the square-shape subgraph, return true if there
 * is a cycle path inside the square, false otherwise.
 * 
 * @param {std::vector<std::string>} subgraph: list of location ids in the square
 * @param {std::vector<double>} square: four vertexes of the square area
 * @return {bool}: whether there is a cycle or not
 */
bool TrojanMap::CycleDetection(std::vector<std::string> &subgraph, std::vector<double> &square) {
  std::vector<bool> onPath(subgraph.size(), false);
  bool hasCycle = false;
  for(int i = 0; i < subgraph.size(); i++) {
    std::string prev = "";
    std::vector<bool> visited(subgraph.size(), false);
    traverse(subgraph, square, prev, i, onPath, visited, hasCycle);
    if(hasCycle) break;
  }
  
  return hasCycle;
}
void TrojanMap::traverse(std::vector<std::string> &subgraph, std::vector<double> &square, std::string prev, int i, std::vector<bool> &onPath, 
                          std::vector<bool> &visited, bool &hasCycle) {
  if(onPath[i]) hasCycle = true; //if traverse to some location which is still on the current path

  if(visited[i] || hasCycle) return; // visited or cycle detected

  visited[i] = true;
  onPath[i] = true;
  for(auto neighbor: GetNeighborIDs(subgraph[i])) {
    auto it = find(subgraph.begin(), subgraph.end(), neighbor);
    if(it == subgraph.end() || neighbor == prev) continue; //exclude the locations outside the square and the previous location
    else {
      int idx = it - subgraph.begin();
      traverse(subgraph, square, subgraph[i], idx, onPath, visited, hasCycle);
    }
  }
  onPath[i] = false;
}


/**
 * FindNearby: Given a class name C, a location name L and a number r, 
 * find all locations in class C on the map near L with the range of r and return a vector of string ids
 * 
 * @param {std::string} className: the name of the class
 * @param {std::string} locationName: the name of the location
 * @param {int} r: search radius
 * @param {int} k: search numbers
 * @return {std::vector<std::string>}: location name that meets the requirements
 */
std::vector<std::string> TrojanMap::FindNearby(std::string attributesName, std::string name, double r, int k) {
  std::vector<std::string> res;
  std::priority_queue<std::pair<double, std::string>,
                      std::vector<std::pair<double, std::string>>, cmp> pq;
  std::string targetID = GetID(name);
  for (auto cur : data){
    if (cur.second.attributes.count(attributesName) > 0){
      double dist = CalculateDistance(targetID, cur.second.id);
      if (dist <= r && dist != 0){
        pq.push(std::make_pair(dist, cur.second.id));
      }
    }
  }

  while (k > 0 && !pq.empty()){
    std::pair<double, std::string> temp = pq.top();
    pq.pop();
    res.push_back(temp.second);
    k--;
  }

  return res;
}

/**
 * CreateGraphFromCSVFile: Read the map data from the csv file
 * 
 */
void TrojanMap::CreateGraphFromCSVFile() {
  // Do not change this function
  std::fstream fin;
  fin.open("src/lib/data.csv", std::ios::in);
  std::string line, word;

  // remove first line
  getline(fin, line);
  while (getline(fin, line)) {
    std::stringstream s(line);

    Node n;
    int count = 0;
    while (getline(s, word, ',')) {
      word.erase(std::remove(word.begin(), word.end(), '\''), word.end());
      word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '{'), word.end());
      word.erase(std::remove(word.begin(), word.end(), '}'), word.end());
      if (count == 0)
        n.id = word;
      else if (count == 1)
        n.lat = stod(word);
      else if (count == 2)
        n.lon = stod(word);
      else if (count == 3)
        n.name = word;
      else {
        word.erase(std::remove(word.begin(), word.end(), ' '), word.end());
        if (isalpha(word[0]))
          n.attributes.insert(word);
        if (isdigit(word[0]))
          n.neighbors.push_back(word);
      }
      count++;
    }
    data[n.id] = n;
  }
  fin.close();
}
