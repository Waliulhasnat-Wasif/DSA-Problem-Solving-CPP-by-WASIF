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
                    st.pop_back();
                }
            } else {
                st.emplace_back(c, 1);
            }
        }

        string res;
        res.reserve(s.length());

        for (const auto& [ch, count] : st) {
            res.append(count, ch);
        }

        return res;
    }
};

class SolutionOptimized {
public:
    string removeDuplicates(string s, int k) const {
        vector<int> count_stack;

        count_stack.reserve(s.length());

        int write_idx = 0;

        for (char c : s) {
            s[write_idx] = c;

            if (write_idx == 0 || s[write_idx] != s[write_idx - 1]) {
                count_stack.push_back(1);
            } else {
                count_stack.back()++;

                if (count_stack.back() == k) {
                    count_stack.pop_back();
                    write_idx -= k;
                }
            }

            write_idx++;
        }

        s.resize(write_idx);
        return s;
    }
};

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

int main() {
    cout << "--- Testing LC 1209: Remove All Adjacent Duplicates II ---\n\n";

    try {
        runComparativeTest("Example 1 (No Deletions)", "abcd", 2, "abcd");

        runComparativeTest("Example 2 (Chain Reaction)", "deeedbbcccbdaa", 3, "aa");

        runComparativeTest("Example 3 (Overlapping Chains)", "pbbcggttciiippooaais", 2, "ps");

        runComparativeTest("Edge Case 1 (Complete Clearance)", "aaaa", 4, "");

        runComparativeTest("Edge Case 2 (Deep Recursive Clear)", "abaabaaaba", 2, "");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}