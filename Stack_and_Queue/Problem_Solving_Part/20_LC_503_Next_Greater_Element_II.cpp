#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;
using std::underflow_error;
using std::vector;

// ==========================================
// Upgraded Generic Monotonic Stack (Zero-Allocation Vector Backend)
// ==========================================
template <typename T, typename PopCondition>
class MonotonicStack {
public:
    // HIGHLIGHT: Removed default construction '= PopCondition()' because
    // capturing lambdas (like [&nums]) cannot be default-constructed in C++.
    explicit MonotonicStack(size_t max_capacity, const PopCondition& pop_cond) : pop_condition_(pop_cond) {
        stack_.reserve(max_capacity);
    }

    ~MonotonicStack() = default;

    // HIGHLIGHT: Separated the resolution logic so it can be called independently.
    // Perfect for the second pass of a circular array where pushing is redundant.
    template <typename Callback>
    void resolve(const T& val, Callback on_pop) {
        while (!stack_.empty() && pop_condition_(stack_.back(), val)) {
            on_pop(stack_.back());
            stack_.pop_back();
        }
    }

    // Push Operation with Synchronous Callback
    template <typename Callback>
    void push(const T& val, Callback on_pop) {
        resolve(val, on_pop);
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
    vector<T> stack_;
    PopCondition pop_condition_;
};

// ==========================================
// Approach 1: Baseline Architecture (Brute-Force Circular Search)
// Methodologically proves why Monotonic Stacks are required for performance.
// Time Complexity: O(N^2) | Auxiliary Space: O(1)
// ==========================================
class SolutionBaseline {
public:
    vector<int> nextGreaterElements(const vector<int>& nums) const {
        size_t n = nums.size();
        vector<int> res(n, -1);

        for (size_t i = 0; i < n; ++i) {
            // Look ahead up to N-1 elements circularly
            for (size_t j = 1; j < n; ++j) {
                size_t circular_idx = (i + j) % n;
                if (nums[circular_idx] > nums[i]) {
                    res[i] = nums[circular_idx];
                    break;
                }
            }
        }

        return res;
    }
};

// ==========================================
// Approach 2: Optimized Architecture (Event-Driven Monotonic Stack)
// FAANG-level robust execution. Solves the circular constraint in strict O(N).
// Time Complexity: Strict O(N) | Space Complexity: O(N)
// ==========================================
class SolutionOptimized {
public:
    vector<int> nextGreaterElements(const vector<int>& nums) const {
        size_t n = nums.size();
        vector<int> res(n, -1);

        // Condition: Pop if the value at top's index is less than the current value
        auto nge_condition = [&nums](size_t top_idx, size_t curr_idx) { return nums[top_idx] < nums[curr_idx]; };

        // Initialize stack for indices
        MonotonicStack<size_t, decltype(nge_condition)> mono_stack(n, nge_condition);

        // Pass 1: Standard Left-to-Right traversal
        for (size_t i = 0; i < n; ++i) {
            mono_stack.push(i, [&](size_t popped_idx) { res[popped_idx] = nums[i]; });
        }

        // Pass 2: Circular resolution.
        // We only resolve existing elements in the stack. No redundant pushes.
        for (size_t i = 0; i < n - 1; ++i) {
            // Early exit optimization: If stack is empty, all NGEs are found.
            if (mono_stack.empty()) {
                break;
            }

            mono_stack.resolve(i, [&](size_t popped_idx) { res[popped_idx] = nums[i]; });
        }

        return res;
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
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
    cout << "--- Testing LC 503: Next Greater Element II ---\n\n";

    try {
        // 1. Example 1: Basic Circular
        runComparativeTest("Example 1 (Basic Circular)", {1, 2, 1}, {2, -1, 2});

        // 2. Example 2: Extended Circular
        runComparativeTest("Example 2 (Extended Circular)", {1, 2, 3, 4, 3}, {2, 3, 4, -1, 4});

        // 3. Edge Case: All identical elements
        runComparativeTest("Edge Case 1 (All Identical)", {5, 5, 5, 5}, {-1, -1, -1, -1});

        // 4. Edge Case: Strictly decreasing
        runComparativeTest("Edge Case 2 (Strictly Decreasing)", {5, 4, 3, 2, 1}, {-1, 5, 5, 5, 5});

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}