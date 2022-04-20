# EE 538 Final Project Report

# Team: Xiaofeng Luo, Ruijie Zhu

## Phase 1

### Algorithms:  

* CalculateEditDistance  

We use dynamic programming to solve this problem, specifically, we use memerization.  

The data structure we use for a memo is a 2D vector, in which memo[i][j] represents 
the number of least operations we need when we compare the prefixes of inputs with length i and j.  

memo[i][j] can be decided by memo[i-1][j-1], memo[i][j-1], memo[i-1][j].  
If a[i-1] == b[j-1], then memo[i][j] = memo[i-1][j-1];  
else, memo[i][j] = 1+min(memo[i-1][j-1], memo[i][j-1], memo[i-1][j])  

base case:  
1. a.length or b.length equals to 0, the answer will be a.length+b.length.  
2. memo[i][j] has already been recorded, then return the recorded value.  

### Description for each function:  
1. GetLat, GetLon, GetName, GetNeighborIDs: Get the information from the data based on given id.  
Already stored in the map data -----> O(1)  

2. GetID, GetPosition: Get the information from the data based on given name.  
Need to traverse the map -----> O(n)  

3. CalculateEditDistance: Calculate edit distance between two location names.  
Assuming the length of two names are a and b, let n = max(a,b), worst case will be -----> O(n)  

4. FindClosestName: Given a location name, return the name with smallest edit distance.  
Traverse the map and call CalculateEditDistance each time, assume n words in the map, m is the max length of the two words, worst case -----> O(mn)  

5. Autocomplete: Given a parital name, return all the possible locations with partial name as the prefix.  
Traverse the map and compare the names char by char, assume n words in the map, m is the length of the input name, worset case -----> O(mn)  

### Time spent:  
1. Autocomplete: 
18ms  

2. Find the location: 
13ms   

### Conclusion:  
1. FindClosestName will be meaningless if we do not exclude the empty names in the data.  
e.g. if we type in "ca", it should return CAVA, not an empty string  

2. When we want to initialize some variables in order to store the minimum results, it is better to set it to INT_MAX/2.  
Since INT_MAX is the biggest number for int type, if we plus something to it, we will get some error.  

3. For better team work and management of our project, we should only add the changed files that our projects need and push them. Pushing everthing to the repository might be a bad idea, especially we are using different operating systems.  

4. Useful git commands:  
git log: see the commit history and the IDs
git reset --hard ID(first 6 char): reset to some old version



## Phase 2

### Algorithms:  

* CalculateShortestPath_Dijkstra  



* CalculateShortestPath_Bellman_Ford  



* DeliveringTrojan  

We use DFS to solve this problem.  

The function is actually meant to find if there exists a cycle in a directed graph, if not, return the topological sorting result.  

Data Structure:  


* CycleDetection  

We use DFS to solve this problem.  

The function is actually meant to find if there exists a cycle in an undirected graph.  

Data Structure:  


### Description for each function:  
1. CalculateShortestPath_Dijkstra  

2. CalculateShortestPath_Bellman_Ford  

3. ReadLocationsFromCSVFile  

4. ReadDependenciesFromCSVFile  

5. buildGraph  

6. traverseH  

7. DeliveringTrojan  

8. inSquare  

9. GetSubgraph  

10. traverse  

11. CycleDetection  



### Time spent:  
1. CalculateShortestPath_Dijkstra  
Ralphs->Target: 190ms  

2. CalculateShortestPath_Bellman_Ford  
Ralphs->Target: 9889ms  

3. DeliveringTrojan  
the given example: 0ms  
my_loc1.csv, my_dep1.csv: 0ms  
my_loc2.csv, my_dep2.csv: 0ms  

4. CycleDetection  
[-118.299,-118.264,34.032,34.011]: 4ms  

### Conclusion:  
