#include <algorithm>
#include <iostream>
#include <limits>
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
    long long subArrayRanges(const vector<int>& nums) const {
        long long total_range = 0;
        size_t n = nums.size();

        for (size_t i = 0; i < n; ++i) {
            int curr_min = nums[i];
            int curr_max = nums[i];

            for (size_t j = i; j < n; ++j) {
                curr_min = std::min(curr_min, nums[j]);
                curr_max = std::max(curr_max, nums[j]);
                total_range += static_cast<long long>(curr_max - curr_min);
            }
        }
        return total_range;
    }
};

class SolutionExtreme {
public:
    long long subArrayRanges(const vector<int>& nums) const {
        size_t n = nums.size();

        long long sum_min = 0;
        long long sum_max = 0;

        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i <= n; ++i) {
            int curr_val = (i == n) ? std::numeric_limits<int>::min() : nums[i];

            while (!st.empty() && nums[st.back()] >= curr_val) {
                size_t popped_idx = st.back();
                st.pop_back();

                size_t left_dist = st.empty() ? (popped_idx + 1) : (popped_idx - st.back());
                size_t right_dist = i - popped_idx;

                sum_min += 1LL * nums[popped_idx] * left_dist * right_dist;
            }

            if (i < n) st.push_back(i);
        }

        st.clear();

        for (size_t i = 0; i <= n; ++i) {
            int curr_val = (i == n) ? std::numeric_limits<int>::max() : nums[i];

            while (!st.empty() && nums[st.back()] <= curr_val) {
                size_t popped_idx = st.back();
                st.pop_back();

                size_t left_dist = st.empty() ? (popped_idx + 1) : (popped_idx - st.back());
                size_t right_dist = i - popped_idx;

                sum_max += 1LL * nums[popped_idx] * left_dist * right_dist;
            }

            if (i < n) st.push_back(i);
        }

        return sum_max - sum_min;
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

void runComparativeTest(const string& test_name, const vector<int>& nums, long long expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: nums = " << format_vector(nums) << "\n";

    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    long long res_ext = sol_ext.subArrayRanges(nums);
    long long res_base = sol_base.subArrayRanges(nums);

    cout << "Expected:  " << expected << "\n";
    cout << "Extreme:   " << res_ext << "\n";
    cout << "Baseline:  " << res_base << "\n";

    bool passed = (res_ext == expected && res_base == expected);
    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 2104: Sum of Subarray Ranges (O(N) Extreme) ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard)", {1, 2, 3}, 4);

        runComparativeTest("Example 2 (Duplicates)", {1, 3, 3}, 4);

        runComparativeTest("Example 3 (Negatives)", {4, -2, -3, 4, 1}, 59);

        runComparativeTest("Edge Case (All Identical)", {5, 5, 5, 5}, 0);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}