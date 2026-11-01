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
    vector<int> canSeePersonsCount(const vector<int>& heights) const {
        size_t n = heights.size();
        vector<int> res(n, 0);

        for (size_t i = 0; i < n; ++i) {
            int max_seen_so_far = 0;
            int visible_count = 0;

            for (size_t j = i + 1; j < n; ++j) {
                if (heights[j] > max_seen_so_far) {
                    visible_count++;
                    max_seen_so_far = heights[j];
                }
                if (heights[j] >= heights[i]) {
                    break;
                }
            }
            res[i] = visible_count;
        }
        return res;
    }
};

class SolutionExtreme {
public:
    vector<int> canSeePersonsCount(const vector<int>& heights) const {
        size_t n = heights.size();
        vector<int> res(n, 0);

        // Stack stores INDICES, maintaining a strictly decreasing order of heights
        vector<size_t> st;
        st.reserve(n);

        // FAANG Flex: The safest unsigned reverse loop in C++
        // `i-- > 0` checks if i > 0, then decrements it.
        // Starts at n-1 and perfectly safely terminates at 0.
        for (size_t i = n; i-- > 0;) {
            int visible_count = 0;

            // Pop all strictly shorter people to our right.
            // We can see them, but they won't block the view of taller people behind.
            while (!st.empty() && heights[st.back()] < heights[i]) {
                st.pop_back();
                ++visible_count;
            }

            // If stack not empty, the person at the top is TALLER than us.
            // We can see them, but our view is immediately blocked by them.
            if (!st.empty()) {
                visible_count++;
            }

            res[i] = visible_count;
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

void runComparativeTest(const string& test_name, const vector<int>& heights, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: heights = " << format_vector(heights) << "\n";

    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    vector<int> res_ext = sol_ext.canSeePersonsCount(heights);
    vector<int> res_base = sol_base.canSeePersonsCount(heights);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Extreme:   " << format_vector(res_ext) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    bool passed = (res_ext == expected && res_base == expected);
    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 1944: Visible People in a Queue (Extreme) ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard Queue)", {10, 6, 8, 5, 11, 9}, {3, 1, 2, 1, 1, 0});

        runComparativeTest("Example 2 (Ascending then Peak)", {5, 1, 2, 3, 10}, {4, 1, 1, 1, 0});

        runComparativeTest("Edge Case 1 (Strictly Descending)", {5, 4, 3, 2, 1}, {1, 1, 1, 1, 0});

        runComparativeTest("Edge Case 2 (Strictly Ascending)", {1, 2, 3, 4, 5}, {1, 1, 1, 1, 0});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}