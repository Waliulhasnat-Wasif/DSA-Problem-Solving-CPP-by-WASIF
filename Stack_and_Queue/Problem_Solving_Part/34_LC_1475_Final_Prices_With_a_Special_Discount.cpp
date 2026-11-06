/**
 * @file Final_Prices_With_Discount.cpp
 * @brief FAANG Interview-optimized combined solution for LeetCode 1475.
 * @details Features 2 architectures:
 *          1. O(N^2) Baseline Brute-Force.
 *          2. O(N) Extreme Monotonic Stack (Left-to-Right).
 *          Strictly utilizes index tracking to maintain the gold standard.
 */

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
// Approach 1: Baseline Architecture (Brute-Force)
// Scans ahead for every item to find the first valid discount.
// Time Complexity: O(N^2) | Space Complexity: O(1) (excluding output)
// ==========================================
class SolutionBaseline {
public:
    vector<int> finalPrices(const vector<int>& prices) const {
        size_t n = prices.size();
        vector<int> res = prices;  // Copy original prices

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                if (prices[j] <= prices[i]) {
                    res[i] -= prices[j];  // Apply discount
                    break;                // Only the first valid discount applies
                }
            }
        }
        return res;
    }
};

// ==========================================
// Approach 2: Ultra-Optimized Architecture (Monotonic Stack)
// FAANG-level execution. Finds Next Smaller or Equal Element (NSEE) in O(N).
// Time Complexity: Strict O(N) | Space Complexity: O(N)
// ==========================================
class SolutionExtreme {
public:
    vector<int> finalPrices(const vector<int>& prices) const {
        size_t n = prices.size();
        vector<int> res = prices;  // Initialize result with original prices

        // Stack stores INDICES of items waiting for a discount
        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            // If current price is less than or equal to the price of the item
            // at the top of the stack, it acts as a discount for that item!
            while (!st.empty() && prices[i] <= prices[st.back()]) {
                size_t waiting_idx = st.back();
                st.pop_back();

                // Apply the discount to the waiting item
                res[waiting_idx] -= prices[i];
            }

            // Current item now waits for its own discount in the future
            st.push_back(i);
        }

        // Any indices left in the stack will never find a valid discount.
        // Since we initialized `res` with original prices, we need not do anything.

        return res;
    }
};

// ==========================================
// Test Execution Engine
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

void runComparativeTest(const string& test_name, const vector<int>& prices, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: prices = " << format_vector(prices) << "\n";

    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    vector<int> res_ext = sol_ext.finalPrices(prices);
    vector<int> res_base = sol_base.finalPrices(prices);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Extreme:   " << format_vector(res_ext) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    bool passed = (res_ext == expected && res_base == expected);
    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 1475: Final Prices With Discount ---\n\n";

    try {
        runComparativeTest("Example 1 (Mixed Discounts)", {8, 4, 6, 2, 3}, {4, 2, 4, 2, 3});

        runComparativeTest("Example 2 (Strictly Ascending - No Discounts)", {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5});

        runComparativeTest("Example 3 (Mixed with Equals)", {10, 1, 1, 6}, {9, 0, 1, 6});

        runComparativeTest("Edge Case (All Identical)", {5, 5, 5, 5}, {0, 0, 0, 5});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}