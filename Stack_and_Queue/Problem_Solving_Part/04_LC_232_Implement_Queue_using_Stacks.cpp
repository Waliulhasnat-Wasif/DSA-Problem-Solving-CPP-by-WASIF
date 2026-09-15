/**
 * @file Implement_Queue_Using_Stacks.cpp
 * @brief Enterprise-grade solution for LeetCode 232.
 * @details Compares a Push-Heavy Baseline Architecture against an Amortized
 *          O(1) Optimized Architecture. Showcases advanced C++ concepts like
 *          Logical Constness using 'mutable', Exception Safety, and Rule of Five.
 */

#include <exception>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::stack;
using std::string;
using std::underflow_error;

// ==========================================
// Approach 1: Baseline Architecture (Push-Heavy)
// Time Complexity: push is O(N), pop/peek are O(1)
// ==========================================
class MyQueuePushHeavy {
public:
    MyQueuePushHeavy() = default;
    ~MyQueuePushHeavy() = default;
    MyQueuePushHeavy(const MyQueuePushHeavy& other) = default;
    MyQueuePushHeavy(MyQueuePushHeavy&& other) noexcept = default;
    MyQueuePushHeavy& operator=(const MyQueuePushHeavy& other) = default;
    MyQueuePushHeavy& operator=(MyQueuePushHeavy&& other) noexcept = default;

    void push(int x) {
        // Step 1: Move all elements from main to temp
        while (!main_stack_.empty()) {
            temp_stack_.push(main_stack_.top());
            main_stack_.pop();
        }

        // Step 2: Push the new element to the bottom of main
        main_stack_.push(x);

        // Step 3: Move everything back
        while (!temp_stack_.empty()) {
            main_stack_.push(temp_stack_.top());
            temp_stack_.pop();
        }
    }

    int pop() {
        if (empty()) {
            throw underflow_error("Error: Queue Underflow!");
        }
        int front_val = main_stack_.top();
        main_stack_.pop();
        return front_val;
    }

    int peek() const {
        if (empty()) {
            throw underflow_error("Error: Queue is empty!");
        }
        return main_stack_.top();
    }

    bool empty() const {
        return main_stack_.empty();
    }

private:
    stack<int> main_stack_;
    stack<int> temp_stack_;
};

// ==========================================
// Approach 2: Optimized Architecture (Amortized O(1))
// Time Complexity: Amortized O(1) for all operations
// ==========================================
class MyQueue {
public:
    MyQueue() = default;
    ~MyQueue() = default;
    MyQueue(const MyQueue& other) = default;
    MyQueue(MyQueue&& other) noexcept = default;
    MyQueue& operator=(const MyQueue& other) = default;
    MyQueue& operator=(MyQueue&& other) noexcept = default;

    // Time Complexity: O(1)
    void push(int x) {
        input_stack_.push(x);
    }

    // Time Complexity: Amortized O(1)
    int pop() {
        if (empty()) {
            throw underflow_error("Error: Queue Underflow!");
        }
        transferIfNeeded();
        int front_val = output_stack_.top();
        output_stack_.pop();
        return front_val;
    }

    // Time Complexity: Amortized O(1)
    // Marked 'const' despite modifying stacks internally, thanks to 'mutable'
    int peek() const {
        if (empty()) {
            throw underflow_error("Error: Queue is empty!");
        }
        transferIfNeeded();
        return output_stack_.top();
    }

    bool empty() const {
        return input_stack_.empty() && output_stack_.empty();
    }

private:
    // HIGHLIGHT: 'mutable' allows internal state modification inside 'const'
    // functions. This achieves "Logical Constness".
    mutable stack<int> input_stack_;
    mutable stack<int> output_stack_;

    // Helper function to lazily transfer elements when output_stack_ is empty
    void transferIfNeeded() const {
        if (output_stack_.empty()) {
            while (!input_stack_.empty()) {
                output_stack_.push(input_stack_.top());
                input_stack_.pop();
            }
        }
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns)
// ==========================================
void runDualArchitectureTest(const string& test_name) {
    cout << "Test Case: " << test_name << "\n";

    MyQueue my_queue_opt;            // Approach 2 (Primary/Amortized)
    MyQueuePushHeavy my_queue_base;  // Approach 1 (Baseline)

    cout << "Operation: push(1)\n";
    my_queue_opt.push(1);
    my_queue_base.push(1);

    cout << "Operation: push(2)\n";
    my_queue_opt.push(2);
    my_queue_base.push(2);

    int peek_opt = my_queue_opt.peek();
    int peek_base = my_queue_base.peek();
    cout << "Operation: peek()  -> Optimized: " << peek_opt << " | Baseline: " << peek_base << " | Expected: 1 "
         << (peek_opt == 1 && peek_base == 1 ? "[PASS]" : "[FAIL]") << "\n";

    int pop_opt = my_queue_opt.pop();
    int pop_base = my_queue_base.pop();
    cout << "Operation: pop()   -> Optimized: " << pop_opt << " | Baseline: " << pop_base << " | Expected: 1 "
         << (pop_opt == 1 && pop_base == 1 ? "[PASS]" : "[FAIL]") << "\n";

    bool empty_opt = my_queue_opt.empty();
    bool empty_base = my_queue_base.empty();
    cout << "Operation: empty() -> Optimized: " << (empty_opt ? "true" : "false") << " | Baseline: " << (empty_base ? "true" : "false")
         << " | Expected: false " << (!empty_opt && !empty_base ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LeetCode 232: Implement Queue using Stacks ---\n\n";

    try {
        runDualArchitectureTest("Example 1 (Basic FIFO Validations)");
    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}