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

class Queue {
public:
    // 1. Constructor (Fail-Fast Guard executed BEFORE allocation)
    explicit Queue(size_t initial_capacity = 2) : arr_(nullptr), capacity_(initial_capacity), current_size_(0), front_index_(0), rear_index_(0) {
        if (initial_capacity == 0) {
            throw invalid_argument("Capacity must be > 0");
        }
        arr_ = new int[capacity_];
    }

    // 2. Destructor
    ~Queue() {
        delete[] arr_;
    }

    // 3. Copy Constructor (Deep Copy + Linear Unwrapping)
    Queue(const Queue& other)
        : arr_(nullptr), capacity_(other.capacity_), current_size_(other.current_size_), front_index_(0), rear_index_(other.current_size_) {
        arr_ = new int[capacity_];
        for (size_t i = 0; i < current_size_; i++) {
            arr_[i] = other.arr_[(other.front_index_ + i) % other.capacity_];
        }
    }

    // 4. Move Constructor (O(1) Strict Ownership Transfer)
    Queue(Queue&& other) noexcept
        : arr_(other.arr_),
          capacity_(other.capacity_),
          current_size_(other.current_size_),
          front_index_(other.front_index_),
          rear_index_(other.rear_index_) {
        other.arr_ = nullptr;
        other.capacity_ = 0;
        other.current_size_ = 0;
        other.front_index_ = 0;
        other.rear_index_ = 0;
    }

    // 5. Custom Friend Swap Function (ADL-enabled)
    friend void swap(Queue& first, Queue& second) noexcept {
        using std::swap;
        swap(first.arr_, second.arr_);
        swap(first.capacity_, second.capacity_);
        swap(first.current_size_, second.current_size_);
        swap(first.front_index_, second.front_index_);
        swap(first.rear_index_, second.rear_index_);
    }

    // 6. Unified Assignment Operator (Canonical Copy-and-Swap)
    Queue& operator=(Queue temp) {
        swap(*this, temp);
        return *this;
    }

    void push(int val) {
        if (current_size_ == capacity_) {
            resize();
        }
        arr_[rear_index_] = val;
        rear_index_ = (rear_index_ + 1) % capacity_;
        current_size_++;
    }

    void pop() {
        if (empty()) {
            throw underflow_error("Error: Queue Underflow!");
        }
        front_index_ = (front_index_ + 1) % capacity_;
        current_size_--;
    }

    int front() const {
        if (empty()) {
            throw underflow_error("Error: Queue is empty!");
        }
        return arr_[front_index_];
    }

    int back() const {
        if (empty()) {
            throw underflow_error("Error: Queue is empty!");
        }
        size_t last_index = (rear_index_ == 0) ? capacity_ - 1 : rear_index_ - 1;
        return arr_[last_index];
    }

    bool empty() const {
        return current_size_ == 0;
    }

    size_t size() const {
        return current_size_;
    }

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
        int* temp_arr = new int[new_capacity];

        for (size_t i = 0; i < current_size_; i++) {
            temp_arr[i] = arr_[(front_index_ + i) % capacity_];
        }

        delete[] arr_;
        arr_ = temp_arr;
        capacity_ = new_capacity;

        front_index_ = 0;
        rear_index_ = current_size_;
    }
};

class Stack {
public:
    // Leveraging RAII: The compiler automatically calls the Deep Copy and Move
    // Constructors of your custom Queue class. No manual memory logic needed!
    Stack() = default;
    ~Stack() = default;
    Stack(const Stack& other) = default;
    Stack(Stack&& other) noexcept = default;
    Stack& operator=(const Stack& other) = default;
    Stack& operator=(Stack&& other) noexcept = default;

    // Push Operation -> Amortized O(N) Time
    void push(int val) {
        temp_queue_.push(val);

        while (!main_queue_.empty()) {
            temp_queue_.push(main_queue_.front());
            main_queue_.pop();
        }

        // Utilizes the custom ADL 'friend void swap' you wrote in your Queue class!
        // This makes swapping the two queues a strict O(1) pointer-swap operation.
        using std::swap;
        swap(main_queue_, temp_queue_);
    }

    // Pop Operation -> Strict O(1) Time
    void pop() {
        if (empty()) {
            throw underflow_error("Error: Stack Underflow! Cannot pop from an empty stack.");
        }
        main_queue_.pop();
    }

    // Top Operation -> Strict O(1) Time
    int top() const {
        if (empty()) {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        return main_queue_.front();
    }

    // Empty Check -> O(1) Time
    bool empty() const {
        return main_queue_.empty();
    }

    // Size Check -> O(1) Time
    size_t size() const {
        return main_queue_.size();
    }

private:
    Queue main_queue_;
    Queue temp_queue_;
};

int main() {
    try {
        Stack s1;

        cout << "--- Testing Push Operations (Stack via Custom Queue) ---" << endl;
        s1.push(10);
        s1.push(20);
        s1.push(30);

        cout << "Top element: " << s1.top() << endl;    // Expected: 30
        cout << "Current Size: " << s1.size() << endl;  // Expected: 3

        cout << "\n--- Testing Pop Operations ---" << endl;
        s1.pop();                                                  // Removes 30
        cout << "After first pop, new Top: " << s1.top() << endl;  // Expected: 20

        // Copy Constructor Validation (Triggers the deep copy of your Queue)
        cout << "\n--- Testing Rule of Five (Deep Copy via RAII) ---" << endl;
        Stack s2 = s1;
        s2.push(99);
        cout << "s2 Top: " << s2.top() << endl;                                     // 99
        cout << "s1 Top remains: " << s1.top() << " (Deep Copy Isolated)" << endl;  // 20

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}