/**
 * @file Online_Stock_Span.cpp
 * @brief FAANG Interview-optimized solution for LeetCode 901.
 * @details Uses a vector-backed Monotonic Decreasing Stack storing
 *          {price, span} pairs. Achieves amortized O(1) time per next()
 *          call with strict memory efficiency and zero temporary objects.
 */

#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::vector;

// ==========================================
// Interview-Optimized Architecture (Monotonic Stack of Pairs)
// FAANG-level robust execution.
// Time Complexity: Amortized O(1) per call | Space Complexity: O(N)
// ==========================================
class StockSpanner {
public:
    StockSpanner() {
        // FAANG Flex: Reserve memory to prevent dynamic reallocations.
        // The problem constraints guarantee at most 10^4 calls.
        st.reserve(10000);
    }

    int next(int price) {
        int current_span = 1;

        // Pop while the top price is less than or equal to the current price.
        // Accumulate their spans because they represent consecutive smaller days.
        while (!st.empty() && st.back().first <= price) {
            current_span += st.back().second;
            st.pop_back();
        }

        // FAANG Flex: emplace_back constructs the pair strictly in-place inside
        // the vector's memory, completely avoiding temporary object creation.
        st.emplace_back(price, current_span);

        return current_span;
    }

private:
    // Vector-as-Stack for contiguous memory caching.
    // Stores pairs of {price, accumulated_span}.
    vector<pair<int, int>> st;
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================

// Fully Optimized Branchless Vector Formatter
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

void runTest(const string& test_name, const vector<int>& prices, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Prices:  " << format_vector(prices) << "\n";

    StockSpanner spanner;
    vector<int> res;
    res.reserve(prices.size());

    for (int p : prices) {
        res.push_back(spanner.next(p));
    }

    cout << "Expected: " << format_vector(expected) << "\n";
    cout << "Output:   " << format_vector(res) << "\n";
    cout << "Result:   " << (res == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LC 901: Online Stock Span (Interview Mode) ---\n\n";

    try {
        // 1. Example 1: Standard fluctuations
        runTest("Example 1 (Standard Fluctuations)", {100, 80, 60, 70, 60, 75, 85}, {1, 1, 1, 2, 1, 4, 6});

        // 2. Example 2: Strict ascending (Each day spans all previous)
        runTest("Example 2 (Strict Ascending)", {10, 20, 30, 40}, {1, 2, 3, 4});

        // 3. Edge Case: Strict descending (Span is always 1)
        runTest("Edge Case 1 (Strict Descending)", {50, 40, 30, 20}, {1, 1, 1, 1});

        // 4. Edge Case: Identical prices
        runTest("Edge Case 2 (Identical Prices)", {10, 10, 10, 10}, {1, 2, 3, 4});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}