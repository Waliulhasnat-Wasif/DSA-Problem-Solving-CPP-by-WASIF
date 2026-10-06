#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::pair;
using std::stack;
using std::string;
using std::vector;

class SolutionBaseline {
public:
    string removeDuplicates(const string& s, int k) const {
        vector<pair<char, int>> st;
        st.reserve(s.length());

        for (char c : s) {
            if (!st.empty() && st.back().first == c) {
                ++st.back().second;
                if (st.back().second == k) {
                    st.pop_back();  // Group reached size 'k', destroy it
                }
            } else {
                // HIGHLIGHT: emplace_back constructs the pair directly inside the
                // vector's memory, avoiding temporary object creation overhead.
                st.emplace_back(c, 1);
            }
        }

        string res;
        res.reserve(s.length());

        // HIGHLIGHT: C++17 Structured Binding for ultimate readability.
        // We iterate forward natively, completely bypassing std::reverse.
        for (const auto& [ch, count] : st) {
            res.append(count, ch);
        }

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