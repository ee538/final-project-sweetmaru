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

* CalculateShortestPath_Dijkstra:  
Our algorithm is following the instruction of discussion.  
● Get the start and end node from points’ names.  
● Create a min heap using priority queue with pair of <dist,id>.  
● Use a unordered map to save the shortest distance of each node.  
● Use a unordered map to save the predecessor of each node to build the path later;  
● Use a unordered map to store the visited nodes;  
● Initialize the map with start node is visited, the distance is 0 and  
● Put the start node into the min heap.  
● Extract the node from min heap and update the map until we find the destination node or
the heap is empty.  
● Use predecessor map to build the entire shortest path.  
 While loop:  
○ Find the shortest distance node;  
○ Check if current node is the end node or not;  
○ If not, check if current node’s distance is greater than this node’s current shortest
distance in map or not.  
○ If yes, skip; If not, check if current node is visited or not.  
○ If yes, skip; If not, traverse current node’s neighbor nodes;  
○ The new distance of each neighbor node is the current distance + the distance
between current node and the neighbor node;  
○ Check each neighbor node’s new distance is greater than the previous one or not;  
○ If yes, update the shortest distance map and predecessor map and push this pair of
<dist,id> in min heap.  



* CalculateShortestPath_Bellman_Ford  
Our algorithm is following the instruction of discussion.  
● Get the start and end node from points’ names;  
● Use a unordered map to save the shortest distance of each node.  
● Use a unordered map to save the predecessor of each node to build the path later;  
● Set a boolean stop sign as true;  
● Traverse all edges  
● The new distance of each neighbor node is the current distance + the distance between
current node and the neighbor node;  
● Check each neighbor node’s new distance is shorter than the previous one or not;  
● If yes, update this neighbor’s shortest distance and predecessor and mark stop sign as
false (Because we can go further.)  
● If the stop sign is false, we continue traverse new data map again. Else, we break;  
● Check the shortest distance to end point exists or not;  
● If it exists, build the path from the predecessor map;  



* DeliveringTrojan  
We use DFS to solve this problem.  

The function is actually meant to find if there exists a cycle in a directed graph, if not, return the possible topological sorting result.  

Data Structure:  
1. a 2D string-type vector representing the graph built based on the dependencies  
2. a bool-type vector "onPath" representing the current path we are detecting, true if current on the path, false otherwise  
3. a bool-type vector "visited" representing the node we have visited, true if visited, false otherwise  
4. a bool-type variable "hasCycle" representing the current detecting result, true if a cycle has detected, false otherwise  
5. an int-type variable "i" in traverseH function representing the index of the current node in the graph  

Helper functions:  
1. "buildGraph": build the graph we need based on the parameters locations and dependencies  
2. "traverseH": traverse the graph  
When traversing, mark a node as visited and onPath on the preorder position, traverse its children, then mark it as not on the current path and push it to the postorder vector on the postorder position  

If we detect that we return to a node which is already on the current path at the beginning, that means there exists a cycle  

DeliveringTrojan function:  
1. Use a for loop to make every node in the "locations" as a start point, and do the traversing.  
2. If there exists no cycle, the final result of topological sort is just the reverse order of the postorder vector in the traverseH function.  



* CycleDetection  
We use DFS to solve this problem.  

The function is actually meant to find if there exists a cycle in an undirected graph.  

Data Structure:  
1. a string-type vector representing the subgraph inside the square  
2. a bool-type vector "onPath" representing the current path we are detecting, true if current on the path, false otherwise  
3. a bool-type vector "visited" representing the node we have visited, true if visited, false otherwise  
4. a bool-type variable "hasCycle" representing the current detecting result, true if a cycle has detected, false otherwise  
5. a string-type variable "prev" representing the parent id of the current node  
6. an int-type variable "i" in traverseH function representing the index of the current node in the subgraph  

Helper functions:  
1. "traverse": traverse the subgraph  
The basic algorithm is pretty much the same as the one in topological sort. The only difference is that we are now traversing through an undirected graph, which in other words means, it is directed in both ways. So we need a variable "prev" to record the parent of the current node, and skip the parent when doing the DFS.  

And we also need to skip the ids whcih are outside of the square.  

### Description for each function:  
1. CalculateShortestPath_Dijkstra:  
With the min heap -----> O(N + ElogN). Because every node will be traversed and priority queue will take logN time to sort itself.  

2. CalculateShortestPath_Bellman_Ford: O(N^2).  
Because it will loop N times for each node -----> N * N = N^2.  

3. ReadLocationsFromCSVFile, ReadDependenciesFromCSVFile: Read and parse data from CSV file, and return vectors for topological sort problem.  
Traverse the CSV file -----> O(n)  

