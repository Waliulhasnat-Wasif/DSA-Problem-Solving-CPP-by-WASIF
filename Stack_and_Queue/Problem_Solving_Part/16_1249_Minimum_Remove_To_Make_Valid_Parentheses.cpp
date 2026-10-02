/**
 * @file Minimum_Remove_to_Make_Valid_Parentheses.cpp
 * @brief Enterprise-grade solution for LeetCode 1249.
 * @details Compares an Explicit Index Stack baseline against an In-Place
 *          Two-Pass optimal architecture. Features true O(1) auxiliary space
 *          by utilizing the Two-Pointer Overwrite pattern and string resizing.
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

// ==========================================
// Approach 1: Baseline Architecture (Explicit Index Stack)
// Uses a stack to track invalid indices. Easy to conceptualize.
// Time Complexity: O(N) | Auxiliary Space: O(N)
// ==========================================
class SolutionBaseline {
public:
    string minRemoveToMakeValid(const string& s) const {
        int n = static_cast<int>(s.length());
        stack<int> invalid_indices;

        // Pass 1: Identify all invalid parentheses
        for (int i = 0; i < n; ++i) {
            if (s[i] == '(') {
                invalid_indices.push(i);
            } else if (s[i] == ')') {
                if (!invalid_indices.empty() && s[invalid_indices.top()] == '(') {
                    invalid_indices.pop();  // Valid pair found
                } else {
                    invalid_indices.push(i);  // Invalid ')'
                }
            }
        }

        // Convert invalid indices to a boolean array for O(1) lookup
        vector<bool> remove(n, false);
        while (!invalid_indices.empty()) {
            remove[invalid_indices.top()] = true;
            invalid_indices.pop();
        }

        // Pass 2: Reconstruct valid string
        string res;
        res.reserve(n);
        for (int i = 0; i < n; ++i) {
            if (!remove[i]) {
                res += s[i];
            }
        }

        return res;
    }
};

// ==========================================
// Approach 2: Optimized Architecture (In-Place Two-Pointer Overwrite)
// FAANG-level optimization. Eliminates O(N) auxiliary string memory.
// Time Complexity: Strict O(N) | Auxiliary Space: Strict O(1)
// ==========================================
class SolutionOptimized {
public:
    // HIGHLIGHT: Taking 's' by value allows us to modify the memory in-place,
    // achieving true O(1) extra space without mutating the caller's variable.
    string minRemoveToMakeValid(string s) const {
        int open_seen = 0;
        int balance = 0;
        int write_idx = 0;

        // First Pass (Left to Right): Remove all invalid ')' in-place
        for (size_t i = 0; i < s.length(); ++i) {
            if (s[i] == '(') {
                open_seen++;
                balance++;
                s[write_idx++] = s[i];
            } else if (s[i] == ')') {
                if (balance > 0) {
                    balance--;
                    s[write_idx++] = s[i];
                }
            } else {
                s[write_idx++] = s[i];  // Keep English letters
            }
        }

        // Shrink the string to the valid length after removing ')'
        s.resize(write_idx);

        // Second Pass (Left to Right): Remove excess '(' in-place
        if (balance > 0) {
            int keep_open = open_seen - balance;
            int current_open = 0;
            write_idx = 0;  // Reset write pointer to overwrite again

            for (size_t i = 0; i < s.length(); ++i) {
                if (s[i] == '(') {
                    current_open++;
                    if (current_open <= keep_open) {
                        s[write_idx++] = s[i];
                    }
                } else {
                    s[write_idx++] = s[i];
                }
            }

            // Shrink again for the final valid string
            s.resize(write_idx);
        }

        return s;
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
void runComparativeTest(const string& test_name, const string& s, const string& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: \"" << s << "\"\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    string res_opt = sol_opt.minRemoveToMakeValid(s);
    string res_base = sol_base.minRemoveToMakeValid(s);

    cout << "Expected:  \"" << expected << "\"\n";
    cout << "Optimized: \"" << res_opt << "\"\n";
    cout << "Baseline:  \"" << res_base << "\"\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LC 1249: Min Remove to Make Valid Parentheses ---\n\n";

    try {
        // 1. Example 1: Standard mix
        runComparativeTest("Example 1 (Standard Mix)", "lee(t(c)o)de)", "lee(t(c)o)de");

        // 2. Example 2: Extraneous closing bracket early on
        runComparativeTest("Example 2 (Early Closing Bracket)", "a)b(c)d", "ab(c)d");

        // 3. Example 3: Completely inverted
        runComparativeTest("Example 3 (Completely Inverted)", "))((", "");

        // 4. Edge Case: All opens
        runComparativeTest("Edge Case 1 (All Opens)", "(((((", "");

        // 5. Edge Case: Valid already
        runComparativeTest("Edge Case 2 (Already Valid)", "(a(b)c)", "(a(b)c)");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}