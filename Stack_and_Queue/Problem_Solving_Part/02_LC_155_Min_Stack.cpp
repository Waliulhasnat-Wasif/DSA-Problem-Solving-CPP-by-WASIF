#include <algorithm>
#include <exception>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::min;
using std::pair;
using std::stack;
using std::string;
using std::underflow_error;

class MinStackTwoStacks {
public:
    MinStackTwoStacks() = default;
    ~MinStackTwoStacks() = default;
    MinStackTwoStacks(const MinStackTwoStacks& other) = default;
    MinStackTwoStacks(MinStackTwoStacks&& other) noexcept = default;
    MinStackTwoStacks& operator=(const MinStackTwoStacks& other) = default;
    MinStackTwoStacks& operator=(MinStackTwoStacks&& other) noexcept = default;

    void push(int val) {
        main_stack_.push(val);
        if (min_stack_.empty() || val <= min_stack_.top()) {
            min_stack_.push(val);
        }
    }

    void pop() {
        if (main_stack_.empty()) {
            throw underflow_error("Error: Stack Underflow!");
        }
        if (main_stack_.top() == min_stack_.top()) {
            min_stack_.pop();
        }
        main_stack_.pop();
    }

    int top() const {
        if (main_stack_.empty()) {
            throw underflow_error("Error: Stack is empty!");
        }
        return main_stack_.top();
    }

    int getMin() const {
        if (min_stack_.empty()) {
            throw underflow_error("Error: Stack is empty!");
        }
        return min_stack_.top();
    }

private:
    stack<int> main_stack_;
    stack<int> min_stack_;
};

class MinStack {
public:
    MinStack() = default;
    ~MinStack() = default;
    MinStack(const MinStack& other) = default;
    MinStack(MinStack&& other) noexcept = default;
    MinStack& operator=(const MinStack& other) = default;
    MinStack& operator=(MinStack&& other) noexcept = default;

    void push(int val) {
        if (st_.empty()) {
            st_.push({val, val});
        } else {
            int current_min = min(val, st_.top().second);
            st_.push({val, current_min});
        }
    }

    void pop() {
        if (st_.empty()) {
            throw underflow_error("Error: Stack Underflow!");
        }
        st_.pop();
    }

    int top() const {
        if (st_.empty()) {
            throw underflow_error("Error: Stack is empty!");
        }
        return st_.top().first;
    }

    int getMin() const {
        if (st_.empty()) {
            throw underflow_error("Error: Stack is empty!");
        }
        return st_.top().second;
    }

private:
    stack<pair<int, int>> st_;
};

void runDualArchitectureTest(const string& test_name) {
    cout << "Test Case: " << test_name << "\n";

    MinStack min_stack_pair;
    MinStackTwoStacks min_stack_two;

    cout << "Operation: push(-2)\n";
    min_stack_pair.push(-2);
    min_stack_two.push(-2);

    cout << "Operation: push(0)\n";
    min_stack_pair.push(0);
    min_stack_two.push(0);

    cout << "Operation: push(-3)\n";
    min_stack_pair.push(-3);
    min_stack_two.push(-3);

    int min1_pair = min_stack_pair.getMin();
    int min1_two = min_stack_two.getMin();
    cout << "Operation: getMin() -> Pair Approach: " << min1_pair << " | Two-Stack Approach: " << min1_two << " | Expected: -3 "
         << (min1_pair == -3 && min1_two == -3 ? "[PASS]" : "[FAIL]") << "\n";

    cout << "Operation: pop()\n";
    min_stack_pair.pop();
    min_stack_two.pop();

    int top1_pair = min_stack_pair.top();
    int top1_two = min_stack_two.top();
    cout << "Operation: top()    -> Pair Approach: " << top1_pair << "  | Two-Stack Approach: " << top1_two << "  | Expected: 0  "
         << (top1_pair == 0 && top1_two == 0 ? "[PASS]" : "[FAIL]") << "\n";

    int min2_pair = min_stack_pair.getMin();
    int min2_two = min_stack_two.getMin();
    cout << "Operation: getMin() -> Pair Approach: " << min2_pair << " | Two-Stack Approach: " << min2_two << " | Expected: -2 "
         << (min2_pair == -2 && min2_two == -2 ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LeetCode 155: Min Stack ---\n\n";

    try {
        runDualArchitectureTest("Example 1 (Dual-Architecture Validation)");
    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}