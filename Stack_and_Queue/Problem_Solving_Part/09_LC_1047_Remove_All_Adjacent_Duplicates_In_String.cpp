#include <algorithm>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::stack;
using std::string;

class SolutionStack {
public:
    string removeDuplicates(const string& s) const {
        stack<char> st;

        for (char c : s) {
            if (!st.empty() && st.top() == c) {
                st.pop();
            } else {
                st.push(c);
            }
        }

        string result = "";
        while (!st.empty()) {
            result += st.top();
            st.pop();
        }

        std::reverse(result.begin(), result.end());

        return result;
    }
};

class SolutionOptimized {
public:
    string removeDuplicates(const string& s) const {
        string result;

        result.reserve(s.size());

        for (char c : s) {
            if (!result.empty() && result.back() == c) {
                result.pop_back();
            } else {
                result.push_back(c);
            }
        }

        return result;
    }
};

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

int main() {
    cout << "--- Testing LC 1047: Remove All Adjacent Duplicates ---\n\n";

    try {
        runComparativeTest("Example 1 (Chain Reaction)", "abbaca", "ca");

        runComparativeTest("Example 2 (Multiple Pairs)", "azxxzy", "ay");

        runComparativeTest("Edge Case 1 (Complete Annihilation)", "abba", "");

        runComparativeTest("Edge Case 2 (No Duplicates)", "abcdef", "abcdef");

        runComparativeTest("Edge Case 3 (All Same Characters)", "aaaaaa", "");

        runComparativeTest("Edge Case 4 (Odd Length Same)", "aaaaaaa", "a");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}