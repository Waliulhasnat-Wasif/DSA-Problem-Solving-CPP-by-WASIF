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
    int shortestSubarray(const vector<int>& nums, int k) const {
        size_t n = nums.size();
        vector<long long> P(n + 1, 0);

        for (size_t i = 0; i < n; ++i) {
            P[i + 1] = P[i] + nums[i];
        }

        size_t min_len = n + 1;

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j <= n; ++j) {
                if (P[j] - P[i] >= static_cast<long long>(k)) {
                    min_len = std::min(min_len, j - i);
                }
            }
        }

        return min_len <= n ? static_cast<int>(min_len) : -1;
    }
};

class SolutionExtreme {
private:
    struct FlatDeque {
        vector<size_t> data;
        size_t head = 0;
        size_t tail = 0;

        explicit FlatDeque(size_t cap) : data(cap) {}

        bool empty() const {
            return head == tail;
        }
        size_t front() const {
            return data[head];
        }
        size_t back() const {
            return data[tail - 1];
        }

        void pop_front() {
            ++head;
        }
        void pop_back() {
            --tail;
        }
        void push_back(size_t idx) {
            data[tail++] = idx;
        }
    };

public:
    int shortestSubarray(const vector<int>& nums, int k) const {
        size_t n = nums.size();

        vector<long long> P(n + 1, 0);
        for (size_t i = 0; i < n; ++i) {
            P[i + 1] = P[i] + nums[i];
        }

        size_t min_len = n + 1;
        long long K = static_cast<long long>(k);

        FlatDeque dq(n + 1);

        for (size_t i = 0; i <= n; ++i) {
            while (!dq.empty() && P[i] - P[dq.front()] >= K) {
                min_len = std::min(min_len, i - dq.front());
                dq.pop_front();
            }

            while (!dq.empty() && P[dq.back()] >= P[i]) {
                dq.pop_back();
            }

            dq.push_back(i);
        }

        return min_len <= n ? static_cast<int>(min_len) : -1;
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

void runComparativeTest(const string& test_name, const vector<int>& nums, int k, int expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: nums = " << format_vector(nums) << ", k = " << k << "\n";

    SolutionExtreme sol_ext;
    SolutionBaseline sol_base;

    int res_ext = sol_ext.shortestSubarray(nums, k);
    int res_base = sol_base.shortestSubarray(nums, k);

    cout << "Expected:  " << expected << "\n";
    cout << "Extreme:   " << res_ext << "\n";
    cout << "Baseline:  " << res_base << "\n";

    bool passed = (res_ext == expected && res_base == expected);
    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 862: Shortest Subarray Sum >= K ---\n\n";

    try {
        runComparativeTest("Example 1 (Single Element)", {1}, 1, 1);

        runComparativeTest("Example 2 (Impossible)", {1, 2}, 4, -1);

        runComparativeTest("Example 3 (Negative Elements)", {2, -1, 2}, 3, 3);

        runComparativeTest("Edge Case 1 (Exact Sum Match)", {4, -1, 2, 3}, 4, 1);

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}