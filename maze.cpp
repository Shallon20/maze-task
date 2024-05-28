// NAME: SHALLON MARIA OMWONO
// STUDENT ID: 230ADB062

// references:
// https://stackoverflow.com/questions/10730447/c-maze-assignment
// https://www.geeksforgeeks.org/c-program-for-dijkstras-shortest-path-algorithm-greedy-algo-7/
// https://github.com/ra101/Maze-Solver-Cpp
// https://rosettacode.org/wiki/Dijkstra%27s_algorithm#C++
// chatGPT on time and space complexities

#include <iostream>
#include <fstream>
#include <queue>
#include <vector>
#include <climits>
#include <cctype> // isdigit function

using namespace std;

const int MAX_SIZE = 101; // Maximum maze size
const int DIRECTIONS = 8; // Number of possible movement directions
int dx[DIRECTIONS] = {-1, -1, -1, 0, 0, 1, 1, 1}; // rows
int dy[DIRECTIONS] = {-1, 0, 1, -1, 1, -1, 0, 1}; // columns

// Cell structure in the maze
struct Cell {
    int x, y;
    int distance;
    int coins; 
    vector<pair<int, int>> path; // Path taken to reach cell

    // Prioritizes cells with fewer coins when distances are equal
    bool operator>(const Cell& other) const {
        if (distance == other.distance)
            return coins > other.coins;
        return distance > other.distance;
    }
};

// Check if cell is not in lava and not a wall
bool isValid(int x, int y, int rows, int cols, char maze[MAX_SIZE][MAX_SIZE]) {
    return (x >= 0 && x < rows && y >= 0 && y < cols && maze[x][y] != 'X');
}

// Finds shortest path from start cell to goal cell
int findShortestPath(char maze[MAX_SIZE][MAX_SIZE], int rows, int cols, int startX, int startY, int goalX, int goalY, vector<pair<int, int>>& finalPath) {
    // Priority queue to explore cells with least distance and coins first
    priority_queue<Cell, vector<Cell>, greater<Cell>> pq;

    // 2D vector track visited cells
    vector<vector<bool>> visited(static_cast<size_t>(rows), vector<bool>(static_cast<size_t>(cols), false));

    // Push starting cell into priority queue(0 distance and 0 coins)
    pq.push({startX, startY, 0, 0, {{startX, startY}}});
    visited[static_cast<size_t>(startX)][static_cast<size_t>(startY)] = true;

    // Cells in priority queue computed
    while (!pq.empty()) {
        Cell current = pq.top();
        pq.pop();

        // If goal cell is reached, final path is set  and total coins collected returned
        if (current.x == goalX && current.y == goalY) {
            finalPath = current.path;
            return current.coins;
        }

        // All possible movements from the current cell are explored
        for (int i = 0; i < DIRECTIONS; ++i) {
            int newX = current.x + dx[i];
            int newY = current.y + dy[i];

            // Check if new cell is valid and unvisited
            if (isValid(newX, newY, rows, cols, maze) && !visited[static_cast<size_t>(newX)][static_cast<size_t>(newY)]) {
                visited[static_cast<size_t>(newX)][static_cast<size_t>(newY)] = true;
                int newCoins = current.coins;
                if (isdigit(maze[newX][newY])) {
                    newCoins += maze[newX][newY] - '0';  // Convert char to int
                }
                vector<pair<int, int>> newPath = current.path;
                newPath.emplace_back(newX, newY);
                pq.push({newX, newY, current.distance + 1, newCoins, newPath});
            }
        }
    }
    return -1; // If goal is not reached
}

int main() {
    const char* filenames[] = {"maze_11x11.txt", "maze_31x31.txt", "maze_101x101.txt"}; // Input maze files
    ofstream outputFile("output.txt"); // Output file that saves results

    // Check if output file is open
    if (!outputFile.is_open()) {
        cerr << "Failed to open output.txt" << endl;
        return 1;
    }

    // Maze files processing
    for (const char* filename : filenames) {
        ifstream inputFile(filename); 
        // Open input file
        if (!inputFile.is_open()) {
            cerr << "Failed to open " << filename << endl;
            outputFile << "Failed to open " << filename << endl;
            continue;
        }

        char maze[MAX_SIZE][MAX_SIZE]; // 2D array to store maze
        int rows = 0, cols = 0; // Number of rows and columns
        int startX = -1, startY = -1, goalX = -1, goalY = -1; // Start and goal coordinates

        string line;
        // Maze read from the input file line by line
        while (getline(inputFile, line)) {
            if (cols == 0) {
                cols = static_cast<int>(line.size());
            }
            for (size_t j = 0; j < line.size(); ++j) {
                maze[rows][static_cast<int>(j)] = line[j];
                // Start and goal 
                if (line[j] == 'S') {
                    startX = rows;
                    startY = static_cast<int>(j);
                } else if (line[j] == 'G') {
                    goalX = rows;
                    goalY = static_cast<int>(j);
                }
            }
            ++rows;
        }
        inputFile.close(); // Input file closes

        // Check if maze format is valid
        if (startX == -1 || startY == -1 || goalX == -1 || goalY == -1) {
            cerr << "Invalid maze format in " << filename << endl;
            outputFile << "Invalid maze format in " << filename << endl;
            continue;
        }

        outputFile << "\n Results are as follows: " << filename << endl;

        vector<pair<int, int>> finalPath; // Vector to store the final path
        // Shortest path and total coins collected
        int coinsCollected = findShortestPath(maze, rows, cols, startX, startY, goalX, goalY, finalPath);

        // Print and save the results
        if (coinsCollected == -1) {
            cout << "No path found in " << filename << endl;
            outputFile << "No path found in " << filename << endl;
        } else {
            
            outputFile << "\t Solution for " << filename << ": " << coinsCollected << endl;

            outputFile << "\tPath taken:" ;
            int totalCoins = 0;
            // Collecting coins steps
            for (const auto& step : finalPath) {
                outputFile << "\t(" << step.first << "," << step.second << ") ";
                if (isdigit(maze[step.first][step.second])) {
                    totalCoins += maze[step.first][step.second] - '0';
                    outputFile << "+" << (maze[step.first][step.second] - '0') << " ";
                }
            }
        
            outputFile << "= " << totalCoins << endl;
        }
    }
    outputFile.close(); // Close output file
    return 0;
}
