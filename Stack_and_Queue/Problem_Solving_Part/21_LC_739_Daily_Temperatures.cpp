#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class SolutionBaseline {
public:
    vector<int> dailyTemperatures(const vector<int>& temperatures) const {
        size_t n = temperatures.size();
        vector<int> res(n, 0);

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                if (temperatures[j] > temperatures[i]) {
                    res[i] = static_cast<int>(j - i);
                    break;
                }
            }
        }

        return res;
    }
};

class SolutionOptimized {
public:
    vector<int> dailyTemperatures(const vector<int>& temperatures) const {
        size_t n = temperatures.size();
        vector<int> res(n, 0);

        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            while (!st.empty() && temperatures[st.back()] < temperatures[i]) {
                size_t prev_day_idx = st.back();
                st.pop_back();

                res[prev_day_idx] = static_cast<int>(i - prev_day_idx);
            }

            st.push_back(i);
        }

        return res;
    }
};

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

void runComparativeTest(const string& test_name, const vector<int>& temps, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Temperatures: " << format_vector(temps) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    vector<int> res_opt = sol_opt.dailyTemperatures(temps);
    vector<int> res_base = sol_base.dailyTemperatures(temps);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Optimized: " << format_vector(res_opt) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 739: Daily Temperatures (Interview Mode) ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard Fluctuations)", {73, 74, 75, 71, 69, 72, 76, 73}, {1, 1, 4, 2, 1, 1, 0, 0});

        runComparativeTest("Example 2 (Strict Ascending)", {30, 40, 50, 60}, {1, 1, 1, 0});

        runComparativeTest("Edge Case 1 (Strict Descending)", {90, 80, 70, 60}, {0, 0, 0, 0});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}