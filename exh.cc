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

bool placeRectangles(int L, vector<vector<int>>& positions) {
    // Implementar la lògica per col·locar els rectangles en el rotlle de longitud L
    // Retornar true si és possible col·locar tots els rectangles, false en cas contrari
    return true;
}

void exhaustiveSearch(const string& outputFile) {
    int L = 0;
    vector<vector<int>> bestPositions;
    while (true) {
        vector<vector<int>> positions;
        if (placeRectangles(L, positions)) {
            bestPositions = positions;
            writeOutput(outputFile, L, bestPositions);
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
    exhaustiveSearch(argv[2]);

    return 0;
}