#include <iostream>
#include <queue>
#include <stdexcept>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::underflow_error;

class Stack {
public:
    // 1. Constructor and Rule of Five
    // Since we are using standard containers (std::queue), we can explicitly default
    // them. The compiler generates perfectly safe deep copies and moves (RAII).
    Stack() = default;
    ~Stack() = default;
    Stack(const Stack& other) = default;
    Stack(Stack&& other) noexcept = default;
    Stack& operator=(const Stack& other) = default;
    Stack& operator=(Stack&& other) noexcept = default;

    // 2. Push Operation -> Time Complexity: O(N)
    // Logic: Insert in temp, move everything from main to temp, then swap.
    void push(int val) {
        // Step 1: Push the new element to the temporary queue
        temp_queue_.push(val);

        // Step 2: Transfer all existing elements from main_queue_ to temp_queue_
        while (!main_queue_.empty()) {
            temp_queue_.push(main_queue_.front());
            main_queue_.pop();
        }

        // Step 3: Swap the names/pointers of the two queues (O(1) operation)
        std::swap(main_queue_, temp_queue_);
    }

    // 3. Pop Operation -> Time Complexity: Strict O(1)
    void pop() {
        if (empty()) {
            throw underflow_error("Error: Stack Underflow! Cannot pop from an empty stack.");
        }
        main_queue_.pop();
    }

    // 4. Top Operation -> Time Complexity: Strict O(1)
    int top() const {
        if (empty()) {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        // The front of the queue is the top of our simulated stack
        return main_queue_.front();
    }

    // 5. Empty Check -> O(1)
    bool empty() const {
        return main_queue_.empty();
    }

    // 6. Size Check -> O(1)
    size_t size() const {
        return main_queue_.size();
    }

private:
    std::queue<int> main_queue_;  // Holds the elements in Stack order (LIFO)
    std::queue<int> temp_queue_;  // Used for temporary processing during push()
};

int main() {
    try {
        Stack s1;

        cout << "--- Testing Push Operations (Simulating Stack) ---" << endl;
        s1.push(10);
        s1.push(20);
        s1.push(30);

        // Should behave strictly like a LIFO Stack
        cout << "Top element: " << s1.top() << endl;    // Expected: 30
        cout << "Current Size: " << s1.size() << endl;  // Expected: 3

        cout << "\n--- Testing Pop Operations ---" << endl;
        s1.pop();                                                  // Removes 30
        cout << "After first pop, new Top: " << s1.top() << endl;  // Expected: 20

        s1.pop();                                                   // Removes 20
        cout << "After second pop, new Top: " << s1.top() << endl;  // Expected: 10

        // Copy Constructor Validation
        cout << "\n--- Testing Rule of Five (Deep Copy) ---" << endl;
        Stack s2 = s1;
        s2.push(99);
        cout << "s2 Top: " << s2.top() << endl;                                     // 99
        cout << "s1 Top remains: " << s1.top() << " (Deep Copy Isolated)" << endl;  // 10

        // Emptying the Stack
        s1.pop();  // Removes 10
        cout << "\nIs s1 empty? " << (s1.empty() ? "Yes" : "No") << endl;

        // Exception Handling Test
        cout << "\n--- Edge Case Testing ---" << endl;
        s1.pop();  // Triggers the underflow exception

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}