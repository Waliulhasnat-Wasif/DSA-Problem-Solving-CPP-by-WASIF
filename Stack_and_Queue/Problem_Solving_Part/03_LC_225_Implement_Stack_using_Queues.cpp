#include <exception>
#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::queue;
using std::string;
using std::underflow_error;

class MyStackTwoQueues {
public:
    MyStackTwoQueues() = default;
    ~MyStackTwoQueues() = default;
    MyStackTwoQueues(const MyStackTwoQueues& other) = default;
    MyStackTwoQueues(MyStackTwoQueues&& other) noexcept = default;
    MyStackTwoQueues& operator=(const MyStackTwoQueues& other) = default;
    MyStackTwoQueues& operator=(MyStackTwoQueues&& other) noexcept = default;

    void push(int val) {
        temp_queue_.push(val);

        while (!main_queue_.empty()) {
            temp_queue_.push(main_queue_.front());
            main_queue_.pop();
        }

        main_queue_.swap(temp_queue_);
    }

    int pop() {
        if (empty()) {
            throw underflow_error("Error: Stack Underflow! Cannot pop from an empty stack.");
        }
        int top_val = main_queue_.front();
        main_queue_.pop();
        return top_val;
    }

    int top() const {
        if (empty()) {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        return main_queue_.front();
    }

    bool empty() const {
        return main_queue_.empty();
    }

    size_t size() const {
        return main_queue_.size();
    }

private:
    std::queue<int> main_queue_;
    std::queue<int> temp_queue_;
};

class MyStack {
public:
    MyStack() = default;
    ~MyStack() = default;
    MyStack(const MyStack& other) = default;
    MyStack(MyStack&& other) noexcept = default;
    MyStack& operator=(const MyStack& other) = default;
    MyStack& operator=(MyStack&& other) noexcept = default;

    void push(int x) {
        q_.push(x);
        int current_size = q_.size();

        for (int i = 0; i < current_size - 1; ++i) {
            q_.push(q_.front());
            q_.pop();
        }
    }

    int pop() {
        if (q_.empty()) {
            throw underflow_error("Error: Stack Underflow!");
        }
        int top_val = q_.front();
        q_.pop();
        return top_val;
    }

    int top() const {
        if (q_.empty()) {
            throw underflow_error("Error: Stack is empty!");
        }
        return q_.front();
    }

    bool empty() const {
        return q_.empty();
    }

    size_t size() const {
        return q_.size();
    }

private:
    std::queue<int> q_;
};

void runDualArchitectureTest(const string& test_name) {
    cout << "Test Case: " << test_name << "\n";

    MyStack my_stack_single;
    MyStackTwoQueues my_stack_two;

    cout << "Operation: push(1)\n";
    my_stack_single.push(1);
    my_stack_two.push(1);

    cout << "Operation: push(2)\n";
    my_stack_single.push(2);
    my_stack_two.push(2);

    int top_single = my_stack_single.top();
    int top_two = my_stack_two.top();
    cout << "Operation: top()   -> Single Queue: " << top_single << " | Two Queue: " << top_two << " | Expected: 2 "
         << (top_single == 2 && top_two == 2 ? "[PASS]" : "[FAIL]") << "\n";

    int pop_single = my_stack_single.pop();
    int pop_two = my_stack_two.pop();
    cout << "Operation: pop()   -> Single Queue: " << pop_single << " | Two Queue: " << pop_two << " | Expected: 2 "
         << (pop_single == 2 && pop_two == 2 ? "[PASS]" : "[FAIL]") << "\n";

    bool empty_single = my_stack_single.empty();
    bool empty_two = my_stack_two.empty();
    cout << "Operation: empty() -> Single Queue: " << (empty_single ? "true" : "false") << " | Two Queue: " << (empty_two ? "true" : "false")
         << " | Expected: false " << (!empty_single && !empty_two ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LeetCode 225: Implement Stack using Queues ---\n\n";

    try {
        runDualArchitectureTest("Example 1 (Basic Operations Validation)");
    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}