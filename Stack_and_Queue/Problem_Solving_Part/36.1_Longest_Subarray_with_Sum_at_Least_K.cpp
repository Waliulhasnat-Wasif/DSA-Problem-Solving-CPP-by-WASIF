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
                if (P[j] - P[i] >= K) {
                    max_len = std::max(max_len, j - i);
                }
            }
        }
        return static_cast<int>(max_len);
    }
};

// ==========================================
// Approach 2: Ultra-Optimized Architecture (2-Pass Monotonic Stack)
// FAANG Flex: Contiguous vector-as-stack, zero reallocations, and strict
// unsigned boundary guards (`st.back() >= j`) to prevent underflow when K <= 0.
// Time Complexity: Strict O(N) | Space Complexity: O(N) contiguous
// ==========================================
class SolutionExtreme {
public:
    int longestSubarray(const vector<int>& nums, int k) const {
        size_t n = nums.size();
        if (n == 0) return 0;

        // Prefix sums in 64-bit to prevent 32-bit integer overflow
        vector<long long> P(n + 1, 0);
        for (size_t i = 0; i < n; ++i) {
            P[i + 1] = P[i] + nums[i];
        }

        // Pass 1 (Left-to-Right): Filter candidate left bounds.
        // Keep strictly DECREASING prefix sums. If i1 < i2 and P[i2] >= P[i1],
        // i1 strictly dominates i2 in both length and ease of reaching sum >= K.
        vector<size_t> st;
        st.reserve(n);
        for (size_t i = 0; i < n; ++i) {
            if (st.empty() || P[i] < P[st.back()]) {
                st.push_back(i);
            }
        }

        // Pass 2 (Right-to-Left): Greedily maximize length (j - st.back()).
        // Safe unsigned loop: j runs from n down to 1, never underflows.
        long long K = static_cast<long long>(k);
        size_t max_len = 0;

        for (size_t j = n; j > 0 && !st.empty(); --j) {
            // Guard: Left bound must be strictly less than right bound j
            while (!st.empty() && st.back() >= j) {
                st.pop_back();
            }
            // Once matched at the rightmost possible j, st.back() can never yield
            // a longer subarray for any smaller j; safe to pop permanently.
            while (!st.empty() && P[j] - P[st.back()] >= K) {
                max_len = std::max(max_len, j - st.back());
                st.pop_back();
            }
        }

        return static_cast<int>(max_len);
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
    cout << "--- Testing Longest Subarray with Sum >= K ---\n\n";

    try {
        runComparativeTest("Example 1 (Negatives + Shifted Start)", {-5, 2, -1, 4, -2}, 3, 4);

        runComparativeTest("Example 2 (Multiple Pops in Pass 2)", {-1, -2, 5, -1}, 2, 3);

        runComparativeTest("Edge Case 1 (Entire Array Valid)", {2, -1, 3, -2, 4}, 6, 5);

        runComparativeTest("Edge Case 2 (Negative K - Boundary Guard Test)", {-1, -2, -3}, -1, 1);

        runComparativeTest("Edge Case 3 (Negative K - Full Array Match)", {-5, -5}, -10, 2);

        runComparativeTest("Edge Case 4 (Zero K with Negatives)", {1, -2, 1, -1}, 0, 3);

        runComparativeTest("Edge Case 5 (All Negatives - Impossible)", {-3, -2, -1}, 1, 0);

        runComparativeTest("Edge Case 6 (Single Element - Valid)", {5}, 5, 1);

        runComparativeTest("Edge Case 7 (Single Element - Invalid)", {4}, 5, 0);

        runComparativeTest("Edge Case 8 (Empty Array)", {}, 1, 0);

        runComparativeTest("Edge Case 9 (64-bit Overflow Protection)", {1000000000, 1000000000, 1000000000}, 2000000000, 3);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}