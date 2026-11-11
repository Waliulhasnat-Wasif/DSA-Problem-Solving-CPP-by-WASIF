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

// ==========================================
// Approach 1: Baseline Architecture (Brute-Force Simulation)
// Simulates the exact physical removal of elements step-by-step.
// Time Complexity: Worst case O(N^2) | Space Complexity: O(N) for buffer
// ==========================================
class SolutionBaseline {
public:
    int totalSteps(const vector<int>& nums) const {
        if (nums.empty()) return 0;

        vector<int> current = nums;
        int steps = 0;

        while (true) {
            vector<int> next_arr;
            next_arr.reserve(current.size());

            // First element is never deleted by a left neighbor
            next_arr.push_back(current[0]);

            bool changed = false;
            for (size_t i = 1; i < current.size(); ++i) {
                // If the left neighbor is STRICTLY greater, this element dies.
                if (current[i - 1] > current[i]) {
                    changed = true;
                } else {
                    next_arr.push_back(current[i]);
                }
            }

            if (!changed) break;  // Array is completely non-decreasing
            current = next_arr;
            steps++;
        }

        return steps;
    }
};

// ==========================================
// Approach 2: Ultra-Optimized Architecture (Monotonic Stack + DP)
// FAANG Flex: Uses a monotonic decreasing stack to find the nearest strictly
// greater element to the left, while maintaining a DP array to track how many
// steps it takes to clear intermediate smaller elements.
// Time Complexity: Strict O(N) | Space Complexity: O(N)
// ==========================================
class SolutionExtreme {
public:
    int totalSteps(const vector<int>& nums) const {
        size_t n = nums.size();
        if (n == 0) return 0;

        vector<int> dp(n, 0);  // dp[i] = steps required to delete nums[i]
        int max_steps = 0;

        // Stack stores INDICES, maintaining a strictly decreasing order of values
        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            int current_steps = 0;

            // Pop smaller or equal elements, they cannot delete nums[i].
            // However, nums[i] must wait for them to be deleted before it interacts
            // with the larger element to its left.
            while (!st.empty() && nums[st.back()] <= nums[i]) {
                current_steps = std::max(current_steps, dp[st.back()]);
                st.pop_back();
            }

            // If stack is NOT empty, top element is strictly greater than nums[i].
            // It will delete nums[i] right after all intermediate elements are gone.
            if (!st.empty()) {
                dp[i] = current_steps + 1;
                max_steps = std::max(max_steps, dp[i]);
            }
            // If stack IS empty, nums[i] is the largest seen so far.
            // It survives forever. dp[i] remains 0.

            st.push_back(i);
        }

        return max_steps;
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

void runComparativeTest(const string& test_name, const vector<int>& nums, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: nums = " << format_vector(nums) << "\n";

    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    int res_ext = sol_ext.totalSteps(nums);
    int res_base = sol_base.totalSteps(nums);

    cout << "Expected:  " << expected << "\n";
    cout << "Extreme:   " << res_ext << "\n";
    cout << "Baseline:  " << res_base << "\n";

    bool passed = (res_ext == expected && res_base == expected);
    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 2289: Steps to Make Array Non-decreasing ---\n\n";

    try {
        runComparativeTest("Example 1 (Multiple Steps)", {5, 3, 4, 4, 7, 3, 6, 11, 8, 5, 11}, 3);

        runComparativeTest("Example 2 (Already Non-decreasing)", {4, 5, 7, 7, 13}, 0);

        runComparativeTest("Edge Case 1 (Strictly Descending - O(N^2) killer)", {10, 9, 8, 7, 6, 5, 4, 3, 2, 1}, 1);

        runComparativeTest("Edge Case 2 (Mountain Shape)", {1, 3, 5, 4, 2}, 1);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}