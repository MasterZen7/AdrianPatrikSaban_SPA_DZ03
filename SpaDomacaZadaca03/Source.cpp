#include <iostream>
#include <vector>
#include <queue>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

int rows = 20;
int cols = 40;
int inf = 1e9;

struct Point {
    int row, col;
    bool operator==(const Point& other) const {
        return row == other.row && col == other.col;
    }
    bool operator!=(const Point& other) const {
        return !(*this == other);
    }
};

struct Node {
    Point pt;
    int dist;
    bool operator>(const Node& other) const {
        return dist > other.dist;
    }
};

void clear_console() {
    system("cls");
}

void print_grid(const vector<vector<char>>& grid) {
    clear_console();
    for (const auto& row : grid) {
        for (const auto& cell : row) {
            cout << cell;
        }
        cout << '\n';
    }
}

bool is_valid(int row, int col) {
    return (row >= 0) && (row < rows) && (col >= 0) && (col < cols);
}

vector<Point> dijkstra(const vector<vector<char>>& grid, Point src, Point dest) {
    vector<vector<int>> dist(rows, vector<int>(cols, inf));
    vector<vector<Point>> prev(rows, vector<Point>(cols, { -1, -1 }));
    priority_queue<Node, vector<Node>, greater<Node>> pq;

    vector<int> dirX = { 0, 0, -1, 1 };
    vector<int> dirY = { 1, -1, 0, 0 };

    dist[src.row][src.col] = 0;
    pq.push({ src, 0 });

    while (!pq.empty()) {
        Point u = pq.top().pt;
        pq.pop();

        if (u == dest) break;

        for (int i = 0; i < 4; ++i) {
            int newRow = u.row + dirX[i];
            int newCol = u.col + dirY[i];

            if (is_valid(newRow, newCol) && grid[newRow][newCol] != '|') {
                int alt = dist[u.row][u.col] + 1;
                if (alt < dist[newRow][newCol]) {
                    dist[newRow][newCol] = alt;
                    prev[newRow][newCol] = u;
                    pq.push({ {newRow, newCol}, alt });
                }
            }
        }
    }

    vector<Point> path;
    for (Point at = dest; at.row != -1 && at.col != -1; at = prev[at.row][at.col]) {
        path.push_back(at);
    }
    reverse(path.begin(), path.end());

    if (path.empty() || path[0] != src) {
        return {};
    } else {
        return path;
    }
}

void find_path(Point A, Point B, const vector<vector<char>>& grid) {
    vector<vector<char>> displayGrid = grid;

    vector<Point> path = dijkstra(grid, { A.row - 1, A.col - 1 }, { B.row - 1, B.col - 1 });
    if (path.empty()) {
        cout << "No path found!\n";
        return;
    }

    int currRow = A.row - 1;
    int currCol = A.col - 1;

    for (const auto& p : path) {
        while (currCol != p.col) {
            displayGrid[currRow][currCol] = 'A';
            print_grid(displayGrid);

            auto start = high_resolution_clock::now();
            auto end = start + milliseconds(100);

            while (high_resolution_clock::now() < end) {
            }

            displayGrid[currRow][currCol] = '*';

            if (currCol < p.col) {
                currCol++;
            } else if (currCol > p.col) {
                currCol--;
            }
        }

        while (currRow != p.row) {
            displayGrid[currRow][currCol] = 'A';
            print_grid(displayGrid);

            auto start = high_resolution_clock::now();
            auto end = start + milliseconds(100);

            while (high_resolution_clock::now() < end) {
            }

            displayGrid[currRow][currCol] = '*';

            if (currRow < p.row) {
                currRow++;
            } else if (currRow > p.row) {
                currRow--;
            }
        }
    }

    displayGrid[B.row - 1][B.col - 1] = 'B';
    print_grid(displayGrid);
}

void generate_random_walls(vector<vector<char>>& grid, Point A, Point B) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, rows - 1);
    uniform_int_distribution<> dis_col(0, cols - 1);

    int wall_count = (rows * cols) / 4;
    for (int i = 0; i < wall_count; ++i) {
        int r = dis(gen);
        int c = dis_col(gen);
        if (grid[r][c] == '.' && !(r == A.row - 1 && c == A.col - 1) && !(r == B.row - 1 && c == B.col - 1)) {
            grid[r][c] = '|';
        }
    }
}

bool ensure_path_exists(Point A, Point B, vector<vector<char>>& grid) {
    while (true) {
        generate_random_walls(grid, A, B);
        if (!dijkstra(grid, { A.row - 1, A.col - 1 }, { B.row - 1, B.col - 1 }).empty()) {
            return true;
        }
    }
}

int main() {
    Point A, B;
    vector<vector<char>> grid(rows, vector<char>(cols, '.'));

    cout << "Enter row for point A (1-20): ";
    cin >> A.row;
    cout << "Enter column for point A (1-40): ";
    cin >> A.col;

    cout << "Enter row for point B (1-20): ";
    cin >> B.row;
    cout << "Enter column for point B (1-40): ";
    cin >> B.col;

    grid[A.row - 1][A.col - 1] = 'A';
    grid[B.row - 1][B.col - 1] = 'B';

    ensure_path_exists(A, B, grid);

    find_path(A, B, grid);
    return 0;
}