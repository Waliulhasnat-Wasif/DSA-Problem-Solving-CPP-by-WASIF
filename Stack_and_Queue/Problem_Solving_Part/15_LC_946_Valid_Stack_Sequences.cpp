/**
 * @file Validate_Stack_Sequences.cpp
 * @brief Enterprise-grade solution for LeetCode 946.
 * @details Compares an Explicit std::stack simulation (O(N) space) against
 *          a highly Optimized In-Place Two-Pointer architecture (O(1) auxiliary
 *          space). Features safe memory manipulation and strict type safety.
 */

#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::stack;
using std::string;
using std::vector;

// Helper function to format vector output for the test engine
string format_vector(const vector<int>& v) {
    string res = "[";
    for (size_t i = 0; i < v.size(); ++i) {
        res += std::to_string(v[i]);
        if (i != v.size() - 1) {
            res += ", ";
        }
    }
    res += "]";
    return res;
}

// ==========================================
// Approach 1: Baseline Architecture (Explicit Stack Simulation)
// Clear, straightforward logical implementation using O(N) extra space.
// Time Complexity: O(N) | Space Complexity: O(N)
// ==========================================
class SolutionBaseline {
public:
    bool validateStackSequences(const vector<int>& pushed, const vector<int>& popped) const {
        stack<int> st;
        int j = 0;
        int n = static_cast<int>(popped.size());

        for (int x : pushed) {
            st.push(x);
            // Greedily pop if the top of the stack matches current popped element
            while (!st.empty() && j < n && st.top() == popped[j]) {
                st.pop();
                j++;
            }
        }

        // Valid if all elements are popped successfully
        return st.empty();
    }
};

// ==========================================
// Approach 2: Optimized Architecture (In-Place Array as Stack)
// FAANG-level optimization. Reuses the 'pushed' array as the stack itself.
// Time Complexity: Strict O(N) | Auxiliary Space: Strict O(1)
// ==========================================
class SolutionOptimized {
public:
    // HIGHLIGHT: Taking 'pushed' by reference allows true O(1) auxiliary space
    // by directly modifying the input array.
    bool validateStackSequences(vector<int>& pushed, const vector<int>& popped) const {
        int i = 0;  // Stack pointer representing the 'top' of our simulated stack
        int j = 0;  // Pointer for the 'popped' array
        int n = static_cast<int>(pushed.size());

        for (int x : pushed) {
            // Push operation: Overwrite the array at the stack pointer
            pushed[i] = x;

            // Greedily pop operation: Move the stack pointer backwards
            while (i >= 0 && j < n && pushed[i] == popped[j]) {
                i--;
                j++;
            }

            // Increment stack pointer for the next incoming element
            i++;
        }

        // Valid if the simulated stack pointer is back to 0 (empty)
        return i == 0;
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
void runComparativeTest(const string& test_name, const vector<int>& pushed, const vector<int>& popped, bool expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Pushed: " << format_vector(pushed) << " | Popped: " << format_vector(popped) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    // We pass a copy to the optimized solution to protect the baseline's data
    vector<int> pushed_copy = pushed;
    bool res_opt = sol_opt.validateStackSequences(pushed_copy, popped);
    bool res_base = sol_base.validateStackSequences(pushed, popped);

    cout << "Expected:   " << (expected ? "true" : "false") << "\n";
    cout << "Optimized:  " << (res_opt ? "true" : "false") << "\n";
    cout << "Baseline:   " << (res_base ? "true" : "false") << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LC 946: Validate Stack Sequences ---\n\n";

    try {
        // 1. Example 1: Valid sequence
        runComparativeTest("Example 1 (Valid Sequence)", {1, 2, 3, 4, 5}, {4, 5, 3, 2, 1}, true);

        // 2. Example 2: Invalid sequence (Early pop attempt)
        runComparativeTest("Example 2 (Invalid Sequence)", {1, 2, 3, 4, 5}, {4, 3, 5, 1, 2}, false);

        // 3. Edge Case: Single element match
        runComparativeTest("Edge Case 1 (Single Element - Match)", {0}, {0}, true);

        // 4. Edge Case: Single element mismatch
        runComparativeTest("Edge Case 2 (Single Element - Mismatch)", {0}, {1}, false);

        // 5. Edge Case: Immediate push-pop pattern (Stairs pattern)
        runComparativeTest("Edge Case 3 (Immediate Push-Pop)", {1, 2, 3}, {1, 2, 3}, true);

        // 6. Edge Case: Full push then full pop (Reverse pattern)
        runComparativeTest("Edge Case 4 (Reverse Pop)", {1, 2, 3}, {3, 2, 1}, true);

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}