#include <iostream>
#include <stack>
#include <string>

using std::cout;
using std::stack;
using std::string;

class Solution {
public:
    bool isValid(const string& s) {
        if (s.length() % 2 != 0) {
            return false;
        }

        stack<char> st;

        for (char ch : s) {
            if (ch == '(') {
                st.push(')');
            } else if (ch == '{') {
                st.push('}');
            } else if (ch == '[') {
                st.push(']');
            } else {
                if (st.empty() || st.top() != ch) {
                    return false;
                }
                st.pop();
            }
        }

        return st.empty();
    }
};

void runTestCase(const string& input, bool expected, const string& test_name) {
    Solution sol;
    cout << "Test Case: " << test_name << "\n";
    cout << "Input    : \"" << input << "\"\n";

    bool result = sol.isValid(input);

    cout << "Output   : " << (result ? "true" : "false") << "\n";
    cout << "Expected : " << (expected ? "true" : "false") << "\n";

    if (result == expected) {
        cout << "Status   : [PASS]\n";
    } else {
        cout << "Status   : [FAIL]\n";
    }

    cout << string(50, '-') << "\n";
}

int main() {
    cout << "--- Testing LeetCode 20: Valid Parentheses ---\n\n";

    runTestCase("()", true, "Example 1 (Simple pair)");
    runTestCase("()[]{}", true, "Example 2 (Sequential pairs)");
    runTestCase("(]", false, "Example 3 (Type mismatch)");
    runTestCase("([])", true, "Example 4 (Nested valid pairs)");
    runTestCase("([)]", false, "Example 5 (Interleaved invalid pairs)");

    runTestCase("]", false, "Edge Case 1 (Single closing bracket)");
    runTestCase("[", false, "Edge Case 2 (Single opening bracket)");
    runTestCase("((((((((", false, "Edge Case 3 (All opening brackets)");
    runTestCase("", true, "Edge Case 4 (Empty string)");

    return 0;
}