/**
 * @file Car_Fleet.cpp
 * @brief FAANG Interview-optimized combined solution for LeetCode 853.
 * @details Compares a map-based baseline against an O(N log N) optimal
 *          architecture using contiguous memory structs. Replaces the explicit
 *          stack with a "Virtual Stack" (O(1) tracking) for maximum efficiency.
 */

#include <algorithm>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

// ==========================================
// Approach 1: Baseline Architecture (Implicit Sorting with Map)
// Uses std::map to automatically sort cars by position in descending order.
// Time Complexity: O(N log N) | Space Complexity: O(N) (Tree nodes allocation)
// ==========================================
class SolutionBaseline {
public:
    int carFleet(int target, const vector<int>& position, const vector<int>& speed) const {
        // Map stores position -> time. greater<int> sorts closest to target first.
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

// ==========================================
// Approach 2: Interview-Optimized Architecture (Structs + Virtual Stack)
// FAANG-level execution. Bypasses tree-node allocations by using contiguous
// vector memory and eliminates the explicit stack entirely via state tracking.
// Time Complexity: O(N log N) | Space Complexity: O(N) contiguous
// ==========================================

// FAANG Flex: Custom struct with constructor for zero-copy emplace_back
struct Car {
    int pos;
    double time;
    Car(int p, double t) : pos(p), time(t) {}
};

class SolutionInterview {
public:
    int carFleet(int target, const vector<int>& position, const vector<int>& speed) const {
        size_t n = position.size();
        if (n == 0) return 0;

        vector<Car> cars;
        cars.reserve(n);  // Zero dynamic reallocation

        for (size_t i = 0; i < n; ++i) {
            double time = static_cast<double>(target - position[i]) / speed[i];
            // FAANG Flex: emplace_back constructs the object directly in memory
            cars.emplace_back(position[i], time);
        }

        // Sort descending by position (cars closest to target come first)
        std::sort(cars.begin(), cars.end(), [](const Car& a, const Car& b) { return a.pos > b.pos; });

        int fleets = 0;
        double max_time_ahead = 0.0;

        // FAANG Flex: Virtual Stack. Since we only ever need the top of the stack,
        // we can replace the entire O(N) stack with a single O(1) tracking variable.
        for (size_t i = 0; i < n; ++i) {
            if (cars[i].time > max_time_ahead) {
                // This car takes strictly longer than the fleet ahead. It cannot
                // catch up, so it establishes a brand new fleet.
                max_time_ahead = cars[i].time;
                fleets++;
            }
            // Else: It catches up and gets absorbed. No action needed.
        }

        return fleets;
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
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

    SolutionInterview sol_opt;
    SolutionBaseline sol_base;

    int res_opt = sol_opt.carFleet(target, pos, speed);
    int res_base = sol_base.carFleet(target, pos, speed);

    cout << "Expected:  " << expected << "\n";
    cout << "Optimized: " << res_opt << "\n";
    cout << "Baseline:  " << res_base << "\n";

    cout << "Result:    " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LC 853: Car Fleet (Interview Mode) ---\n\n";

    try {
        // 1. Example 1: Standard fluctuations
        runComparativeTest("Example 1 (Standard Fleets)", 12, {10, 8, 0, 5, 3}, {2, 4, 1, 1, 3}, 3);

        // 2. Example 2: Single car
        runComparativeTest("Example 2 (Single Car)", 10, {3}, {3}, 1);

        // 3. Example 3: Multiple catch-ups
        runComparativeTest("Example 3 (Chain Catch-up)", 100, {0, 2, 4}, {4, 2, 1}, 1);

        // 4. Edge Case: All cars same speed
        runComparativeTest("Edge Case 1 (Same Speed)", 10, {1, 2, 3}, {2, 2, 2}, 3);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}