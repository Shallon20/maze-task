// NAME: SHALLON MARIA OMWONO
// STUDENT ID: 230ADB062

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <climits> // Include the <climits> header for INT_MAX

using namespace std;

// Structure of a cell in the maze
struct Cell {
    int x, y; // Coordinates
    int coins;
};

// Function to check if a cell is valid
bool isValid(int x, int y, size_t rows, size_t cols, vector<vector<char>>& maze) {
    return (x >= 0 && static_cast<size_t>(x) < rows && y >= 0 && static_cast<size_t>(y) < cols && maze[static_cast<size_t>(x)][static_cast<size_t>(y)] != 'X');
}

// Shortest path function
int findShortestPath(vector<vector<char>>& maze, int startX, int startY, int goalX, int goalY) {
    size_t rows = maze.size();
    size_t cols = maze[0].size();

    // Possible moves
    int dx[] = {-1, -1, -1, 0, 0, 1, 1, 1};
    int dy[] = {-1, 0, 1, -1, 1, -1, 0, 1};

    // Priority queue
    priority_queue<pair<int, pair<int, int>>, vector<pair<int, pair<int, int>>>, greater<pair<int, pair<int, int>>>> priorityq;

    // 2D vector to store the minimum coins to reach each cell
    vector<vector<int>> minCoins(rows, vector<int>(cols, INT_MAX));

    // Starting cell pushed into the priority queue and 0 coins are required
    priorityq.push({0, {startX, startY}});
    minCoins[static_cast<size_t>(startX)][static_cast<size_t>(startY)] = 0;

    //Dijkstra's algorithm
    while (!priorityq.empty()) {
        // Cell with minimum coins from the priority queue
        int coins = priorityq.top().first;
        int x = priorityq.top().second.first;
        int y = priorityq.top().second.second;
        priorityq.pop();

        // Check if goal is reached
        if (x == goalX && y == goalY) {
            return coins; 
        }

        // Possible moves from the current cell
        for (int i = 0; i < 8; i++) {
            int newX = x + dx[i];
            int newY = y + dy[i];

            // Check if new cell is valid
            if (isValid(newX, newY, rows, cols, maze)) {
                // Total coins required to reach new cell calculation
                int newCoins = coins + (maze[static_cast<size_t>(newX)][static_cast<size_t>(newY)] - '0');

                // Minimum coins updated if current path is better
                if (newCoins < minCoins[static_cast<size_t>(newX)][static_cast<size_t>(newY)]) {
                    minCoins[static_cast<size_t>(newX)][static_cast<size_t>(newY)] = newCoins;
                    priorityq.push({newCoins, {newX, newY}});
                }
            }
        }
    }

    // If goal not reachable
    return -1;
}

int main() {
    // Input file
    ifstream inputFile("maze_11x11.txt");

    //Int to size_t conversion
    size_t rows, cols; 
    inputFile >> rows >> cols;

    // Input maze layout
    vector<vector<char>> maze(rows, vector<char>(cols));
    for (size_t i = 0; i < rows; i++) {
        for (size_t j = 0; j < cols; j++) {
            inputFile >> maze[i][j];
        }
    }

    // Starting point
    int startX, startY;
    inputFile >> startX >> startY;

    // Goal point
    int goalX, goalY;
    inputFile >> goalX >> goalY;

    // Close input file
    inputFile.close();

    // Shortest path and coins collection
    int coinsCollected = findShortestPath(maze, startX, startY, goalX, goalY);

    // Coins collected
    cout << "Coins collected on the shortest path: " << coinsCollected << endl;

    // Open output file to save the result
    ofstream outputFile("output.txt");
    outputFile << coinsCollected << endl;

    // Close output file
    outputFile.close();

    return 0;
}
