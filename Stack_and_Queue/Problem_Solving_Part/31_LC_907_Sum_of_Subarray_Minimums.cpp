#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

const int MOD = 1e9 + 7;

// ==========================================
// Approach 1: Baseline Architecture (Brute-Force)
// Time Complexity: O(N^2) | Space Complexity: O(1)
// ==========================================
class SolutionBaseline {
public:
    int sumSubarrayMins(const vector<int>& arr) const {
        long long total_sum = 0;
        size_t n = arr.size();

        for (size_t i = 0; i < n; ++i) {
            int curr_min = arr[i];
            for (size_t j = i; j < n; ++j) {
                curr_min = std::min(curr_min, arr[j]);
                total_sum = (total_sum + curr_min) % MOD;
            }
        }
        return static_cast<int>(total_sum);
    }
};

// ==========================================
// Approach 2: Standard Interview Architecture (Two-Pass)
// Uses two monotonic stacks to explicitly find left and right bounds.
// Handles duplicates using strictly less (`<`) and less-equal (`<=`).
// Time Complexity: Strict O(N) | Space Complexity: O(N)
// ==========================================
class SolutionStandard {
public:
    int sumSubarrayMins(const vector<int>& arr) const {
        size_t n = arr.size();
        vector<size_t> left(n), right(n);
        vector<size_t> st;
        st.reserve(n);

        // Find previous strictly less element
        for (size_t i = 0; i < n; ++i) {
            while (!st.empty() && arr[st.back()] >= arr[i]) {
                st.pop_back();
            }
            // If stack empty, distance is i + 1. Else distance is i - st.back()
            left[i] = st.empty() ? (i + 1) : (i - st.back());
            st.push_back(i);
        }

        st.clear();

        // Find next less or equal element
        for (size_t i = n; i > 0; --i) {
            size_t idx = i - 1;
            while (!st.empty() && arr[st.back()] > arr[idx]) {
                st.pop_back();
            }
            right[idx] = st.empty() ? (n - idx) : (st.back() - idx);
            st.push_back(idx);
        }

        long long total_sum = 0;
        for (size_t i = 0; i < n; ++i) {
            long long contribution = (1LL * arr[i] * left[i] * right[i]) % MOD;
            total_sum = (total_sum + contribution) % MOD;
        }

        return static_cast<int>(total_sum);
    }
};

// ==========================================
// Approach 3: Ultra-Optimized Architecture (One-Pass Extreme)
// FAANG Flex: Solves the problem in a single pass without extra arrays
// using the 'Sentinel Virtual Zero' trick.
// Time Complexity: Strict O(N) | Space Complexity: O(N) (Stack only)
// ==========================================
class SolutionExtreme {
public:
    int sumSubarrayMins(const vector<int>& arr) const {
        size_t n = arr.size();
        long long total_sum = 0;

        // Stack stores INDICES (Crucial for distance calculation)
        vector<size_t> st;
        st.reserve(n);

        // Loop runs up to `n` (inclusive) to trigger the Sentinel Zero
        for (size_t i = 0; i <= n; ++i) {
            // The problem states 1 <= arr[i].
            // A virtual 0 at the end forces all remaining elements to pop.
            int curr_val = (i == n) ? 0 : arr[i];

            // Pop elements greater than or equal to curr_val
            // Using `>=` perfectly handles duplicates without double counting!
            while (!st.empty() && arr[st.back()] >= curr_val) {
                size_t popped_idx = st.back();
                st.pop_back();

                // Distance to the left boundary (Previous Smaller Element)
                size_t left_dist = st.empty() ? (popped_idx + 1) : (popped_idx - st.back());

                // Distance to the right boundary (Next Smaller or Equal Element)
                size_t right_dist = i - popped_idx;

                // Contribution = value * left_dist * right_dist
                long long contribution = (1LL * arr[popped_idx] * left_dist * right_dist) % MOD;

                total_sum = (total_sum + contribution) % MOD;
            }
            st.push_back(i);
        }

        return static_cast<int>(total_sum);
    }
};

// ==========================================
// Test Execution Engine
// ==========================================
string format_vector(const vector<int>& v) {
    if (v.empty()) return "[]";
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

void runComparativeTest(const string& test_name, const vector<int>& arr, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: arr = " << format_vector(arr) << "\n";

    SolutionStandard sol_std;
    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    int res_std = sol_std.sumSubarrayMins(arr);
    int res_ext = sol_ext.sumSubarrayMins(arr);
    int res_base = sol_base.sumSubarrayMins(arr);

    cout << "Expected:  " << expected << "\n";
    cout << "Extreme:   " << res_ext << "\n";
    cout << "Standard:  " << res_std << "\n";

    bool passed = (res_ext == expected && res_std == expected && res_base == expected);

    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 907: Sum of Subarray Minimums (One-Pass) ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard)", {3, 1, 2, 4}, 17);

        runComparativeTest("Example 2 (Large Values)", {11, 81, 94, 43, 3}, 444);

        // The Duplicate Bug Case (Prevents over-counting)
        // [2]*3 + [2,2]*2 + [2,2,2]*1 = 6+4+2 = 12
        runComparativeTest("Edge Case 1 (Duplicates)", {2, 2, 2}, 12);

        runComparativeTest("Edge Case 2 (Descending)", {4, 3, 2, 1}, 20);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}