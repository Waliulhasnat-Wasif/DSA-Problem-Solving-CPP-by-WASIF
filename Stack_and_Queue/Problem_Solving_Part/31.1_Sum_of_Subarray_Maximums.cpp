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

const int MOD = 1e9 + 7;
const int INF = std::numeric_limits<int>::max();

class SolutionBaseline {
public:
    int sumSubarrayMaxs(const vector<int>& arr) const {
        long long total_sum = 0;
        size_t n = arr.size();

        for (size_t i = 0; i < n; ++i) {
            int curr_max = arr[i];
            for (size_t j = i; j < n; ++j) {
                curr_max = std::max(curr_max, arr[j]);
                total_sum = (total_sum + curr_max) % MOD;
            }
        }
        return static_cast<int>(total_sum);
    }
};

class SolutionStandard {
public:
    int sumSubarrayMaxs(const vector<int>& arr) const {
        size_t n = arr.size();
        vector<size_t> left(n), right(n);
        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            while (!st.empty() && arr[st.back()] <= arr[i]) {
                st.pop_back();
            }
            left[i] = st.empty() ? (i + 1) : (i - st.back());
            st.push_back(i);
        }

        st.clear();

        for (size_t i = n; i > 0; --i) {
            size_t idx = i - 1;
            while (!st.empty() && arr[st.back()] < arr[idx]) {
                st.pop_back();
            }
            right[idx] = st.empty() ? (n - idx) : (st.back() - idx);
            st.push_back(idx);
        }

        long long total_sum = 0;
        for (size_t i = 0; i < n; ++i) {
            long long contribution = (1LL * arr[i] * left[i] * right[i]) % MOD;
            total_sum = (total_sum + contribution) % MOD;
        }

        return static_cast<int>(total_sum);
    }
};

class SolutionExtreme {
public:
    int sumSubarrayMaxs(const vector<int>& arr) const {
        size_t n = arr.size();
        long long total_sum = 0;

        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i <= n; ++i) {
            int curr_val = (i == n) ? INF : arr[i];

            while (!st.empty() && arr[st.back()] <= curr_val) {
                size_t popped_idx = st.back();
                st.pop_back();

                size_t left_dist = st.empty() ? (popped_idx + 1) : (popped_idx - st.back());
                size_t right_dist = i - popped_idx;

                long long contribution = (1LL * arr[popped_idx] * left_dist * right_dist) % MOD;
                total_sum = (total_sum + contribution) % MOD;
            }
            st.push_back(i);
        }

        return static_cast<int>(total_sum);
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

void runComparativeTest(const string& test_name, const vector<int>& arr, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: arr = " << format_vector(arr) << "\n";

    SolutionStandard sol_std;
    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    int res_std = sol_std.sumSubarrayMaxs(arr);
    int res_ext = sol_ext.sumSubarrayMaxs(arr);
    int res_base = sol_base.sumSubarrayMaxs(arr);

    cout << "Expected:  " << expected << "\n";
    cout << "Extreme:   " << res_ext << "\n";
    cout << "Standard:  " << res_std << "\n";

    bool passed = (res_ext == expected && res_std == expected && res_base == expected);

    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing Subarray Maximums (One-Pass Extreme) ---\n\n";

    try {
        // [1]=1, [2]=2, [3]=3, [1,2]=2, [2,3]=3, [1,2,3]=3. Sum = 14
        runComparativeTest("Example 1 (Strictly Increasing)", {1, 2, 3}, 14);

        // [3]=3, [1]=1, [2]=2, [4]=4
        // [3,1]=3, [1,2]=2, [2,4]=4
        // [3,1,2]=3, [1,2,4]=4
        // [3,1,2,4]=4. Sum = 30
        runComparativeTest("Example 2 (Standard)", {3, 1, 2, 4}, 30);

        runComparativeTest("Edge Case 1 (Duplicates)", {2, 2, 2}, 12);

        runComparativeTest("Edge Case 2 (Descending)", {4, 3, 2, 1}, 30);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}