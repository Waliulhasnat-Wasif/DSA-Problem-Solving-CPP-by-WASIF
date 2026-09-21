#include <iostream>
#include <stdexcept>
#include <string>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;

class SolutionBaseline {
public:
    bool backspaceCompare(const string& s, const string& t) const {
        return buildString(s) == buildString(t);
    }

private:
    string buildString(const string& str) const {
        string result;
        result.reserve(str.length());

        for (char c : str) {
            if (c == '#') {
                if (!result.empty()) {
                    result.pop_back();
                }
            } else {
                result.push_back(c);
            }
        }
        return result;
    }
};

class SolutionOptimized {
public:
    bool backspaceCompare(const string& s, const string& t) const {
        int i = static_cast<int>(s.length()) - 1;
        int j = static_cast<int>(t.length()) - 1;

        int skip_s = 0;
        int skip_t = 0;

        while (i >= 0 || j >= 0) {
            while (i >= 0) {
                if (s[i] == '#') {
                    skip_s++;
                    i--;
                } else if (skip_s > 0) {
                    skip_s--;
                    i--;
                } else {
                    break;
                }
            }

            while (j >= 0) {
                if (t[j] == '#') {
                    skip_t++;
                    j--;
                } else if (skip_t > 0) {
                    skip_t--;
                    j--;
                } else {
                    break;
                }
            }

            if (i >= 0 && j >= 0 && s[i] != t[j]) {
                return false;
            }

            if ((i >= 0) != (j >= 0)) {
                return false;
            }

            i--;
            j--;
        }

        return true;
    }
};

void runComparativeTest(const string& test_name, const string& s, const string& t, bool expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "String s: \"" << s << "\" | String t: \"" << t << "\"\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    bool res_opt = sol_opt.backspaceCompare(s, t);
    bool res_base = sol_base.backspaceCompare(s, t);

    cout << "Expected: " << (expected ? "true" : "false") << "\n";
    cout << "Optimized Output: " << (res_opt ? "true" : "false") << "\n";
    cout << "Baseline Output:  " << (res_base ? "true" : "false") << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 844: Backspace String Compare ---\n\n";

    try {
        runComparativeTest("Example 1 (Basic Backspace)", "ab#c", "ad#c", true);

        runComparativeTest("Example 2 (Complete Clearance)", "ab##", "c#d#", true);

        runComparativeTest("Example 3 (Different Outputs)", "a#c", "b", false);

        runComparativeTest("Edge Case 1 (Backspace on Empty)", "a##c", "#a#c", true);

        runComparativeTest("Edge Case 2 (Uneven Lengths)", "bxj##tw", "bxo#j##tw", true);

        runComparativeTest("Edge Case 3 (Empty vs Non-Empty)", "a", "a#", false);

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}