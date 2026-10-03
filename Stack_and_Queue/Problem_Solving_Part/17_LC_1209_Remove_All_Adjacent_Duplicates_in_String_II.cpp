/**
 * @file Remove_All_Adjacent_Duplicates_II.cpp
 * @brief Enterprise-grade solution for LeetCode 1209.
 * @details Compares a Baseline Stack of Pairs against the Ultimate In-Place
 *          Two-Pointer Overwrite architecture. Uses a compressed Group-Count
 *          Stack to achieve peak cache locality and O(1) string allocation.
 */

#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::pair;
using std::stack;
using std::string;
using std::vector;

// ==========================================
// Approach 1: Baseline Architecture (Explicit Stack of Pairs)
// Easy to conceptualize, but heavy on memory allocations.
// Time Complexity: O(N) | Auxiliary Space: O(N) for stack + O(N) for string
// ==========================================
class SolutionBaseline {
public:
    string removeDuplicates(const string& s, int k) const {
        stack<pair<char, int>> st;

        for (char c : s) {
            if (!st.empty() && st.top().first == c) {
                st.top().second++;
                if (st.top().second == k) {
                    st.pop();
                }
            } else {
                st.push({c, 1});
            }
        }

        string res;
        // Pre-allocate maximum possible size to avoid reallocations
        res.reserve(s.length());
        while (!st.empty()) {
            res.append(st.top().second, st.top().first);
            st.pop();
        }

        // Stack yields reversed string, must reverse it back
        std::reverse(res.begin(), res.end());
        return res;
    }
};

// ==========================================
// Approach 2: Optimized Architecture (In-Place Two-Pointer Overwrite)
// FAANG-level optimization. Eliminates auxiliary string memory via overwrite.
// Utilizes a compressed Group-Count stack for minimal memory footprint.
// Time Complexity: Strict O(N) | Space: Compressed O(N) integers only
// ==========================================
class SolutionOptimized {
public:
    // HIGHLIGHT: Pass 's' by value to perform true in-place modification
    string removeDuplicates(string s, int k) const {
        vector<int> count_stack;

        // Reserve worst-case capacity to guarantee zero dynamic reallocation
        count_stack.reserve(s.length());

        int write_idx = 0;

        for (char c : s) {
            // 1. Overwrite the character in-place
            s[write_idx] = c;

            // 2. Manage the Group-Count Stack
            if (write_idx == 0 || s[write_idx] != s[write_idx - 1]) {
                // New character group started
                count_stack.push_back(1);
            } else {
                // Increment the count of the current contiguous group
                count_stack.back()++;

                // 3. Trigger deletion if the group reaches 'k' elements
                if (count_stack.back() == k) {
                    count_stack.pop_back();  // Logically destroy the count
                    write_idx -= k;          // Logically destroy the characters
                }
            }

            write_idx++;
        }

        // Shrink the string to the final valid length in O(1) time
        s.resize(write_idx);
        return s;
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
void runComparativeTest(const string& test_name, const string& s, int k, const string& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: \"" << s << "\", k = " << k << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    string res_opt = sol_opt.removeDuplicates(s, k);
    string res_base = sol_base.removeDuplicates(s, k);

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
    cout << "--- Testing LC 1209: Remove All Adjacent Duplicates II ---\n\n";

    try {
        // 1. Example 1: Nothing to delete
        runComparativeTest("Example 1 (No Deletions)", "abcd", 2, "abcd");

        // 2. Example 2: Sequential chain reaction
        runComparativeTest("Example 2 (Chain Reaction)", "deeedbbcccbdaa", 3, "aa");

        // 3. Example 3: Complex overlapping chains
        runComparativeTest("Example 3 (Overlapping Chains)", "pbbcggttciiippooaais", 2, "ps");

        // 4. Edge Case: Complete clearance
        runComparativeTest("Edge Case 1 (Complete Clearance)", "aaaa", 4, "");

        // 5. Edge Case: Deep recursive clear
        runComparativeTest("Edge Case 2 (Deep Recursive Clear)", "abaabaaaba", 2, "");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}