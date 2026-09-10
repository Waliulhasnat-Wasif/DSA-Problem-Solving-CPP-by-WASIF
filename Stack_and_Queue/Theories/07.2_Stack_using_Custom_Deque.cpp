
#include <iostream>
#include <stdexcept>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;
using std::move;
using std::underflow_error;

class Deque {
public:
    // 1. Constructor (Fail-Fast Guard executed BEFORE allocation)
    explicit Deque(size_t initial_capacity = 2) : arr_(nullptr), capacity_(initial_capacity), current_size_(0), front_index_(0), rear_index_(0) {
        if (initial_capacity == 0) {
            throw invalid_argument("Deque capacity must be greater than zero.");
        }
        arr_ = new int[capacity_];
    }

    // 2. Destructor
    ~Deque() {
        delete[] arr_;
    }

    // 3. Copy Constructor (Deep Copy + Linear Unwrapping)
    Deque(const Deque& other)
        : arr_(nullptr), capacity_(other.capacity_), current_size_(other.current_size_), front_index_(0), rear_index_(other.current_size_) {
        arr_ = new int[capacity_];
        for (size_t i = 0; i < current_size_; i++) {
            arr_[i] = other.arr_[(other.front_index_ + i) % other.capacity_];
        }
    }

    // 4. Move Constructor (O(1) Strict Ownership Transfer)
    Deque(Deque&& other) noexcept
        : arr_(other.arr_),
          capacity_(other.capacity_),
          current_size_(other.current_size_),
          front_index_(other.front_index_),
          rear_index_(other.rear_index_) {
        // Nullify source to ensure valid destruction
        other.arr_ = nullptr;
        other.capacity_ = 0;
        other.current_size_ = 0;
        other.front_index_ = 0;
        other.rear_index_ = 0;
    }

    // 5. Custom Friend Swap Function (ADL-enabled)
    friend void swap(Deque& first, Deque& second) noexcept {
        using std::swap;
        swap(first.arr_, second.arr_);
        swap(first.capacity_, second.capacity_);
        swap(first.current_size_, second.current_size_);
        swap(first.front_index_, second.front_index_);
        swap(first.rear_index_, second.rear_index_);
    }

    // 6. Unified Assignment Operator (Canonical Copy-and-Swap)
    Deque& operator=(Deque temp) {
        swap(*this, temp);
        return *this;
    }

    // Push at Front -> Amortized O(1) Time
    void pushFront(int val) {
        if (current_size_ == capacity_) {
            resize();
        }
        front_index_ = (front_index_ - 1 + capacity_) % capacity_;
        arr_[front_index_] = val;
        current_size_++;
    }

    // Push at Back -> Amortized O(1) Time
    void pushBack(int val) {
        if (current_size_ == capacity_) {
            resize();
        }
        arr_[rear_index_] = val;
        rear_index_ = (rear_index_ + 1) % capacity_;
        current_size_++;
    }

    // Pop from Front -> Strict O(1) Time
    void popFront() {
        if (empty()) {
            throw underflow_error("Error: Deque Underflow! Cannot popFront from an empty deque.");
        }
        front_index_ = (front_index_ + 1) % capacity_;
        current_size_--;
    }

    // Pop from Back -> Strict O(1) Time
    void popBack() {
        if (empty()) {
            throw underflow_error("Error: Deque Underflow! Cannot popBack from an empty deque.");
        }
        rear_index_ = (rear_index_ - 1 + capacity_) % capacity_;
        current_size_--;
    }

    // Front Operation -> Strict O(1) Time
    int front() const {
        if (empty()) {
            throw underflow_error("Error: Deque is empty! No front element.");
        }
        return arr_[front_index_];
    }

    // Back Operation -> Strict O(1) Time
    int back() const {
        if (empty()) {
            throw underflow_error("Error: Deque is empty! No back element.");
        }
        size_t last_index = (rear_index_ - 1 + capacity_) % capacity_;
        return arr_[last_index];
    }

    // Empty Check
    bool empty() const {
        return current_size_ == 0;
    }

    // Size Check
    size_t size() const {
        return current_size_;
    }

    // Capacity Check
    size_t getCapacity() const {
        return capacity_;
    }

private:
    int* arr_;
    size_t capacity_;
    size_t current_size_;
    size_t front_index_;
    size_t rear_index_;

    void resize() {
        size_t new_capacity = capacity_ * 2;
        int* new_arr = new int[new_capacity];

        // Unwrapping the circular deque into a strict linear sequence
        for (size_t i = 0; i < current_size_; i++) {
            new_arr[i] = arr_[(front_index_ + i) % capacity_];
        }

        delete[] arr_;
        arr_ = new_arr;
        capacity_ = new_capacity;
        front_index_ = 0;
        rear_index_ = current_size_;
    }
};

class Stack {
public:
    // Leveraging RAII: Rule of Zero allows the compiler to perfectly manage memory
    // by implicitly calling the Rule of Five functions from the Deque class.
    Stack() = default;
    ~Stack() = default;
    Stack(const Stack& other) = default;
    Stack(Stack&& other) noexcept = default;
    Stack& operator=(const Stack& other) = default;
    Stack& operator=(Stack&& other) noexcept = default;

    // Push Operation -> Amortized O(1) Time
    void push(int val) {
        // Direct insertion at the back of the deque, no shifting required.
        deque_.pushBack(val);
    }

    // Pop Operation -> Strict O(1) Time
    void pop() {
        if (empty()) {
            throw underflow_error("Error: Stack Underflow! Cannot pop from an empty stack.");
        }
        deque_.popBack();
    }

    // Top Operation -> Strict O(1) Time
    int top() const {
        if (empty()) {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        return deque_.back();
    }

    // Empty Check -> Strict O(1) Time
    bool empty() const {
        return deque_.empty();
    }

    // Size Check -> Strict O(1) Time
    size_t size() const {
        return deque_.size();
    }

private:
    Deque deque_;  // Only a single Deque is needed.
};

int main() {
    try {
        Stack s1;

        cout << "--- Testing Push Operations (Stack via Custom Deque) ---" << endl;
        s1.push(10);
        s1.push(20);
        s1.push(30);

        cout << "Top element: " << s1.top() << endl;    // Expected: 30
        cout << "Current Size: " << s1.size() << endl;  // Expected: 3

        cout << "\n--- Testing Pop Operations ---" << endl;
        s1.pop();                                                  // Removes 30
        cout << "After first pop, new Top: " << s1.top() << endl;  // Expected: 20

        // Copy Constructor Validation
        cout << "\n--- Testing Rule of Five (Deep Copy via RAII) ---" << endl;
        Stack s2 = s1;
        s2.push(99);
        cout << "s2 Top: " << s2.top() << endl;                                     // Expected: 99
        cout << "s1 Top remains: " << s1.top() << " (Deep Copy Isolated)" << endl;  // Expected: 20

        // Move Constructor Validation
        Stack s3 = move(s2);
        cout << "\n--- Testing Move Semantics ---" << endl;
        cout << "s3 Top: " << s3.top() << endl;                          // Expected: 99
        cout << "s2 is empty: " << (s2.empty() ? "Yes" : "No") << endl;  // Expected: Yes

        // Exception Handling Test
        cout << "\n--- Edge Case Testing ---" << endl;
        Stack empty_stack;
        empty_stack.pop();  // Triggers the underflow exception

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}