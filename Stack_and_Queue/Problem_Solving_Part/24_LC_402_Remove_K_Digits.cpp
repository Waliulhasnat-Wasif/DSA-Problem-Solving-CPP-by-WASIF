#include <iostream>
#include <stdexcept>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::string;

class SolutionBaseline {
public:
    string removeKdigits(string num, int k) const {
        if (k >= static_cast<int>(num.length())) {
            return "0";
        }

        while (k > 0) {
            size_t n = num.length();
            size_t i = 0;

            while (i < n - 1 && num[i] <= num[i + 1]) {
                ++i;
            }

            num.erase(i, 1);
            --k;
        }

        size_t start = 0;
        while (start < num.length() && num[start] == '0') {
            ++start;
        }

        string res = num.substr(start);
        return res.empty() ? "0" : res;
    }
};

class SolutionOptimized {
public:
    string removeKdigits(const string& num, int k) const {
        if (k >= static_cast<int>(num.length())) {
            return "0";
        }

        string st;
        st.reserve(num.length());

        for (char digit : num) {
            while (k > 0 && !st.empty() && st.back() > digit) {
                st.pop_back();
                k--;
            }

            if (!st.empty() || digit != '0') {
                st.push_back(digit);
            }
        }

        while (k > 0 && !st.empty()) {
            st.pop_back();
            k--;
        }

        return st.empty() ? "0" : st;
    }
};

void runComparativeTest(const string& test_name, const string& num, int k, const string& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: num = \"" << num << "\", k = " << k << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    string res_opt = sol_opt.removeKdigits(num, k);
    string res_base = sol_base.removeKdigits(num, k);

    cout << "Expected:  \"" << expected << "\"\n";
    cout << "Optimized: \"" << res_opt << "\"\n";
    cout << "Baseline:  \"" << res_base << "\"\n";

    cout << "Result:    " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 402: Remove K Digits (Interview Mode) ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard Fluctuations)", "1432219", 3, "1219");

        runComparativeTest("Example 2 (Leading Zeros)", "10200", 1, "200");

        runComparativeTest("Example 3 (Total Wipeout)", "10", 2, "0");

        runComparativeTest("Edge Case 1 (Strictly Increasing)", "12345", 2, "123");

        runComparativeTest("Edge Case 2 (Identical Digits + Zeros)", "112000", 2, "1000");

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}