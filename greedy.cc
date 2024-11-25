#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>

using namespace std;
using namespace std::chrono;

struct Rectangle {
    int count;
    int width;
    int height;
};

int W, N;
vector<Rectangle> rectangles;
double startTime;

void readInput(const string& inputFile) {
    ifstream in(inputFile);
    in >> W >> N;
    rectangles.resize(N);
    for (int i = 0; i < N; ++i) {
        in >> rectangles[i].count >> rectangles[i].width >> rectangles[i].height;
    }
    in.close();
}

void writeOutput(const string& outputFile, int L, const vector<vector<int>>& positions) {
    ofstream out(outputFile);
    double elapsedTime = duration_cast<duration<double>>(steady_clock::now() - startTime).count();
    out << fixed << setprecision(1) << elapsedTime << endl;
    out << L << endl;
    for (const auto& pos : positions) {
        out << pos[0] << " " << pos[1] << " " << pos[2] << " " << pos[3] << endl;
    }
    out.close();
}

bool placeRectanglesGreedy(int& L, vector<vector<int>>& positions) {
    vector<vector<bool>> grid(L, vector<bool>(W, false));
    int y = 0;

    for (const auto& rect : rectangles) {
        for (int k = 0; k < rect.count; ++k) {
            bool placed = false;
            for (int i = 0; i <= L - rect.height && !placed; ++i) {
                for (int j = 0; j <= W - rect.width && !placed; ++j) {
                    bool canPlace = true;
                    for (int x = 0; x < rect.height && canPlace; ++x) {
                        for (int y = 0; y < rect.width && canPlace; ++y) {
                            if (grid[i + x][j + y]) {
                                canPlace = false;
                            }
                        }
                    }
                    if (canPlace) {
                        for (int x = 0; x < rect.height; ++x) {
                            for (int y = 0; y < rect.width; ++y) {
                                grid[i + x][j + y] = true;
                            }
                        }
                        positions.push_back({j, i, j + rect.width - 1, i + rect.height - 1});
                        placed = true;
                    }
                }
            }
            if (!placed) {
                return false;
            }
        }
    }
    return true;
}

void greedySearch(const string& outputFile) {
    int L = 0;
    vector<vector<int>> positions;
    while (true) {
        positions.clear();
        if (placeRectanglesGreedy(L, positions)) {
            writeOutput(outputFile, L, positions);
            break;
        }
        ++L;
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    startTime = steady_clock::now();
    readInput(argv[1]);
    greedySearch(argv[2]);

    return 0;
}