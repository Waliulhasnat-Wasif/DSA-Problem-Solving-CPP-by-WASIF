#include <iostream>
#include <string>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::string;

class MyCircularDeque {
public:
    explicit MyCircularDeque(int k) : arr_(nullptr), capacity_(static_cast<size_t>(k)), current_size_(0), front_index_(0), rear_index_(0) {
        arr_ = new int[capacity_];
    }

    ~MyCircularDeque() {
        delete[] arr_;
    }

    MyCircularDeque(const MyCircularDeque& other)
        : arr_(nullptr), capacity_(other.capacity_), current_size_(other.current_size_), front_index_(0), rear_index_(other.current_size_) {
        arr_ = new int[capacity_];

        for (size_t i = 0; i < current_size_; i++) {
            arr_[i] = other.arr_[(other.front_index_ + i) % other.capacity_];
        }
    }

    MyCircularDeque(MyCircularDeque&& other) noexcept
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

    friend void swap(MyCircularDeque& first, MyCircularDeque& second) noexcept {
        using std::swap;
        swap(first.arr_, second.arr_);
        swap(first.capacity_, second.capacity_);
        swap(first.current_size_, second.current_size_);
        swap(first.front_index_, second.front_index_);
        swap(first.rear_index_, second.rear_index_);
    }

    MyCircularDeque& operator=(MyCircularDeque temp) {
        swap(*this, temp);
        return *this;
    }

    bool insertFront(int value) {
        if (isFull()) {
            return false;
        }

        front_index_ = (front_index_ - 1 + capacity_) % capacity_;
        arr_[front_index_] = value;
        current_size_++;
        return true;
    }

    bool insertLast(int value) {
        if (isFull()) {
            return false;
        }

        arr_[rear_index_] = value;
        rear_index_ = (rear_index_ + 1) % capacity_;
        current_size_++;
        return true;
    }

    bool deleteFront() {
        if (isEmpty()) {
            return false;
        }

        front_index_ = (front_index_ + 1) % capacity_;
        current_size_--;
        return true;
    }

    bool deleteLast() {
        if (isEmpty()) {
            return false;
        }

        rear_index_ = (rear_index_ - 1 + capacity_) % capacity_;
        current_size_--;
        return true;
    }

    int getFront() const {
        if (isEmpty()) {
            return -1;
        }
        return arr_[front_index_];
    }

    int getRear() const {
        if (isEmpty()) {
            return -1;
        }

        size_t last_index = (rear_index_ == 0) ? capacity_ - 1 : rear_index_ - 1;
        return arr_[last_index];
    }

    bool isEmpty() const {
        return current_size_ == 0;
    }

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

void runUserArchitectureTest(const string& test_name) {
    cout << "Test Case: " << test_name << "\n";

    MyCircularDeque my_deque(3);

    cout << "Operation: insertLast(1)  -> Expected: true | Output: " << (my_deque.insertLast(1) ? "true" : "false") << "\n";
    cout << "Operation: insertLast(2)  -> Expected: true | Output: " << (my_deque.insertLast(2) ? "true" : "false") << "\n";
    cout << "Operation: insertFront(3) -> Expected: true | Output: " << (my_deque.insertFront(3) ? "true" : "false") << "\n";
    cout << "Operation: insertFront(4) -> Expected: false| Output: " << (my_deque.insertFront(4) ? "true" : "false") << "\n";

    cout << "Operation: getRear()      -> Expected: 2    | Output: " << my_deque.getRear() << "\n";

    cout << "Operation: isFull()       -> Expected: true | Output: " << (my_deque.isFull() ? "true" : "false") << "\n";

    cout << "Operation: deleteLast()   -> Expected: true | Output: " << (my_deque.deleteLast() ? "true" : "false") << "\n";

    cout << "Operation: insertFront(4) -> Expected: true | Output: " << (my_deque.insertFront(4) ? "true" : "false") << "\n";

    cout << "Operation: getFront()     -> Expected: 4    | Output: " << my_deque.getFront() << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 641 (User's Adapted Deque Architecture) ---\n\n";

    try {
        runUserArchitectureTest("Example 1 (Basic Circular Deque Validation)");
    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}