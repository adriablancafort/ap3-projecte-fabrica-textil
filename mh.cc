#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <random>

using namespace std;
using namespace std::chrono;

struct Rectangle {
    int count;
    int width;
    int height;
};

struct Solution {
    vector<vector<int>> positions;
    int length;
    double fitness;
};

int W, N;
vector<Rectangle> rectangles;
double startTime;
mt19937 rng(random_device{}());

void readInput(const string& inputFile) {
    ifstream in(inputFile);
    in >> W >> N;
    rectangles.resize(N);
    for (int i = 0; i < N; ++i) {
        in >> rectangles[i].count >> rectangles[i].width >> rectangles[i].height;
    }
    in.close();
}

void writeOutput(const string& outputFile, const Solution& bestSolution) {
    ofstream out(outputFile);
    double elapsedTime = duration_cast<duration<double>>(steady_clock::now() - startTime).count();
    out << fixed << setprecision(1) << elapsedTime << endl;
    out << bestSolution.length << endl;
    for (const auto& pos : bestSolution.positions) {
        out << pos[0] << " " << pos[1] << " " << pos[2] << " " << pos[3] << endl;
    }
    out.close();
}

bool placeRectangles(int L, vector<vector<int>>& positions) {
    vector<vector<bool>> grid(L, vector<bool>(W, false));
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

double evaluateFitness(const Solution& solution) {
    return 1.0 / solution.length;
}

Solution generateRandomSolution() {
    Solution solution;
    solution.length = W; // Starting with a minimal length
    while (!placeRectangles(solution.length, solution.positions)) {
        solution.length++;
        solution.positions.clear();
    }
    solution.fitness = evaluateFitness(solution);
    return solution;
}

Solution crossover(const Solution& parent1, const Solution& parent2) {
    Solution child;
    child.length = (parent1.length + parent2.length) / 2;
    while (!placeRectangles(child.length, child.positions)) {
        child.length++;
        child.positions.clear();
    }
    child.fitness = evaluateFitness(child);
    return child;
}

void mutate(Solution& solution) {
    solution.length += rng() % 3 - 1; // Randomly adjust length by -1, 0, or 1
    solution.positions.clear();
    while (!placeRectangles(solution.length, solution.positions)) {
        solution.length++;
        solution.positions.clear();
    }
    solution.fitness = evaluateFitness(solution);
}

void geneticAlgorithm(const string& outputFile) {
    const int populationSize = 50;
    const int generations = 100;
    const double mutationRate = 0.1;

    vector<Solution> population(populationSize);
    for (int i = 0; i < populationSize; ++i) {
        population[i] = generateRandomSolution();
    }

    for (int gen = 0; gen < generations; ++gen) {
        sort(population.begin(), population.end(), [](const Solution& a, const Solution& b) {
            return a.fitness > b.fitness;
        });

        vector<Solution> newPopulation;
        for (int i = 0; i < populationSize / 2; ++i) {
            newPopulation.push_back(population[i]);
            newPopulation.push_back(crossover(population[i], population[(i + 1) % (populationSize / 2)]));
        }

        for (int i = 0; i < populationSize; ++i) {
            if (rng() / double(rng.max()) < mutationRate) {
                mutate(newPopulation[i]);
            }
        }

        population = newPopulation;
    }

    Solution bestSolution = *max_element(population.begin(), population.end(), [](const Solution& a, const Solution& b) {
        return a.fitness < b.fitness;
    });

    writeOutput(outputFile, bestSolution);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " <input_file> <output_file>" << endl;
        return 1;
    }

    startTime = steady_clock::now();
    readInput(argv[1]);
    geneticAlgorithm(argv[2]);

    return 0;
}