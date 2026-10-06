#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::stack;
using std::string;
using std::vector;

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

class SolutionBaseline {
public:
    bool validateStackSequences(const vector<int>& pushed, const vector<int>& popped) const {
        stack<int> st;
        int j = 0;
        int n = static_cast<int>(popped.size());

        for (int x : pushed) {
            st.push(x);
            while (!st.empty() && j < n && st.top() == popped[j]) {
                st.pop();
                ++j;
            }
        }

        return st.empty();
    }
};

class SolutionOptimized {
public:
    bool validateStackSequences(vector<int>& pushed, const vector<int>& popped) const {
        int i = 0;
        int j = 0;
        int n = static_cast<int>(pushed.size());

        for (int x : pushed) {
            pushed[i] = x;
            while (i >= 0 && j < n && pushed[i] == popped[j]) {
                --i;
                ++j;
            }
            ++i;
        }

        return i == 0;
    }
};

void runComparativeTest(const string& test_name, const vector<int>& pushed, const vector<int>& popped, bool expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Pushed: " << format_vector(pushed) << " | Popped: " << format_vector(popped) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    vector<int> pushed_copy = pushed;
    bool res_opt = sol_opt.validateStackSequences(pushed_copy, popped);
    bool res_base = sol_base.validateStackSequences(pushed, popped);

    cout << "Expected:   " << (expected ? "true" : "false") << "\n";
    cout << "Optimized:  " << (res_opt ? "true" : "false") << "\n";
    cout << "Baseline:   " << (res_base ? "true" : "false") << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 946: Validate Stack Sequences ---\n\n";

    try {
        runComparativeTest("Example 1 (Valid Sequence)", {1, 2, 3, 4, 5}, {4, 5, 3, 2, 1}, true);

        runComparativeTest("Example 2 (Invalid Sequence)", {1, 2, 3, 4, 5}, {4, 3, 5, 1, 2}, false);

        runComparativeTest("Edge Case 1 (Single Element - Match)", {0}, {0}, true);

        runComparativeTest("Edge Case 2 (Single Element - Mismatch)", {0}, {1}, false);

        runComparativeTest("Edge Case 3 (Immediate Push-Pop)", {1, 2, 3}, {1, 2, 3}, true);

        runComparativeTest("Edge Case 4 (Reverse Pop)", {1, 2, 3}, {3, 2, 1}, true);

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}