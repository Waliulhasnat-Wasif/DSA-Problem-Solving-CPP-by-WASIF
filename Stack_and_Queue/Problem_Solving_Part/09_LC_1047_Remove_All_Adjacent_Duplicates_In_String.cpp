#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::stack;
using std::string;

// ==========================================
// Approach 1: Baseline Architecture (Explicit Stack)
// Clear LIFO logic, but requires an O(N) reversal at the end.
// Time Complexity: O(N) | Space Complexity: O(N) auxiliary
// ==========================================
class SolutionStack {
public:
    string removeDuplicates(const string& s) const {
        stack<char> st;

        for (char c : s) {
            if (!st.empty() && st.top() == c) {
                st.pop();  // Duplicate found, destroy both
            } else {
                st.push(c);  // Safe to push
            }
        }

        // Reconstruct string from stack
        string result = "";
        while (!st.empty()) {
            result += st.top();
            st.pop();
        }

        // Stack yields reversed string, so we must reverse it back
        std::reverse(result.begin(), result.end());

        return result;
    }
};

// ==========================================
// Approach 2: Optimized Architecture (String as Stack)
// Output string acts as the stack. Eliminates the need for std::reverse.
// Time: Strict O(N) | Space: O(1) auxiliary (excluding output)
// ==========================================
class SolutionOptimized {
public:
    string removeDuplicates(const string& s) const {
        string result;

        // HIGHLIGHT: Pre-allocate memory to prevent costly dynamic reallocations
        // during push_back(). This is a core C++ performance optimization.
        result.reserve(s.size());

        for (char c : s) {
            // result.back() acts precisely like st.top()
            if (!result.empty() && result.back() == c) {
                // Duplicate found, pop from our "string stack"
                result.pop_back();
            } else {
                result.push_back(c);
            }
        }

        return result;  // Already in the correct order!
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
void runComparativeTest(const string& test_name, const string& input, const string& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: \"" << input << "\"\n";

    SolutionOptimized sol_opt;
    SolutionStack sol_base;

    string res_opt = sol_opt.removeDuplicates(input);
    string res_base = sol_base.removeDuplicates(input);

    cout << "Expected: \"" << expected << "\"\n";
    cout << "Optimized Output: \"" << res_opt << "\"\n";
    cout << "Baseline Output:  \"" << res_base << "\"\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LC 1047: Remove All Adjacent Duplicates ---\n\n";

    try {
        // 1. Example 1: Chain reaction resolving
        runComparativeTest("Example 1 (Chain Reaction)", "abbaca", "ca");

        // 2. Example 2: Multiple identical pairs
        runComparativeTest("Example 2 (Multiple Pairs)", "azxxzy", "ay");

        // 3. Edge Case: Complete annihilation
        runComparativeTest("Edge Case 1 (Complete Annihilation)", "abba", "");

        // 4. Edge Case: No duplicates
        runComparativeTest("Edge Case 2 (No Duplicates)", "abcdef", "abcdef");

        // 5. Edge Case: Long chain of same characters
        runComparativeTest("Edge Case 3 (All Same Characters)", "aaaaaa", "");

        // 6. Edge Case: Odd length of same characters
        runComparativeTest("Edge Case 4 (Odd Length Same)", "aaaaaaa", "a");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}