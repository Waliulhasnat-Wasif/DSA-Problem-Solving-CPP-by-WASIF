#include <iostream>
#include <stack>
#include <stdexcept>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::underflow_error;

class Queue {
public:
    // 1. Constructor and Rule of Five (Leveraging RAII)
    // std::stack manages its own memory automatically.
    Queue() = default;
    ~Queue() = default;
    Queue(const Queue& other) = default;
    Queue(Queue&& other) noexcept = default;
    Queue& operator=(const Queue& other) = default;
    Queue& operator=(Queue&& other) noexcept = default;

    // 2. Push / Enqueue Operation -> Strict O(1) Time
    // Logic: Always push new elements to the in_stack_.
    void push(int val) {
        in_stack_.push(val);
    }

    // 3. Pop / Dequeue Operation -> Amortized O(1) Time
    void pop() {
        if (empty()) {
            throw underflow_error("Error: Queue Underflow! Cannot pop from an empty queue.");
        }

        // Ensure out_stack_ has the elements in correct FIFO order
        transferIfNeeded();
        out_stack_.pop();
    }

    // 4. Front Operation (Read-only) -> Amortized O(1) Time
    int front() const {
        if (empty()) {
            throw underflow_error("Error: Queue is empty! No front element.");
        }

        // Ensure out_stack_ has the elements in correct FIFO order
        transferIfNeeded();
        return out_stack_.top();
    }

    // 5. Empty Check (Read-only) -> Strict O(1) Time
    bool empty() const {
        return in_stack_.empty() && out_stack_.empty();
    }

    // 6. Size Check (Read-only) -> Strict O(1) Time
    size_t size() const {
        return in_stack_.size() + out_stack_.size();
    }

private:
    // 'mutable' allows these variables to be modified even inside 'const'
    // functions like front(), preserving Logical Constness.
    mutable std::stack<int> in_stack_;   // Receives all incoming pushed elements
    mutable std::stack<int> out_stack_;  // Reverses order to serve front() & pop()

    // Helper function to move elements from in_stack_ to out_stack_ ONLY when
    // out_stack_ is empty
    void transferIfNeeded() const {
        if (out_stack_.empty()) {
            while (!in_stack_.empty()) {
                out_stack_.push(in_stack_.top());
                in_stack_.pop();
            }
        }
    }
};

int main() {
    try {
        Queue q1;

        cout << "--- Testing Push Operations (Simulating Queue) ---" << endl;
        q1.push(10);  // in_stack: [10]
        q1.push(20);  // in_stack: [10, 20]
        q1.push(30);  // in_stack: [10, 20, 30]

        // Should behave strictly like a FIFO Queue
        cout << "Front element: " << q1.front() << endl;  // Expected: 10
        cout << "Current Size: " << q1.size() << endl;    // Expected: 3

        cout << "\n--- Testing Pop Operations ---" << endl;
        q1.pop();                                                      // Removes 10 (out_stack pops 10)
        cout << "After first pop, new Front: " << q1.front() << endl;  // Exp: 20

        q1.push(40);                                                        // in_stack gets 40, out_stack still has [30, 20]
        cout << "After pushing 40, Front remains: " << q1.front() << endl;  // Expected: 20

        q1.pop();                                                              // Removes 20
        q1.pop();                                                              // Removes 30
        cout << "After popping 20 and 30, new Front: " << q1.front() << endl;  // Expected: 40 (Triggers transfer of 40)

        // Copy Constructor Validation
        cout << "\n--- Testing Rule of Five (Deep Copy via RAII) ---" << endl;
        Queue q2 = q1;
        q2.push(99);
        cout << "q2 Front: " << q2.front() << ", q2 Size: " << q2.size() << endl;     // 40, 2
        cout << "q1 Size remains: " << q1.size() << " (Deep Copy Isolated)" << endl;  // 1

        // Move Constructor Validation
        Queue q3 = std::move(q2);
        cout << "\n--- Testing Move Semantics ---" << endl;
        cout << "q3 Front: " << q3.front() << ", q3 Size: " << q3.size() << endl;  // 40, 2
        cout << "q2 is empty: " << (q2.empty() ? "Yes" : "No") << endl;            // Yes

        // Exception Handling Test
        cout << "\n--- Edge Case Testing ---" << endl;
        Queue empty_queue;
        empty_queue.pop();  // Triggers the underflow exception

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}