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

// ==========================================
// Approach 1: Baseline Architecture (Optimized Brute-Force)
// Methodologically proves why Monotonic Stacks are required.
// Time Complexity: O(N^2) | Space Complexity: O(1)
// ==========================================
class SolutionBaseline {
public:
    bool find132pattern(const vector<int>& nums) const {
        size_t n = nums.size();
        if (n < 3) {
            return false;
        }

        int min_i = nums[0];  // Tracks the best possible s1

        for (size_t j = 1; j < n - 1; ++j) {
            // Keep track of the minimum element before index j (this is s1)
            min_i = std::min(min_i, nums[j - 1]);

            // Look for any element after index j (this is s3)
            for (size_t k = j + 1; k < n; ++k) {
                if (nums[k] > min_i && nums[k] < nums[j]) {
                    return true;
                }
            }
        }
        return false;
    }
};

// ==========================================
// Approach 2: Interview-Optimized Architecture (Monotonic Stack)
// FAANG-level robust execution. Scans Right-to-Left.
// Time Complexity: Strict O(N) | Space Complexity: O(N)
// ==========================================
class SolutionOptimized {
public:
    bool find132pattern(const vector<int>& nums) const {
        size_t n = nums.size();
        if (n < 3) {
            return false;
        }

        // FAANG Flex: Vector-as-Stack to avoid dynamic reallocations
        vector<int> st;
        st.reserve(n);

        // Track the maximum possible s3 (the '2' in the 132 pattern)
        int s3 = std::numeric_limits<int>::min();

        // Traverse backwards: Right-to-Left
        // FAANG Flex: Safe unsigned-to-signed casting to prevent underflow
        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            int current_num = nums[i];  // Value copy for primitives!

            // If we find an s1 that is smaller than our established s3, we win.
            if (current_num < s3) {
                return true;
            }

            // If current number is greater than stack top, it's a candidate for s2.
            // Pop smaller elements to maximize s3.
            while (!st.empty() && st.back() < current_num) {
                s3 = st.back();
                st.pop_back();
            }

            // Push current number as the new potential s2 candidate
            st.push_back(current_num);
        }

        return false;
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
    res.reserve(v.size() * 6 + 2);  // Heuristic memory pre-allocation

    for (size_t i = 0; i < v.size() - 1; ++i) {
        res += std::to_string(v[i]);
        res += ", ";
    }

    res += std::to_string(v.back());
    res += "]";
    return res;
}

void runComparativeTest(const string& test_name, const vector<int>& nums, bool expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: nums = " << format_vector(nums) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    bool res_opt = sol_opt.find132pattern(nums);
    bool res_base = sol_base.find132pattern(nums);

    cout << "Expected:  " << (expected ? "true" : "false") << "\n";
    cout << "Optimized: " << (res_opt ? "true" : "false") << "\n";
    cout << "Baseline:  " << (res_base ? "true" : "false") << "\n";

    cout << "Result:    " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LC 456: 132 Pattern (Interview Mode) ---\n\n";

    try {
        // 1. Example 1: Strictly increasing (No pattern)
        runComparativeTest("Example 1 (No Pattern)", {1, 2, 3, 4}, false);

        // 2. Example 2: Classic 132
        runComparativeTest("Example 2 (Classic Pattern)", {3, 1, 4, 2}, true);

        // 3. Example 3: Negative numbers
        runComparativeTest("Example 3 (Negatives included)", {-1, 3, 2, 0}, true);

        // 4. Edge Case: Valley shape (Not 132)
        runComparativeTest("Edge Case 1 (Valley Shape)", {5, 4, 3, 2, 1}, false);

        // 5. Edge Case: Duplicate numbers
        runComparativeTest("Edge Case 2 (Duplicates)", {1, 0, 1, -4, -3}, false);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}