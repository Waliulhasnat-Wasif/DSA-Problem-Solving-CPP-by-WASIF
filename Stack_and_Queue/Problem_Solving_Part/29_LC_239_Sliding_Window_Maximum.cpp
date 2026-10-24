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
    vector<int> maxSlidingWindow(const vector<int>& nums, int k) const {
        size_t n = nums.size();
        size_t K = static_cast<size_t>(k);
        vector<int> res;

        if (n == 0 || K == 0) return res;

        res.reserve(n - K + 1);
        for (size_t i = 0; i <= n - K; ++i) {
            int current_max = nums[i];
            for (size_t j = 1; j < K; ++j) {
                current_max = std::max(current_max, nums[i + j]);
            }
            res.push_back(current_max);
        }
        return res;
    }
};

class SolutionStandard {
public:
    vector<int> maxSlidingWindow(const vector<int>& nums, int k) const {
        size_t n = nums.size();
        size_t K = static_cast<size_t>(k);
        vector<int> res;

        if (n == 0 || K == 0) return res;

        res.reserve(n - K + 1);
        deque<size_t> dq;

        for (size_t i = 0; i < n; ++i) {
            if (!dq.empty() && i >= K && dq.front() == i - K) {
                dq.pop_front();
            }

            while (!dq.empty() && nums[dq.back()] <= nums[i]) {
                dq.pop_back();
            }

            dq.push_back(i);

            if (i >= K - 1) {
                res.push_back(nums[dq.front()]);
            }
        }
        return res;
    }
};

class SolutionExtreme {
private:
    struct CircularDeque {
        vector<size_t> data;
        size_t head = 0;
        size_t tail = 0;
        size_t count = 0;
        size_t capacity;

        explicit CircularDeque(size_t cap) : data(cap), capacity(cap) {}

        bool empty() const {
            return count == 0;
        }
        size_t front() const {
            if (empty()) {
                return 0;
            }

            return data[head];
        }

        size_t back() const {
            if (empty()) {
                return 0;
            }

            return data[tail == 0 ? capacity - 1 : tail - 1];
        }

        void pop_front() {
            ++head;
            if (head == capacity) {
                head = 0;
            }
            --count;
        }

        void pop_back() {
            if (tail == 0) {
                tail = capacity - 1;
            } else {
                --tail;
            }
            --count;
        }

        void push_back(size_t val) {
            data[tail++] = val;
            if (tail == capacity) {
                tail = 0;
            }
            ++count;
        }
    };

public:
    vector<int> maxSlidingWindow(const vector<int>& nums, int k) const {
        size_t n = nums.size();
        size_t K = static_cast<size_t>(k);
        vector<int> res;

        if (n == 0 || K == 0) return res;

        res.reserve(n - K + 1);
        CircularDeque dq(K);

        for (size_t i = 0; i < n; ++i) {
            int current_val = nums[i];

            if (!dq.empty() && i >= K && dq.front() == i - K) {
                dq.pop_front();
            }

            while (!dq.empty() && nums[dq.back()] <= current_val) {
                dq.pop_back();
            }

            dq.push_back(i);

            if (i >= K - 1) {
                res.push_back(nums[dq.front()]);
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

void runComparativeTest(const string& test_name, const vector<int>& nums, int k, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: nums = " << format_vector(nums) << ", k = " << k << "\n";

    SolutionStandard sol_std;
    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    vector<int> res_std = sol_std.maxSlidingWindow(nums, k);
    vector<int> res_ext = sol_ext.maxSlidingWindow(nums, k);
    vector<int> res_base = sol_base.maxSlidingWindow(nums, k);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Extreme:   " << format_vector(res_ext) << "\n";
    cout << "Standard:  " << format_vector(res_std) << "\n";

    bool passed = (res_ext == expected && res_std == expected && res_base == expected);

    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 239: Sliding Window Max (Extreme Edition) ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard)", {1, 3, -1, -3, 5, 3, 6, 7}, 3, {3, 3, 5, 5, 6, 7});

        runComparativeTest("Example 2 (Size 1)", {1}, 1, {1});

        runComparativeTest("Edge Case 1 (Descending)", {9, 8, 7, 6, 5}, 2, {9, 8, 7, 6});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}