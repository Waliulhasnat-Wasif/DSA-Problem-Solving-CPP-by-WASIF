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
    // 1. Constructor (Fail-Fast Guard included)
    explicit Deque(size_t initial_capacity = 2)
        : arr_(new int[initial_capacity]), capacity_(initial_capacity), current_size_(0), front_index_(0), rear_index_(0) {
        if (initial_capacity == 0) {
            throw invalid_argument("Deque capacity must be greater than zero.");
        }
    }

    // 2. Destructor
    ~Deque() {
        delete[] arr_;
    }

    // 3. Copy Constructor (Deep Copy + Linear Unwrapping)
    Deque(const Deque& other) : capacity_(other.capacity_), current_size_(other.current_size_), front_index_(0), rear_index_(other.current_size_) {
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
        // Safely nullify the source object
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
        // Backward movement formula: Safely decrements index in a circular fashion
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
        // Forward movement formula
        rear_index_ = (rear_index_ + 1) % capacity_;
        current_size_++;
    }

    // Pop from Front -> Strict O(1) Time
    void popFront() {
        if (empty()) {
            throw underflow_error("Error: Deque Underflow! Cannot popFront from an empty deque.");
        }
        // Forward movement formula for shrinking from the front
        front_index_ = (front_index_ + 1) % capacity_;
        current_size_--;
    }

    // Pop from Back -> Strict O(1) Time
    void popBack() {
        if (empty()) {
            throw underflow_error("Error: Deque Underflow! Cannot popBack from an empty deque.");
        }
        // Backward movement formula for shrinking from the back
        rear_index_ = (rear_index_ - 1 + capacity_) % capacity_;
        current_size_--;
    }

    // Front Operation -> O(1) Time
    int front() const {
        if (empty()) {
            throw underflow_error("Error: Deque is empty! No front element.");
        }
        return arr_[front_index_];
    }

    // Back Operation -> O(1) Time
    int back() const {
        if (empty()) {
            throw underflow_error("Error: Deque is empty! No back element.");
        }
        // Resolving the logical last element's index securely
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

    // Amortized O(n) Time: Doubles the capacity and unwraps the circular array
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

        // Resetting indices after logical unwrapping
        front_index_ = 0;
        rear_index_ = current_size_;
    }
};

// ==========================================
// Test Driver
// ==========================================
int main() {
    try {
        Deque dq1;  // Capacity starts at 2

        cout << "--- Testing Push Operations & Circular Behavior ---" << endl;
        dq1.pushBack(10);  // arr[0]=10, front=0, rear=1
        dq1.pushFront(5);  // Backward wrap-around: arr[1]=5, front=1, rear=1

        cout << "Current Front: " << dq1.front() << endl;           // 5
        cout << "Current Back: " << dq1.back() << endl;             // 10
        cout << "Current Capacity: " << dq1.getCapacity() << endl;  // 2

        dq1.pushFront(1);  // Triggers resize! Unwraps and puts 1 at the new front

        cout << "\nAfter pushFront(1) [Triggered Resize]:" << endl;
        cout << "New Front: " << dq1.front() << endl;           // 1
        cout << "New Back: " << dq1.back() << endl;             // 10
        cout << "New Size: " << dq1.size() << endl;             // 3
        cout << "New Capacity: " << dq1.getCapacity() << endl;  // 4

        // Testing Pop Operations
        cout << "\n--- Pop Operations ---" << endl;
        dq1.popBack();                                                // Removes 10 from the back
        cout << "After popBack(), New Back: " << dq1.back() << endl;  // 5

        dq1.popFront();                                                  // Removes 1 from the front
        cout << "After popFront(), New Front: " << dq1.front() << endl;  // 5

        // Copy Constructor Validation
        Deque dq2 = dq1;
        cout << "\n--- dq2 (Copied from dq1) ---" << endl;
        dq2.pushBack(99);
        cout << "dq2 Front: " << dq2.front() << ", dq2 Back: " << dq2.back() << endl;   // 5, 99
        cout << "dq1 Back remains: " << dq1.back() << " (Deep Copy Isolated)" << endl;  // 5

        // Move Assignment Validation
        Deque dq3;
        dq3 = move(dq2);
        cout << "\n--- dq3 (Move-Assigned from dq2) ---" << endl;
        cout << "dq3 Front: " << dq3.front() << ", dq3 Back: " << dq3.back() << endl;  // 5, 99
        cout << "dq2 Size after move: " << dq2.size() << " (Empty: " << (dq2.empty() ? "Yes" : "No") << ")" << endl;

        // Exception Handling Test
        cout << "\n--- Edge Case Testing ---" << endl;
        Deque empty_deque;
        empty_deque.popFront();  // Triggers the underflow exception
    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}