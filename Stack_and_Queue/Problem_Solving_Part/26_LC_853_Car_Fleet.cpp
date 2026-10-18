#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

class SolutionBaseline {
public:
    int carFleet(int target, const vector<int>& position, const vector<int>& speed) const {
        map<int, double, std::greater<int>> cars;
        size_t n = position.size();

        for (size_t i = 0; i < n; ++i) {
            cars[position[i]] = static_cast<double>(target - position[i]) / speed[i];
        }

        int fleets = 0;
        double max_time = 0.0;

        for (const auto& pair : cars) {
            if (pair.second > max_time) {
                max_time = pair.second;
                fleets++;
            }
        }

        return fleets;
    }
};

struct Car {
    int pos;
    double time;
    Car(int p, double t) : pos(p), time(t) {}
};

class SolutionOptimized {
public:
    int carFleet(int target, const vector<int>& position, const vector<int>& speed) const {
        size_t n = position.size();
        if (n == 0) return 0;

        vector<Car> cars;
        cars.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            double time = static_cast<double>(target - position[i]) / speed[i];
            cars.emplace_back(position[i], time);
        }

        std::sort(cars.begin(), cars.end(), [](const Car& a, const Car& b) { return a.pos > b.pos; });

        int fleets = 0;
        double max_time_ahead = 0.0;

        for (size_t i = 0; i < n; ++i) {
            if (cars[i].time > max_time_ahead) {
                max_time_ahead = cars[i].time;
                ++fleets;
            }
        }

        return fleets;
    }
};

string format_vector(const vector<int>& v) {
    if (v.empty()) {
        return "[]";
    }

    string res = "[";
    res.reserve(v.size() * 6 + 2);

    for (size_t i = 0; i < v.size() - 1; ++i) {
        res += std::to_string(v[i]);
        res += ", ";
    }

    res += std::to_string(v.back());
    res += "]";
    return res;
}

void runComparativeTest(const string& test_name, int target, const vector<int>& pos, const vector<int>& speed, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Target: " << target << " | Pos: " << format_vector(pos) << " | Speed: " << format_vector(speed) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    int res_opt = sol_opt.carFleet(target, pos, speed);
    int res_base = sol_base.carFleet(target, pos, speed);

    cout << "Expected:  " << expected << "\n";
    cout << "Optimized: " << res_opt << "\n";
    cout << "Baseline:  " << res_base << "\n";

    cout << "Result:    " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 853: Car Fleet (Interview Mode) ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard Fleets)", 12, {10, 8, 0, 5, 3}, {2, 4, 1, 1, 3}, 3);

        runComparativeTest("Example 2 (Single Car)", 10, {3}, {3}, 1);

        runComparativeTest("Example 3 (Chain Catch-up)", 100, {0, 2, 4}, {4, 2, 1}, 1);

        runComparativeTest("Edge Case 1 (Same Speed)", 10, {1, 2, 3}, {2, 2, 2}, 3);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}