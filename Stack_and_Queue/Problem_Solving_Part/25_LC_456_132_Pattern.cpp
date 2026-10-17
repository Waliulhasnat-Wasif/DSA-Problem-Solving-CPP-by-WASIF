#include <algorithm>
#include <iostream>
#include <limits>
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
    bool find132pattern(const vector<int>& nums) const {
        size_t n = nums.size();
        if (n < 3) {
            return false;
        }

        int min_i = nums[0];

        for (size_t j = 1; j < n - 1; ++j) {
            min_i = std::min(min_i, nums[j - 1]);

            for (size_t k = j + 1; k < n; ++k) {
                if (nums[k] > min_i && nums[k] < nums[j]) {
                    return true;
                }
            }
        }
        return false;
    }
};

class SolutionOptimized {
public:
    bool find132pattern(const vector<int>& nums) const {
        size_t n = nums.size();
        if (n < 3) {
            return false;
        }

        vector<int> st;
        st.reserve(n);

        int s3 = std::numeric_limits<int>::min();

        for (int i = static_cast<int>(n) - 1; i >= 0; --i) {
            int current_num = nums[i];

            if (current_num < s3) {
                return true;
            }

            while (!st.empty() && st.back() < current_num) {
                s3 = st.back();
                st.pop_back();
            }

            st.push_back(current_num);
        }

        return false;
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

void runComparativeTest(const string& test_name, const vector<int>& nums, bool expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: nums = " << format_vector(nums) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    bool res_opt = sol_opt.find132pattern(nums);
    bool res_base = sol_base.find132pattern(nums);

    cout << "Expected:  " << (expected ? "true" : "false") << "\n";
    cout << "Optimized: " << (res_opt ? "true" : "false") << "\n";
    cout << "Baseline:  " << (res_base ? "true" : "false") << "\n";

    cout << "Result:    " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 456: 132 Pattern (Interview Mode) ---\n\n";

    try {
        runComparativeTest("Example 1 (No Pattern)", {1, 2, 3, 4}, false);

        runComparativeTest("Example 2 (Classic Pattern)", {3, 1, 4, 2}, true);

        runComparativeTest("Example 3 (Negatives included)", {-1, 3, 2, 0}, true);

        runComparativeTest("Edge Case 1 (Valley Shape)", {5, 4, 3, 2, 1}, false);

        runComparativeTest("Edge Case 2 (Duplicates)", {1, 0, 1, -4, -3}, false);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}