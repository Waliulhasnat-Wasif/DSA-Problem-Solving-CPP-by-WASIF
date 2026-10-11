#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
using std::vector;

class SolutionBaseline {
public:
    vector<int> nextGreaterElement(const vector<int>& nums1, const vector<int>& nums2) const {
        vector<int> res;
        res.reserve(nums1.size());

        size_t n = nums2.size();

        for (int x : nums1) {
            bool found = false;
            int next_greater = -1;

            for (size_t j = 0; j < n; ++j) {
                if (nums2[j] == x) {
                    found = true;
                }
                if (found && nums2[j] > x) {
                    next_greater = nums2[j];
                    break;
                }
            }
            res.push_back(next_greater);
        }

        return res;
    }
};

class SolutionOptimized {
public:
    vector<int> nextGreaterElement(const vector<int>& nums1, const vector<int>& nums2) const {
        unordered_map<int, int> nge_map;
        nge_map.reserve(nums2.size());

        vector<int> st;
        st.reserve(nums2.size());

        for (int num : nums2) {
            while (!st.empty() && st.back() < num) {
                nge_map[st.back()] = num;
                st.pop_back();
            }
            st.push_back(num);
        }

        vector<int> res;
        res.reserve(nums1.size());

        for (int x : nums1) {
            auto it = nge_map.find(x);
            if (it != nge_map.end()) {
                res.push_back(it->second);
            } else {
                res.push_back(-1);
            }
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

void runComparativeTest(const string& test_name, const vector<int>& nums1, const vector<int>& nums2, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "nums1: " << format_vector(nums1) << "\n";
    cout << "nums2: " << format_vector(nums2) << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    vector<int> res_opt = sol_opt.nextGreaterElement(nums1, nums2);
    vector<int> res_base = sol_base.nextGreaterElement(nums1, nums2);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Optimized: " << format_vector(res_opt) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 496: Next Greater Element I (Interview Mode) ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard Queries)", {4, 1, 2}, {1, 3, 4, 2}, {-1, 3, -1});

        runComparativeTest("Example 2 (Ascending Sequence)", {2, 4}, {1, 2, 3, 4}, {3, -1});

        runComparativeTest("Edge Case 1 (Strictly Decreasing)", {5, 4, 3}, {5, 4, 3, 2, 1}, {-1, -1, -1});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}