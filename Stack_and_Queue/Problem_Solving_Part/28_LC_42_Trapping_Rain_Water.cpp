#include <algorithm>
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
    int trap(const vector<int>& height) const {
        size_t n = height.size();
        if (n < 3) return 0;

        int total_water = 0;
        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            while (!st.empty() && height[st.back()] < height[i]) {
                size_t valley_idx = st.back();
                st.pop_back();

                if (st.empty()) break;

                size_t left_bound_idx = st.back();
                size_t width = i - left_bound_idx - 1;
                int bounded_height = std::min(height[i], height[left_bound_idx]) - height[valley_idx];

                total_water += bounded_height * static_cast<int>(width);
            }
            st.push_back(i);
        }

        return total_water;
    }
};

class SolutionOptimized {
public:
    int trap(const vector<int>& height) const {
        if (height.empty()) return 0;

        size_t left = 0;
        size_t right = height.size() - 1;

        int left_max = 0;
        int right_max = 0;
        int total_water = 0;

        while (left < right) {
            if (height[left] < height[right]) {
                if (height[left] >= left_max) {
                    left_max = height[left];
                } else {
                    total_water += left_max - height[left];
                }
                ++left;
            } else {
                if (height[right] >= right_max) {
                    right_max = height[right];
                } else {
                    total_water += right_max - height[right];
                }
                --right;
            }
        }

        return total_water;
    }
};

string format_vector(const vector<int>& v) {
    if (v.empty()) return "[]";
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

void runComparativeTest(const string& test_name, const vector<int>& height, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: height = " << format_vector(height) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    int res_opt = sol_opt.trap(height);
    int res_base = sol_base.trap(height);

    cout << "Expected:  " << expected << "\n";
    cout << "Optimized: " << res_opt << "\n";
    cout << "Baseline:  " << res_base << "\n";

    cout << "Result:    " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 42: Trapping Rain Water ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard Map)", {0, 1, 0, 2, 1, 0, 1, 3, 2, 1, 2, 1}, 6);

        runComparativeTest("Example 2 (Deep Valley)", {4, 2, 0, 3, 2, 5}, 9);

        runComparativeTest("Edge Case 1 (Strictly Ascending)", {1, 2, 3, 4, 5}, 0);

        runComparativeTest("Edge Case 2 (Pyramid Shape)", {1, 3, 5, 3, 1}, 0);

        runComparativeTest("Edge Case 3 (Bowl Shape)", {5, 1, 1, 1, 5}, 12);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}