4. buildGraph: Helper function for DeliveringTrojan, to build a graph based on the dependencies  
Traverse the dependencies(size m) and locations(size n) vector -----> worst case O(m+n)  

5. traverseH: Helper function for DeliveringTrojan, to traverse the graph we build  
Traverse the graph(suppose n nodes) -----> worst case O(n)  

6. DeliveringTrojan: Find the possible topological sort order  
Try every node as a start point and traverse from it -----> worst case O(n+k)  
k is the unsure number of the dependencies of each node  

7. inSquare: Give a id retunr whether it is in square or not.  
O(1)  

8. GetSubgraph: Give four vertexes of the square area, return a list of location ids in the squares.  
Traverse the data vector -----> O(n)  

9. traverse: helper function for CycleDetection, to traverse the graph we build  
Traverse the subgraph(suppose n nodes) -----> worst case O(n)  

10. CycleDetection: Given four points of the square-shape subgraph, return true if there is a cycle path inside the square, false otherwise.  
Try every node as a start point and traverse from it -----> worst case O(n+k)  
k is the unsure number of the dependencies of each node  

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
1. We need to carefully decide whether it is a case to pass parameter by reference.  
e.g. The "prev" variable which records the parent node of the current node, should not be passed by reference, because it is used by every child of the parent. If passed by reference, only the first child will get the correct parent.  

2. Even we have done the early termination in the Bellman_Ford method, Dijkstra still works better than Bellman_Ford.  
<p align="center"><img src="img/Shortest path compare.png" alt="Shortest path compare" width="500"/></p>


## Phase 3

### Algorithms:  

* TravellingTrojan  
The function is meant to find the shortest path which visit all the places and back to the start point.  

1. Brute force  
We use the backtracking method to check all the possible permutations.  

Data Structure:  
1. a pair representing the output of shortest path and its distance  
2. a double variable "curDist" recording the current distance of the current path  
3. a string type vector "curRoute" recording the current path  

Helper functions:  
1. backtrackHelper  
Use DFS algorithm to traverse the given location ids. Push the current node into the current path vector at the preorder position and calculate the distance between current node and the parent node. Pop the node at the postorder position to try a new permutation.  

Before push the node, we need to first check if the node is a leaf. If we reach the leaf, then calculate the total distance and update.  

2. Early Backtracking  
The data structure and the algorithm are the same as the brute force method.  
The only difference is that we compare the current distance with the minimum distance before the loop. If it is larger than the minimum distance, we directly return.  

3. 2opt  
We use the given vector as the initial path. Use two for loops in the helper function to obatin a sub part and reverse the sub part to get a new path. If the distance of the new path is smaller, then update the minimum distance and path.  

The data structure is the same as the one in the previous method.  

Helper functions:  
1. twoOptHelper  
Use two for loops to try every possible ways. If a shorter path is detected, update and try to find new improvements using the updated path.  

2. twoOptSwap  
Swap the sub part and return a new possible path.  



* FindNearby
The function is meant to find k valid nearby location ids based on given center location and radius.  

Data Structure:  
1. a priority queue with type std::pair<double, std::string> and less priority ----- to sort the valid location ids for output  
2. a string type vector for storing the results  

We first traverse the data to get the valid location ids and put them into the priority queue. Then we just put the first k results into the vector from the priority queue.  

### Description for each function:  
1. TravellingTrojan_Brute_force  
O(!(n-1))  

2. backtrackHelper  
O(!(n-1))  

3. TravellingTrojan_Backtracking  
worst case -----> O(!(n-1))  

4. earlyBacktrackHelper  
worst case -----> O(!(n-1))  

5. TravellingTrojan_2opt  
best case -----> O(n^2)  
worst case -----> O(n^4)  

6. twoOptHelper  
best case -----> O(n^2)  
worst case -----> O(n^4)  

7. calculateTotalDistance  
n is the number of the locations in the path -----> O(n)  

8. twoOptSwap  
n is the number of the locations in the path -----> O(n)  

9. FindNearby  
n is the total number of data -----> O(n+k)  

### Time spent:  
1. TravellingTrojan  
Brute_force: 941ms  
Backtracking: 557ms  
2opt: 14ms  
<p align="center"><img src="img/TravellingTrojan example.png" alt="TravellingTrojan example" width="800"/></p>

2. FindNearby: 72ms  
<p align="center"><img src="img/FindNearby example.png" alt="FindNearby example" width="500"/></p>

### Conclusion:  
1. Priority queue is useful for an output requiring some certain orders.  

2. In TravellingTrojan, the method 2opt sacrifices accuracy to run faster and get a suboptimal solution. We can compare the run time and results by the following graph.  
<p align="center"><img src="img/TravellingTrojan compare.png" alt="TravellingTrojan compare" width="500"/></p>