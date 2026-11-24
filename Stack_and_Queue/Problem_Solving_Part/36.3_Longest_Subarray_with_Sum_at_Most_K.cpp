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
    int longestSubarray(const vector<int>& nums, int k) const {
        size_t n = nums.size();
        vector<long long> P(n + 1, 0);
        for (size_t i = 0; i < n; ++i) {
            P[i + 1] = P[i] + nums[i];
        }

        long long K = static_cast<long long>(k);
        size_t max_len = 0;

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j <= n; ++j) {
                if (P[j] - P[i] <= K) {
                    max_len = std::max(max_len, j - i);
                }
            }
        }
        return static_cast<int>(max_len);
    }
};

class SolutionExtreme {
public:
    int longestSubarray(const vector<int>& nums, int k) const {
        size_t n = nums.size();
        if (n == 0) return 0;

        vector<long long> P(n + 1, 0);
        for (size_t i = 0; i < n; ++i) {
            P[i + 1] = P[i] + nums[i];
        }

        vector<size_t> st;
        st.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            if (st.empty() || P[i] > P[st.back()]) {
                st.push_back(i);
            }
        }

        long long K = static_cast<long long>(k);
        size_t max_len = 0;

        for (size_t j = n; j > 0 && !st.empty(); --j) {
            // Guard: Critical when K <= 0 to prevent invalid right-side matches
            while (!st.empty() && st.back() >= j) {
                st.pop_back();
            }

            while (!st.empty() && P[j] - P[st.back()] <= K) {
                max_len = std::max(max_len, j - st.back());
                st.pop_back();
            }
        }

        return static_cast<int>(max_len);
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

void runComparativeTest(const string& test_name, const vector<int>& nums, int k, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: nums = " << format_vector(nums) << ", k = " << k << "\n";

    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    int res_ext = sol_ext.longestSubarray(nums, k);
    int res_base = sol_base.longestSubarray(nums, k);

    cout << "Expected:  " << expected << "\n";
    cout << "Extreme:   " << res_ext << "\n";
    cout << "Baseline:  " << res_base << "\n";

    bool passed = (res_ext == expected && res_base == expected);
    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing Longest Subarray with Sum <= K ---\n\n";

    try {
        runComparativeTest("Example 1 (Mixed Negatives)", {5, -2, 4, -1, 3}, 3, 3);

        runComparativeTest("Example 2 (All Non-Negative)", {1, 2, 1, 0, 1, 1, 0}, 4, 5);

        runComparativeTest("Edge Case 1 (Negative K - Valid)", {2, -5, 1, -2, 3}, -4, 4);

        runComparativeTest("Edge Case 2 (Negative K Impossible - Guard Test)", {1, 2, 3}, -1, 0);

        runComparativeTest("Edge Case 3 (Zero K - Full Array Match)", {3, -2, -1, 4, -4}, 0, 5);

        runComparativeTest("Edge Case 4 (All Negatives, Positive K)", {-3, -2, -5}, 10, 3);

        runComparativeTest("Edge Case 5 (Single Element - Valid)", {5}, 5, 1);

        runComparativeTest("Edge Case 6 (Single Element - Invalid)", {6}, 5, 0);

        runComparativeTest("Edge Case 7 (Empty Array)", {}, 0, 0);

        runComparativeTest("Edge Case 8 (64-bit Negative Overflow Protection)", {-1000000000, -1000000000, 1000000000}, -1000000000, 3);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}