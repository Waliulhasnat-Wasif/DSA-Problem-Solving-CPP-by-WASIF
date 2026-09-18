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

class RecentCounterQueue {
public:
    RecentCounterQueue() = default;
    ~RecentCounterQueue() = default;

    int ping(int t) {
        q_.push(t);
        while (!q_.empty() && q_.front() < t - 3000) {
            q_.pop();
        }
        return q_.size();
    }

private:
    queue<int> q_;
};

class RecentCounter {
public:
    RecentCounter() : arr_(nullptr), capacity_(3005), current_size_(0), head_(0), tail_(0) {
        arr_ = new int[capacity_];
    }

    ~RecentCounter() {
        delete[] arr_;
    }

    RecentCounter(const RecentCounter& other)
        : arr_(nullptr), capacity_(other.capacity_), current_size_(other.current_size_), head_(0), tail_(other.current_size_) {
        arr_ = new int[capacity_];
        for (size_t i = 0; i < current_size_; i++) {
            arr_[i] = other.arr_[(other.head_ + i) % other.capacity_];
        }
    }

    RecentCounter(RecentCounter&& other) noexcept
        : arr_(other.arr_), capacity_(other.capacity_), current_size_(other.current_size_), head_(other.head_), tail_(other.tail_) {
        other.arr_ = nullptr;
        other.capacity_ = 0;
        other.current_size_ = 0;
        other.head_ = 0;
        other.tail_ = 0;
    }

    friend void swap(RecentCounter& first, RecentCounter& second) noexcept {
        using std::swap;
        swap(first.arr_, second.arr_);
        swap(first.capacity_, second.capacity_);
        swap(first.current_size_, second.current_size_);
        swap(first.head_, second.head_);
        swap(first.tail_, second.tail_);
    }

    // 6. Unified Assignment Operator
    RecentCounter& operator=(RecentCounter temp) {
        swap(*this, temp);
        return *this;
    }

    int ping(int t) {
        arr_[tail_] = t;
        tail_ = (tail_ + 1) % capacity_;
        current_size_++;

        while (current_size_ > 0 && arr_[head_] < t - 3000) {
            head_ = (head_ + 1) % capacity_;
            current_size_--;
        }

        return current_size_;
    }

private:
    int* arr_;
    size_t capacity_;
    size_t current_size_;
    size_t head_;
    size_t tail_;
};

void runComparativeTest(const string& test_name, const int pings[], int size, const int expected[]) {
    cout << "Test Case: " << test_name << "\n";

    RecentCounter rc_opt;
    RecentCounterQueue rc_base;

    for (int i = 0; i < size; ++i) {
        int t = pings[i];
        int out_opt = rc_opt.ping(t);
        int out_base = rc_base.ping(t);
        int exp = expected[i];

        cout << "ping(" << t << ") -> Opt: " << out_opt << " | Base: " << out_base << " | Expected: " << exp << " "
             << (out_opt == exp && out_base == exp ? "[PASS]" : "[FAIL]") << "\n";
    }
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LeetCode 933: Number of Recent Calls ---\n\n";

    try {
        // 1. Standard Case (LeetCode Example)
        int pings1[] = {1, 100, 3001, 3002};
        int exp1[] = {1, 2, 3, 3};
        runComparativeTest("Example 1 (Standard Sequential Pings)", pings1, 4, exp1);

        // 2. Edge Case: Boundary condition (Exactly 3000ms apart)
        // If ping is at 1, and next is 3001, range is [1, 3001]. Both count.
        int pings2[] = {1, 3001};
        int exp2[] = {1, 2};
        runComparativeTest("Edge Case 1 (Boundary: Exactly 3000ms apart)", pings2, 2, exp2);

        // 3. Edge Case: Complete Flush (Long pause between pings)
        // A huge time jump should clear all previous elements.
        int pings3[] = {10, 20, 30, 10000, 10001};
        int exp3[] = {1, 2, 3, 1, 2};
        runComparativeTest("Edge Case 2 (Complete Flush via Time Jump)", pings3, 5, exp3);

        // 4. Edge Case: Dense Traffic (Pings every millisecond)
        int pings4[] = {1000, 1001, 1002, 1003};
        int exp4[] = {1, 2, 3, 4};
        runComparativeTest("Edge Case 3 (Dense Traffic)", pings4, 4, exp4);

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}