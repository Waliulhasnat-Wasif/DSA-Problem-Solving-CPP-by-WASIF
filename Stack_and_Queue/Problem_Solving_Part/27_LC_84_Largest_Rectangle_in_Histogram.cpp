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
    int largestRectangleArea(const vector<int>& heights) const {
        int max_area = 0;
        int n = static_cast<int>(heights.size());

        for (int i = 0; i < n; ++i) {
            int min_height = heights[i];
            for (int j = i; j < n; ++j) {
                // Find the minimum height in the current window [i..j]
                min_height = std::min(min_height, heights[j]);
                // Calculate area with the minimum height
                max_area = std::max(max_area, min_height * (j - i + 1));
            }
        }

        return max_area;
    }
};

// ==========================================
// Approach 2: Interview-Optimized Architecture (Monotonic Increasing Stack)
// FAANG-level execution. Finds left and right boundaries concurrently in O(N).
// Eliminates code duplication by simulating a trailing zero (Sentinel).
// Time Complexity: Strict O(N) | Space Complexity: O(N)
// ==========================================
class SolutionOptimized {
public:
    int largestRectangleArea(const vector<int>& heights) const {
        size_t n = heights.size();
        int max_area = 0;

        // Vector-as-Stack storing INDICES to compute widths
        vector<size_t> st;
        st.reserve(n);  // FAANG Flex: Zero dynamic reallocation

        // FAANG Flex: The Sentinel Trick
        // We iterate up to 'n' (inclusive). When i == n, we simulate a height
        // of 0. This guarantees ALL remaining bars in the stack are popped and
        // processed, eliminating the need for a second flush loop at the end.
        for (size_t i = 0; i <= n; ++i) {
            int current_height = (i == n) ? 0 : heights[i];

            // Monotonic property: Pop if current bar is SHORTER than the top.
            while (!st.empty() && heights[st.back()] >= current_height) {
                size_t popped_idx = st.back();
                st.pop_back();

                int h = heights[popped_idx];

                // If stack is empty, the popped bar extends all the way to index 0.
                // Otherwise, it extends to the index just below it in the stack.
                size_t w = st.empty() ? i : (i - st.back() - 1);

                max_area = std::max(max_area, h * static_cast<int>(w));
            }
            st.push_back(i);
        }

        return max_area;
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

void runComparativeTest(const string& test_name, const vector<int>& heights, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: heights = " << format_vector(heights) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    int res_opt = sol_opt.largestRectangleArea(heights);
    int res_base = sol_base.largestRectangleArea(heights);

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
    cout << "--- Testing LC 84: Largest Rectangle in Histogram ---\n\n";

    try {
        // 1. Example 1: Standard fluctuations
        runComparativeTest("Example 1 (Standard Histogram)", {2, 1, 5, 6, 2, 3}, 10);

        // 2. Example 2: Simple increasing
        runComparativeTest("Example 2 (Two Elements)", {2, 4}, 4);

        // 3. Edge Case: All identical elements
        runComparativeTest("Edge Case 1 (Flat Histogram)", {5, 5, 5, 5}, 20);

        // 4. Edge Case: Strictly descending
        runComparativeTest("Edge Case 2 (Strictly Descending)", {6, 5, 4, 3, 2, 1}, 12);

        // 5. Edge Case: Valley shape
        runComparativeTest("Edge Case 3 (Valley Shape)", {5, 2, 1, 2, 5}, 5);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}