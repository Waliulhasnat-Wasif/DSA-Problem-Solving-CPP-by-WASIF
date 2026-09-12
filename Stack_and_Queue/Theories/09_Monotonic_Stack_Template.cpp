#include <functional>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::underflow_error;

// Generic Monotonic Stack Implementation
template <typename T, typename PopCondition = std::less<T>>
class MonotonicStack {
public:
    explicit MonotonicStack(PopCondition pop_cond = PopCondition()) : pop_condition_(pop_cond) {}

    ~MonotonicStack() = default;
    MonotonicStack(const MonotonicStack& other) = default;
    MonotonicStack(MonotonicStack&& other) noexcept = default;
    MonotonicStack& operator=(const MonotonicStack& other) = default;
    MonotonicStack& operator=(MonotonicStack&& other) noexcept = default;

    // 3a. Push Operation (Optimized with Callback) -> O(1) Space Overhead
    // The callback is invoked synchronously for every element that gets popped.
    template <typename Callback>
    void push(const T& val, Callback on_pop) {
        while (!stack_.empty() && pop_condition_(stack_.top(), val)) {
            on_pop(stack_.top());  // Pass the popped element directly to the caller
            stack_.pop();
        }
        stack_.push(val);
    }

    // 3b. Push Operation (Discard Mode)
    // Used when the caller doesn't care about the popped elements.
    void push(const T& val) {
        // Calls the above push with a No-Op (Do nothing) lambda
        push(val, [](const T&) {});
    }

    void pop() {
        if (empty()) {
            throw underflow_error("Error: Stack Underflow! Cannot pop from empty stack.");
        }
        stack_.pop();
    }

    const T& top() const {
        if (empty()) {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        return stack_.top();
    }

    bool empty() const {
        return stack_.empty();
    }

    size_t size() const {
        return stack_.size();
    }

private:
    std::stack<T> stack_;
    PopCondition pop_condition_;
};

// ==========================================
// Helper Struct for Test Driver
// ==========================================
struct Element {
    int value;
    size_t index;
};

// ==========================================
// Test Driver (Solving "Next Greater Element")
// ==========================================
int main() {
    try {
        std::vector<int> nums = {2, 1, 2, 4, 3};
        std::vector<int> result(nums.size(), -1);

        cout << "--- Testing Zero-Allocation Monotonic Stack ---" << endl;
        cout << "Input Array: [ ";
        for (int num : nums) {
            cout << num << " ";
        }
        cout << "]" << endl;

        auto nge_condition = [](const Element& top, const Element& current) { return top.value < current.value; };

        MonotonicStack<Element, decltype(nge_condition)> mono_stack(nge_condition);

        for (size_t i = 0; i < nums.size(); i++) {
            Element current_elem = {nums[i], i};

            // HIGHLIGHT: The Callback Pattern in action!
            // Instead of creating and returning a vector, the stack hands us the
            // popped elements one by one exactly when they are popped.
            // We capture 'result' and 'current_elem' by reference [&] to update them.
            mono_stack.push(current_elem, [&](const Element& popped_elem) { result[popped_elem.index] = current_elem.value; });
        }

        cout << "\nResults (Next Greater Elements):" << endl;
        for (size_t i = 0; i < nums.size(); i++) {
            cout << "Element " << nums[i] << " at index " << i << " -> NGE: " << result[i] << endl;
        }

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}