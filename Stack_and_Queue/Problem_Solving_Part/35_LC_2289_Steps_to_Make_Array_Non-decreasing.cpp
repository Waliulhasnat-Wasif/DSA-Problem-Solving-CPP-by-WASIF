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
            ++steps;
        }

        return steps;
    }
};

class SolutionExtreme {
public:
    int totalSteps(const vector<int>& nums) const {
        size_t n = nums.size();
        if (n == 0) return 0;

        vector<int> dp(n, 0);
        int max_steps = 0;

        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            int current_steps = 0;

            while (!st.empty() && nums[st.back()] <= nums[i]) {
                current_steps = std::max(current_steps, dp[st.back()]);
                st.pop_back();
            }

            if (!st.empty()) {
                dp[i] = current_steps + 1;
                max_steps = std::max(max_steps, dp[i]);
            }

            st.push_back(i);
        }

        return max_steps;
    }
};

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