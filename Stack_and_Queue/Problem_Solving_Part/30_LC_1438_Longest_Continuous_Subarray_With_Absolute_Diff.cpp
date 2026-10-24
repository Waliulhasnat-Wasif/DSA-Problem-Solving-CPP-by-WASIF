#include <algorithm>
#include <deque>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::deque;
using std::endl;
using std::string;
using std::vector;

class SolutionBaseline {
public:
    int longestSubarray(const vector<int>& nums, int limit) const {
        int max_len = 0;
        size_t n = nums.size();

        for (size_t i = 0; i < n; ++i) {
            int curr_min = nums[i];
            int curr_max = nums[i];

            for (size_t j = i; j < n; ++j) {
                curr_min = std::min(curr_min, nums[j]);
                curr_max = std::max(curr_max, nums[j]);

                if (curr_max - curr_min <= limit) {
                    max_len = std::max(max_len, static_cast<int>(j - i + 1));
                } else {
                    break;
                }
            }
        }
        return max_len;
    }
};

class SolutionStandard {
public:
    int longestSubarray(const vector<int>& nums, int limit) const {
        size_t n = nums.size();
        if (n == 0) return 0;

        deque<int> max_dq;  // Decreasing
        deque<int> min_dq;  // Increasing

        size_t left = 0;
        size_t max_len = 0;

        for (size_t right = 0; right < n; ++right) {
            int val = nums[right];

            while (!max_dq.empty() && max_dq.back() < val) {
                max_dq.pop_back();
            }
            max_dq.push_back(val);

            while (!min_dq.empty() && min_dq.back() > val) {
                min_dq.pop_back();
            }
            min_dq.push_back(val);

            while (max_dq.front() - min_dq.front() > limit) {
                if (max_dq.front() == nums[left]) max_dq.pop_front();
                if (min_dq.front() == nums[left]) min_dq.pop_front();
                left++;
            }

            max_len = std::max(max_len, right - left + 1);
        }

        return static_cast<int>(max_len);
    }
};

class SolutionExtreme {
private:
    struct FlatDeque {
        vector<int> data;
        size_t head = 0;
        size_t tail = 0;

        explicit FlatDeque(size_t cap) : data(cap) {}

        bool empty() const {
            return head == tail;
        }
        int front() const {
            return data[head];
        }
        int back() const {
            return data[tail - 1];
        }

        void pop_front() {
            head++;
        }
        void pop_back() {
            tail--;
        }
        void push_back(int val) {
            data[tail++] = val;
        }
    };

public:
    int longestSubarray(const vector<int>& nums, int limit) const {
        size_t n = nums.size();
        if (n == 0) return 0;

        FlatDeque max_dq(n);
        FlatDeque min_dq(n);

        size_t left = 0;
        size_t max_len = 0;

        for (size_t right = 0; right < n; ++right) {
            int val = nums[right];

            while (!max_dq.empty() && max_dq.back() < val) {
                max_dq.pop_back();
            }
            max_dq.push_back(val);

            while (!min_dq.empty() && min_dq.back() > val) {
                min_dq.pop_back();
            }
            min_dq.push_back(val);

            while (max_dq.front() - min_dq.front() > limit) {
                if (max_dq.front() == nums[left]) max_dq.pop_front();
                if (min_dq.front() == nums[left]) min_dq.pop_front();
                left++;
            }

            max_len = std::max(max_len, right - left + 1);
        }

        return static_cast<int>(max_len);
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

void runComparativeTest(const string& test_name, const vector<int>& nums, int limit, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: nums = " << format_vector(nums) << ", limit = " << limit << "\n";

    SolutionStandard sol_std;
    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    int res_std = sol_std.longestSubarray(nums, limit);
    int res_ext = sol_ext.longestSubarray(nums, limit);
    int res_base = sol_base.longestSubarray(nums, limit);

    cout << "Expected:  " << expected << "\n";
    cout << "Extreme:   " << res_ext << "\n";
    cout << "Standard:  " << res_std << "\n";

    bool passed = (res_ext == expected && res_std == expected && res_base == expected);

    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 1438: Longest Subarray Limit (Extreme Edition) ---\n\n";

    try {
        runComparativeTest("Example 1", {8, 2, 4, 7}, 4, 2);

        runComparativeTest("Example 2", {10, 1, 2, 4, 7, 2}, 5, 4);

        runComparativeTest("Example 3 (Limit 0)", {4, 2, 2, 2, 4, 4, 2, 2}, 0, 3);

        runComparativeTest("Edge Case (All Within Limit)", {1, 1, 1, 1}, 2, 4);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}