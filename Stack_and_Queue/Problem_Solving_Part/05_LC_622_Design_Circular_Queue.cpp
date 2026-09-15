#include <iostream>
#include <string>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;

// ==========================================
// User's Architecture: Counter-based Circular Queue
// ==========================================
class MyCircularQueue {
public:
    // 1. Constructor (Fixed capacity for LC 622)
    // Defensively anchoring arr_ to nullptr before heap allocation
    explicit MyCircularQueue(int k) : arr_(nullptr), capacity_(static_cast<size_t>(k)), current_size_(0), front_index_(0), rear_index_(0) {
        arr_ = new int[capacity_];
    }

    // 2. Destructor
    ~MyCircularQueue() {
        delete[] arr_;
    }

    // 3. Copy Constructor (Deep Copy + Unwrapping Defragmentation)
    MyCircularQueue(const MyCircularQueue& other)
        : arr_(nullptr), capacity_(other.capacity_), current_size_(other.current_size_), front_index_(0), rear_index_(other.current_size_) {
        arr_ = new int[capacity_];
        // Unwrap the source queue into a linear format in the new memory block
        for (size_t i = 0; i < current_size_; i++) {
            arr_[i] = other.arr_[(other.front_index_ + i) % other.capacity_];
        }
    }

    // 4. Move Constructor (O(1) Ownership Transfer)
    MyCircularQueue(MyCircularQueue&& other) noexcept
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

    // 5. Custom Swap Function (ADL-enabled)
    friend void swap(MyCircularQueue& first, MyCircularQueue& second) noexcept {
        using std::swap;
        swap(first.arr_, second.arr_);
        swap(first.capacity_, second.capacity_);
        swap(first.current_size_, second.current_size_);
        swap(first.front_index_, second.front_index_);
        swap(first.rear_index_, second.rear_index_);
    }

    // 6. Unified Assignment Operator (Canonical Copy-and-Swap)
    MyCircularQueue& operator=(MyCircularQueue temp) {
        swap(*this, temp);
        return *this;
    }

    // ----------------------------------------------------
    // LeetCode API Implementations
    // ----------------------------------------------------

    // Enqueue Operation -> Strict O(1) Time
    bool enQueue(int value) {
        if (isFull()) {
            return false;  // Cannot push, queue is full
        }
        arr_[rear_index_] = value;
        rear_index_ = (rear_index_ + 1) % capacity_;  // Circular wrap-around
        current_size_++;
        return true;
    }

    // Dequeue Operation -> Strict O(1) Time
    bool deQueue() {
        if (isEmpty()) {
            return false;  // Cannot pop, queue is empty
        }
        front_index_ = (front_index_ + 1) % capacity_;  // Circular wrap-around
        current_size_--;
        return true;
    }

    // Front Operation -> Strict O(1) Time
    int Front() const {
        if (isEmpty()) {
            return -1;  // LeetCode requirement instead of throwing error
        }
        return arr_[front_index_];
    }

    // Rear Operation -> Strict O(1) Time
    int Rear() const {
        if (isEmpty()) {
            return -1;  // LeetCode requirement instead of throwing error
        }
        // User's perfectly implemented logical last element index calculation
        size_t last_index = (rear_index_ == 0) ? capacity_ - 1 : rear_index_ - 1;
        return arr_[last_index];
    }

    // Empty Check -> Strict O(1) Time
    bool isEmpty() const {
        return current_size_ == 0;
    }

    // Full Check -> Strict O(1) Time
    bool isFull() const {
        return current_size_ == capacity_;
    }

private:
    int* arr_;
    size_t capacity_;
    size_t current_size_;
    size_t front_index_;
    size_t rear_index_;
};

// ==========================================
// Test Execution Engine (Separation of Concerns)
// ==========================================
void runUserArchitectureTest(const string& test_name) {
    cout << "Test Case: " << test_name << "\n";

    MyCircularQueue my_queue(3);  // Initialize with capacity 3

    cout << "Operation: enQueue(1) -> Expected: true | Output: " << (my_queue.enQueue(1) ? "true" : "false") << "\n";
    cout << "Operation: enQueue(2) -> Expected: true | Output: " << (my_queue.enQueue(2) ? "true" : "false") << "\n";
    cout << "Operation: enQueue(3) -> Expected: true | Output: " << (my_queue.enQueue(3) ? "true" : "false") << "\n";

    // Should fail because capacity is 3
    cout << "Operation: enQueue(4) -> Expected: false | Output: " << (my_queue.enQueue(4) ? "true" : "false") << "\n";

    cout << "Operation: Rear()     -> Expected: 3 | Output: " << my_queue.Rear() << "\n";

    cout << "Operation: isFull()   -> Expected: true | Output: " << (my_queue.isFull() ? "true" : "false") << "\n";

    cout << "Operation: deQueue()  -> Expected: true | Output: " << (my_queue.deQueue() ? "true" : "false") << "\n";

    cout << "Operation: enQueue(4) -> Expected: true | Output: " << (my_queue.enQueue(4) ? "true" : "false") << "\n";

    cout << "Operation: Rear()     -> Expected: 4 | Output: " << my_queue.Rear() << "\n";

    cout << string(80, '-') << "\n";
}

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LC 622 (User's Adapted Architecture) ---\n\n";

    try {
        runUserArchitectureTest("Example 1 (Basic Circular Queue Validation)");
    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}