#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;
using std::underflow_error;
using std::unordered_map;
using std::vector;

// ==========================================
// Upgraded Generic Monotonic Stack (Zero-Allocation Vector Backend)
// ==========================================
template <typename T, typename PopCondition = std::less<T>>
class MonotonicStack {
public:
    // HIGHLIGHT: Injecting maximum capacity into the constructor to reserve
    // memory upfront, ensuring zero dynamic reallocation for the stack.
    explicit MonotonicStack(size_t max_capacity, PopCondition pop_cond = PopCondition()) : pop_condition_(pop_cond) {
        stack_.reserve(max_capacity);
    }

    ~MonotonicStack() = default;

    // Push Operation with Synchronous Callback
    template <typename Callback>
    void push(const T& val, Callback on_pop) {
        // Vector's back() replaces top(), avoiding std::stack overhead
        while (!stack_.empty() && pop_condition_(stack_.back(), val)) {
            on_pop(stack_.back());
            stack_.pop_back();
        }
        stack_.push_back(val);
    }

    void pop() {
        if (empty()) {
            throw underflow_error("Error: Stack Underflow!");
        }
        stack_.pop_back();
    }

    const T& top() const {
        if (empty()) {
            throw underflow_error("Error: Stack is empty!");
        }
        return stack_.back();
    }

    bool empty() const {
        return stack_.empty();
    }

private:
    vector<T> stack_;  // Replaced std::stack with std::vector
    PopCondition pop_condition_;
};

// ==========================================
// Approach 1: Baseline Architecture (Brute-Force Search)
// Straightforward nested loops. Methodologically proves why Monotonic Stacks
// are required for large-scale inputs.
// Time Complexity: O(N * M) | Auxiliary Space: O(1)
// ==========================================
class SolutionBaseline {
public:
    vector<int> nextGreaterElement(const vector<int>& nums1, const vector<int>& nums2) const {
        vector<int> res;
        res.reserve(nums1.size());  // Zero dynamic reallocation

        size_t n = nums2.size();

        for (int x : nums1) {
            bool found = false;
            int next_greater = -1;

            // Brute-force search in nums2
            for (size_t j = 0; j < n; ++j) {
                if (nums2[j] == x) {
                    found = true;
                }
                // Once found, look for the first element strictly greater than x
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

// ==========================================
// Approach 2: Optimized Architecture (Event-Driven Monotonic Stack)
// FAANG-level robust execution utilizing the Event-Driven Monotonic Stack
// template. Solves the NGE mapping in strictly O(N + M) time.
// Time Complexity: O(N + M) | Space Complexity: O(N)
// ==========================================
class SolutionOptimized {
public:
    vector<int> nextGreaterElement(const vector<int>& nums1, const vector<int>& nums2) const {
        unordered_map<int, int> nge_map;

        // HIGHLIGHT: Pre-allocate map buckets.
        // This prevents the underlying hash table from triggering expensive O(N)
        // rehashing operations as new elements are added.
        nge_map.reserve(nums2.size());

        // Define the Monotonic Stack condition: "Pop if top is less than current"
        auto nge_condition = [](int top, int current) { return top < current; };

        // Initialize the upgraded stack with exact capacity
        MonotonicStack<int, decltype(nge_condition)> mono_stack(nums2.size(), nge_condition);

        for (int current_val : nums2) {
            // The callback directly maps the popped element to its NGE
            mono_stack.push(current_val, [&](int popped_val) { nge_map[popped_val] = current_val; });
        }

        vector<int> res;
        res.reserve(nums1.size());  // Zero dynamic reallocation

        for (int x : nums1) {
            auto it = nge_map.find(x);
            if (it != nge_map.end()) {
                res.push_back(it->second);
            } else {
                res.push_back(-1);  // No greater element found
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
    cout << "--- Testing LC 496: Next Greater Element I ---\n\n";

    try {
        runComparativeTest("Example 1 (Standard Queries)", {4, 1, 2}, {1, 3, 4, 2}, {-1, 3, -1});

        runComparativeTest("Example 2 (Ascending Sequence)", {2, 4}, {1, 2, 3, 4}, {3, -1});

        runComparativeTest("Edge Case 1 (Strictly Decreasing)", {5, 4, 3}, {5, 4, 3, 2, 1}, {-1, -1, -1});

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}