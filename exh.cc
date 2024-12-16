#include <iostream>
#include <fstream>
#include <vector>
#include <chrono>
#include <algorithm>
#include <iomanip>

using namespace std;

struct Rectangle {
    int width;  // pi
    int height; // qi
    int count;  // ni
};

struct Placement {
    int x, y;
    int width, height;
    bool rotated;
};

class TextileCutter {
private:
    int W;  // Width of the roll
    vector<Rectangle> rectangles;
    vector<vector<bool>> used;
    vector<Placement> best_solution;
    int best_length = INT_MAX;
    
    bool can_place(const vector<vector<bool>>& grid, int x, int y, int width, int height, int L) {
        if (x + width > W || y + height > L) return false;
        
        for (int i = y; i < y + height; i++) {
            for (int j = x; j < x + width; j++) {
                if (grid[i][j]) return false;
            }
        }
        return true;
    }

    void place_rectangle(vector<vector<bool>>& grid, int x, int y, int width, int height) {
        for (int i = y; i < y + height; i++) {
            for (int j = x; j < x + width; j++) {
                grid[i][j] = true;
            }
        }
    }

    void remove_rectangle(vector<vector<bool>>& grid, int x, int y, int width, int height) {
        for (int i = y; i < y + height; i++) {
            for (int j = x; j < x + width; j++) {
                grid[i][j] = false;
            }
        }
    }

    void search(vector<vector<bool>>& grid, vector<Placement>& current, vector<int>& remaining, int L) {
        if (all_of(remaining.begin(), remaining.end(), [](int x) { return x == 0; })) {
            if (L < best_length) {
                best_length = L;
                best_solution = current;
                save_solution();
            }
            return;
        }

        for (int i = 0; i < rectangles.size(); i++) {
            if (remaining[i] == 0) continue;

            for (int y = 0; y < L; y++) {
                for (int x = 0; x < W; x++) {
                    // Try normal orientation
                    if (can_place(grid, x, y, rectangles[i].width, rectangles[i].height, L)) {
                        place_rectangle(grid, x, y, rectangles[i].width, rectangles[i].height);
                        current.push_back({x, y, rectangles[i].width, rectangles[i].height, false});
                        remaining[i]--;
                        
                        search(grid, current, remaining, L);
                        
                        remaining[i]++;
                        current.pop_back();
                        remove_rectangle(grid, x, y, rectangles[i].width, rectangles[i].height);
                    }

                    // Try rotated orientation if possible
                    if (rectangles[i].height <= W && rectangles[i].width != rectangles[i].height) {
                        if (can_place(grid, x, y, rectangles[i].height, rectangles[i].width, L)) {
                            place_rectangle(grid, x, y, rectangles[i].height, rectangles[i].width);
                            current.push_back({x, y, rectangles[i].height, rectangles[i].width, true});
                            remaining[i]--;
                            
                            search(grid, current, remaining, L);
                            
                            remaining[i]++;
                            current.pop_back();
                            remove_rectangle(grid, x, y, rectangles[i].height, rectangles[i].width);
                        }
                    }
                }
            }
        }
    }

    void save_solution() const {
        ofstream fout(output_file);
        fout << fixed << setprecision(1) 
             << chrono::duration<double>(chrono::steady_clock::now() - start_time).count() << endl;
        fout << best_length << endl;
        for (const auto& p : best_solution) {
            fout << p.x << " " << p.y << " " 
                 << (p.x + p.width - 1) << " " << (p.y + p.height - 1) << endl;
        }
    }

public:
    string output_file;
    chrono::steady_clock::time_point start_time;

    void read_input(const string& filename) {
        ifstream fin(filename);
        int N;
        fin >> W >> N;
        
        while (N > 0) {
            int ni, pi, qi;
            fin >> ni >> pi >> qi;
            rectangles.push_back({pi, qi, ni});
            N -= ni;
        }
    }

    void solve() {
        start_time = chrono::steady_clock::now();
        vector<int> remaining;
        for (const auto& r : rectangles) {
            remaining.push_back(r.count);
        }

        int min_length = 0;
        for (const auto& r : rectangles) {
            min_length += (r.height * r.count + W - 1) / W;
        }

        for (int L = min_length; L <= 100; L++) {
            vector<vector<bool>> grid(L, vector<bool>(W, false));
            vector<Placement> current;
            search(grid, current, remaining, L);
            if (best_length != INT_MAX) break;
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        cerr << "Usage: " << argv[0] << " input_file output_file" << endl;
        return 1;
    }

    TextileCutter cutter;
    cutter.output_file = argv[2];
    cutter.read_input(argv[1]);
    cutter.solve();
    return 0;
}