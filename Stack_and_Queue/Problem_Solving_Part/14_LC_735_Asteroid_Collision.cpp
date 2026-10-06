/**
 * @file Asteroid_Collision.cpp
 * @brief Enterprise-grade solution for LeetCode 735.
 * @details Compares an Explicit std::stack architecture against a highly
 *          Optimized Vector-as-Stack approach. Eliminates O(N) reversal
 *          overhead and features strict type safety, memory pre-allocation,
 *          and modular testing.
 */

#include <algorithm>
#include <cmath>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

// Specific using declarations to maintain namespace hygiene
using std::abs;
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::stack;
using std::string;
using std::vector;

string format_vector(const vector<int>& v) {
    string res = "[";
    res.reserve(v.size() * 6 + 2);
    for (size_t i = 0; i < v.size(); ++i) {
        res += std::to_string(v[i]);
        if (i != v.size() - 1) {
            res += ", ";
        }
    }
    res += "]";
    return res;
}

// ==========================================
// Approach 1: Baseline Architecture (Explicit Stack)
// Clear LIFO logic, but requires an O(N) reversal at the end.
// Time Complexity: O(N) | Space Complexity: O(N) auxiliary
// ==========================================
class SolutionBaseline {
public:
    vector<int> asteroidCollision(const vector<int>& asteroids) const {
        stack<int> st;

        for (int ast : asteroids) {
            bool destroyed = false;

            // Collision happens ONLY when stack top is moving right ( > 0 )
            // and current asteroid is moving left ( < 0 )
            while (!st.empty() && st.top() > 0 && ast < 0) {
                if (st.top() < abs(ast)) {
                    st.pop();  // Stack top explodes, current asteroid continues to fight
                    continue;
                } else if (st.top() == abs(ast)) {
                    st.pop();  // Both explode
                    destroyed = true;
                    break;
                } else {
                    destroyed = true;  // Current asteroid explodes
                    break;
                }
            }

            if (!destroyed) {
                st.push(ast);
            }
        }

        // Reconstruct the surviving asteroids
        int size = static_cast<int>(st.size());
        vector<int> res(size);
        for (int i = size - 1; i >= 0; --i) {
            res[i] = st.top();
            st.pop();
        }

        return res;
    }
};

// ==========================================
// Approach 2: Optimized Architecture (Vector as Stack)
// Output vector acts as the stack. Eliminates the need for std::reverse.
// Time Complexity: Strict O(N) | Space Complexity: Strict O(1) auxiliary
// ==========================================
class SolutionOptimized {
public:
    vector<int> asteroidCollision(const vector<int>& asteroids) const {
        vector<int> res;

        // HIGHLIGHT: Memory Pre-allocation to prevent dynamic reallocations.
        // In the worst case, no asteroids collide (e.g., [-5, -10, 5, 10]).
        res.reserve(asteroids.size());

        for (int ast : asteroids) {
            bool destroyed = false;

            // Vector's back() acts precisely like stack's top()
            while (!res.empty() && res.back() > 0 && ast < 0) {
                if (res.back() < -ast) {
                    res.pop_back();  // Previous asteroid destroyed
                    continue;        // Current asteroid stays alive to fight next one
                } else if (res.back() == -ast) {
                    res.pop_back();  // Both mutually destroy each other
                    destroyed = true;
                    break;
                } else {
                    destroyed = true;  // Current asteroid destroyed
                    break;
                }
            }

            if (!destroyed) {
                res.push_back(ast);
            }
        }

        // HIGHLIGHT: Zero overhead! The vector is already left-to-right.
        return res;
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
void runComparativeTest(const string& test_name, const vector<int>& asteroids, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Asteroids: " << format_vector(asteroids) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    vector<int> res_opt = sol_opt.asteroidCollision(asteroids);
    vector<int> res_base = sol_base.asteroidCollision(asteroids);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Optimized: " << format_vector(res_opt) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LC 735: Asteroid Collision ---\n\n";

    try {
        // 1. Example 1: Basic Collision
        runComparativeTest("Example 1 (Basic Collision)", {5, 10, -5}, {5, 10});

        // 2. Example 2: Mutual Destruction
        runComparativeTest("Example 2 (Mutual Destruction)", {8, -8}, {});

        // 3. Example 3: Multiple Collisions
        runComparativeTest("Example 3 (Multiple Collisions)", {10, 2, -5}, {10});

        // 4. Example 4: Chain Reactions (Leetcode custom example)
        runComparativeTest("Example 4 (Chain Reactions)", {3, 5, -6, 2, -1, 4}, {-6, 2, 4});

        // 5. Edge Case: No collisions (Diverging)
        runComparativeTest("Edge Case 1 (Diverging Asteroids)", {-5, -10, 5, 10}, {-5, -10, 5, 10});

        // 6. Edge Case: One huge asteroid destroying everything
        runComparativeTest("Edge Case 2 (The Juggernaut)", {1, 2, 3, 4, 5, 6, -100}, {-100});

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}