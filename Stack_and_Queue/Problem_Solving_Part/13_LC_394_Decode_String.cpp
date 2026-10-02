#include <cctype>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::isdigit;
using std::stack;
using std::string;
using std::underflow_error;

class SolutionBaseline {
public:
    string decodeString(const string& s) const {
        int index = 0;
        return decodeRecursive(s, index);
    }

private:
    string decodeRecursive(const string& s, int& index) const {
        string res;
        int k = 0;
        int n = static_cast<int>(s.length());

        while (index < n) {
            char c = s[index];
            if (isdigit(c)) {
                k = k * 10 + (c - '0');
                index++;
            } else if (c == '[') {
                index++;  // Skip '['
                // Recursive context switch
                string decoded_inner = decodeRecursive(s, index);
                while (k > 0) {
                    res += decoded_inner;
                    k--;
                }
            } else if (c == ']') {
                index++;  // Skip ']' and return to outer context
                return res;
            } else {
                res += c;
                index++;
            }
        }
        return res;
    }
};

// ==========================================
// Approach 2: Optimized Architecture (Iterative Dual-Stack)
// FAANG-level robust parsing using explicit Heap Stacks.
// Features std::move for zero-cost ownership transfer of string contexts.
// Time Complexity: O(N * maxK) | Space Complexity: O(N) Heap Space
// ==========================================
class SolutionOptimized {
public:
    string decodeString(const string& s) const {
        stack<int> count_stack;
        stack<string> string_stack;

        string current_string = "";
        int k = 0;
        int n = static_cast<int>(s.length());

        for (int i = 0; i < n; ++i) {
            char c = s[i];

            if (isdigit(c)) {
                // Build the multiplier (handles multi-digit numbers like "100[a]")
                k = k * 10 + (c - '0');
            } else if (c == '[') {
                // Context Switch: Save current state to stacks
                count_stack.push(k);

                // HIGHLIGHT: std::move transfers ownership without O(M) copying cost
                string_stack.push(std::move(current_string));

                // Reset for the new inner context
                current_string = "";
                k = 0;
            } else if (c == ']') {
                // Exception safety guard against malformed strings
                if (string_stack.empty() || count_stack.empty()) {
                    throw underflow_error("Error: Malformed encoded string.");
                }

                // Context Resolve: Retrieve previous state
                string prev_string = std::move(string_stack.top());
                string_stack.pop();

                int repeat_times = count_stack.top();
                count_stack.pop();

                // Multiply current context and append to previous context
                size_t total_size = prev_string.size() + (current_string.size() * repeat_times);
                prev_string.reserve(total_size);

                for (int j = 0; j < repeat_times; ++j) {
                    prev_string += current_string;
                }

                // Update current pointer via move semantics
                current_string = std::move(prev_string);
            } else {
                current_string += c;
            }
        }

        return current_string;
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
void runComparativeTest(const string& test_name, const string& s, const string& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Encoded String: \"" << s << "\"\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    string res_opt = sol_opt.decodeString(s);
    string res_base = sol_base.decodeString(s);

    cout << "Expected:   \"" << expected << "\"\n";
    cout << "Optimized:  \"" << res_opt << "\"\n";
    cout << "Baseline:   \"" << res_base << "\"\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LeetCode 394: Decode String ---\n\n";

    try {
        // 1. Example 1: Basic Sequential Decoding
        runComparativeTest("Example 1 (Sequential)", "3[a]2[bc]", "aaabcbc");

        // 2. Example 2: Nested Decoding
        runComparativeTest("Example 2 (Nested)", "3[a2[c]]", "accaccacc");

        // 3. Example 3: Mixed Plain text and Encoded
        runComparativeTest("Example 3 (Mixed Plain)", "2[abc]3[cd]ef", "abcabccdcdcdef");

        // 4. Edge Case: Multi-digit multiplier
        runComparativeTest("Edge Case 1 (Multi-digit)", "10[a]", "aaaaaaaaaa");

        // 5. Edge Case: Deeply Nested
        runComparativeTest("Edge Case 2 (Deeply Nested)", "2[2[2[b]]]", "bbbbbbbb");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}