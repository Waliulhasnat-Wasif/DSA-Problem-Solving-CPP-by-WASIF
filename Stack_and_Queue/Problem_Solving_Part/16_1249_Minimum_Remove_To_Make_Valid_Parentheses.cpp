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

class SolutionBaseline {
public:
    string minRemoveToMakeValid(const string& s) const {
        size_t n = s.length();

        vector<size_t> invalid_indices;
        invalid_indices.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            if (s[i] == '(') {
                invalid_indices.push_back(i);
            } else if (s[i] == ')') {
                if (!invalid_indices.empty() && s[invalid_indices.back()] == '(') {
                    invalid_indices.pop_back();
                } else {
                    invalid_indices.push_back(i);
                }
            }
        }

        vector<bool> remove(n, false);

        for (size_t idx : invalid_indices) {
            remove[idx] = true;
        }

        string res;
        res.reserve(n - invalid_indices.size());

        for (size_t i = 0; i < n; ++i) {
            if (!remove[i]) {
                res += s[i];
            }
        }

        return res;
    }
};

class SolutionOptimized {
public:
    string minRemoveToMakeValid(string s) const {
        int open_seen = 0;
        int balance = 0;
        int write_idx = 0;

        for (size_t i = 0; i < s.length(); ++i) {
            if (s[i] == '(') {
                ++open_seen;
                ++balance;
                s[write_idx++] = s[i];
            } else if (s[i] == ')') {
                if (balance > 0) {
                    --balance;
                    s[write_idx++] = s[i];
                }
            } else {
                s[write_idx++] = s[i];
            }
        }

        s.resize(write_idx);

        if (balance > 0) {
            int keep_open = open_seen - balance;
            int current_open = 0;
            write_idx = 0;
            for (size_t i = 0; i < s.length(); ++i) {
                if (s[i] == '(') {
                    ++current_open;
                    if (current_open <= keep_open) {
                        s[write_idx++] = s[i];
                    }
                } else {
                    s[write_idx++] = s[i];
                }
            }

            s.resize(write_idx);
        }

        return s;
    }
};

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

int main() {
    cout << "--- Testing LC 1249: Min Remove to Make Valid Parentheses ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard Mix)", "lee(t(c)o)de)", "lee(t(c)o)de");

        runComparativeTest("Example 2 (Early Closing Bracket)", "a)b(c)d", "ab(c)d");

        runComparativeTest("Example 3 (Completely Inverted)", "))((", "");

        runComparativeTest("Edge Case 1 (All Opens)", "(((((", "");

        runComparativeTest("Edge Case 2 (Already Valid)", "(a(b)c)", "(a(b)c)");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}