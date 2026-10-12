#include <iostream>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class SolutionBaseline {
public:
    vector<int> nextGreaterElements(const vector<int>& nums) const {
        int n = static_cast<int>(nums.size());
        vector<int> res(n, -1);

        for (int i = 0; i < n; ++i) {
            for (int j = 1; j < n; ++j) {
                int circular_idx = (i + j) % n;
                if (nums[circular_idx] > nums[i]) {
                    res[i] = nums[circular_idx];
                    break;
                }
            }
        }

        return res;
    }
};

class SolutionOptimized {
public:
    vector<int> nextGreaterElements(const vector<int>& nums) const {
        int n = static_cast<int>(nums.size());
        vector<int> res(n, -1);

        vector<int> st;
        st.reserve(n);

        for (int i = 0; i < n; ++i) {
            while (!st.empty() && nums[st.back()] < nums[i]) {
                res[st.back()] = nums[i];
                st.pop_back();
            }
            st.push_back(i);
        }

        for (int i = 0; i < n - 1; ++i) {
            if (st.empty()) {
                break;
            }
            while (!st.empty() && nums[st.back()] < nums[i]) {
                res[st.back()] = nums[i];
                st.pop_back();
            }
        }

        return res;
    }
};

class SolutionOptimized_V2 {
public:
    vector<int> nextGreaterElements(const vector<int>& nums) const {
        int n = static_cast<int>(nums.size());
        vector<int> res(n, -1);

        vector<int> st;
        st.reserve(n);

        for (int i = 0; i < 2 * n; ++i) {
            int circular_idx = i % n;
            while (!st.empty() && nums[st.back()] < nums[circular_idx]) {
                res[st.back()] = nums[circular_idx];
                st.pop_back();
            }
            if (i < n) {
                st.push_back(circular_idx);
            }
        }

        return res;
    }
};

class SolutionOptimized_V3 {
public:
    vector<int> nextGreaterElements(const vector<int>& nums) const {
        int n = static_cast<int>(nums.size());
        vector<int> res(n, -1);

        vector<int> st;
        st.reserve(n);

        for (int i = 2 * n - 1; i >= 0; --i) {
            int circular_idx = i % n;

            // আমার চেয়ে ছোট বা সমানদের পপ করে দাও
            while (!st.empty() && nums[st.back()] <= nums[circular_idx]) {
                st.pop_back();
            }

            // ১ম চক্করে উত্তর স্টোর করো (টপে থাকা উপাদানই আমার উত্তর)
            if (i < n) {
                res[circular_idx] = st.empty() ? -1 : nums[st.back()];
            }

            st.push_back(circular_idx);
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

void runComparativeTest(const string& test_name, const vector<int>& nums, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: " << format_vector(nums) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    vector<int> res_opt = sol_opt.nextGreaterElements(nums);
    vector<int> res_base = sol_base.nextGreaterElements(nums);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Optimized: " << format_vector(res_opt) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 503: Next Greater Element II (Interview Mode) ---\n\n";

    try {
        runComparativeTest("Example 1 (Basic Circular)", {1, 2, 1}, {2, -1, 2});

        runComparativeTest("Example 2 (Extended Circular)", {1, 2, 3, 4, 3}, {2, 3, 4, -1, 4});

        runComparativeTest("Edge Case 1 (All Identical)", {5, 5, 5, 5}, {-1, -1, -1, -1});

        runComparativeTest("Edge Case 2 (Strictly Decreasing)", {5, 4, 3, 2, 1}, {-1, 5, 5, 5, 5});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}