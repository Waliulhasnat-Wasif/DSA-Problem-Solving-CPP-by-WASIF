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
private:
  int* arr;
  size_t capacity;
  size_t currentSize;
  size_t frontIndex;
  size_t rearIndex;

  void resize() {
    size_t newCapacity = capacity * 2;
    int* tempArr = new int[newCapacity];

    // Unwrapping the circular queue into a straight/linear array format
    for (size_t i = 0; i < currentSize; i++) {
      tempArr[i] = arr[(frontIndex + i) % capacity];
    }

    // Safe cleanup and pointer reassignment
    delete[] arr;
    arr = tempArr;
    capacity = newCapacity;

    // Resetting indices after unwrapping
    frontIndex = 0;
    rearIndex = currentSize;
  }

public:
  // 1. Constructor (with Member Initializer List and Fail-Fast Guard)
  explicit Queue(size_t initialCapacity = 2)
      : capacity(initialCapacity), currentSize(0), frontIndex(0), rearIndex(0) {
    if (initialCapacity == 0) {
      throw invalid_argument("Queue capacity must be greater than zero.");
    }
    arr = new int[capacity];
  }

  // 2. Destructor
  ~Queue() {
    delete[] arr;
  }

  // 3. Copy Constructor (Deep Copy + Unwrapping Defragmentation)
  Queue(const Queue& other)
      : capacity(other.capacity),
        currentSize(other.currentSize),
        frontIndex(0),
        rearIndex(other.currentSize) {
    arr = new int[capacity];

    // Unwrap the source queue into a linear format in the new memory block
    for (size_t i = 0; i < currentSize; i++) {
      arr[i] = other.arr[(other.frontIndex + i) % other.capacity];
    }
  }

  // 4. Move Constructor (O(1) Ownership Transfer)
  Queue(Queue&& other) noexcept
      : arr(other.arr),
        capacity(other.capacity),
        currentSize(other.currentSize),
        frontIndex(other.frontIndex),
        rearIndex(other.rearIndex) {
    // Nullify the source object to leave it destructible and empty
    other.arr = nullptr;
    other.capacity = 0;
    other.currentSize = 0;
    other.frontIndex = 0;
    other.rearIndex = 0;
  }

  // 5. Custom Swap Function (ADL-enabled)
  friend void swap(Queue& first, Queue& second) noexcept {
    using std::swap;
    swap(first.arr, second.arr);
    swap(first.capacity, second.capacity);
    swap(first.currentSize, second.currentSize);
    swap(first.frontIndex, second.frontIndex);
    swap(first.rearIndex, second.rearIndex);
  }

  // 6. Unified Assignment Operator (Canonical Copy-and-Swap)
  Queue& operator=(Queue temp) {
    swap(*this, temp);
    return *this;
  }

  // Push / Enqueue Operation -> Amortized O(1) Time
  void push(int val) {
    if (currentSize == capacity) {
      resize();
    }
    arr[rearIndex] = val;
    rearIndex = (rearIndex + 1) % capacity;  // Circular wrap-around
    currentSize++;
  }

  // Pop / Dequeue Operation -> Strict O(1) Time
  void pop() {
    if (empty()) {
      throw underflow_error(
          "Error: Queue Underflow! Cannot pop from an empty queue.");
    }
    frontIndex = (frontIndex + 1) % capacity;  // Circular wrap-around
    currentSize--;
  }

  // Front Operation -> Strict O(1) Time
  int front() const {
    if (empty()) {
      throw underflow_error("Error: Queue is empty! No front element.");
    }
    return arr[frontIndex];
  }

  // Back Operation -> Strict O(1) Time
  int back() const {
    if (empty()) {
      throw underflow_error("Error: Queue is empty! No back element.");
    }
    // Calculate the logical last element's index considering circular
    // wrap-around
    size_t lastIndex = (rearIndex == 0) ? capacity - 1 : rearIndex - 1;
    return arr[lastIndex];
  }

  // Empty Check -> Strict O(1) Time
  bool empty() const {
    return currentSize == 0;
  }

  // Size Check -> Strict O(1) Time
  size_t size() const {
    return currentSize;
  }

  // Capacity Check
  size_t getCapacity() const {
    return capacity;
  }
};

// ==========================================
// Test Driver
// ==========================================
int main() {
  try {
    Queue q1;  // Initial capacity is 2
    q1.push(10);
    q1.push(20);
    q1.push(30);  // Triggers resize (capacity becomes 4)

    cout << "--- q1 Queue (Dynamic Circular Array) ---" << endl;
    cout << "q1 Front: " << q1.front() << endl;           // 10
    cout << "q1 Back: " << q1.back() << endl;             // 30
    cout << "q1 Size: " << q1.size() << endl;             // 3
    cout << "q1 Capacity: " << q1.getCapacity() << endl;  // 4

    // Circular Wrap-around Test
    cout << "\n--- Circular Wrap-around Verification ---" << endl;
    q1.pop();     // Removes 10, frontIndex moves
    q1.push(40);  // Fills the empty slot at the beginning (wraps around)
    q1.push(50);  // Triggers resize, unwraps beautifully
    cout << "After Wrap-around and Resize:" << endl;
    cout << "q1 Front: " << q1.front() << endl;           // 20
    cout << "q1 Back: " << q1.back() << endl;             // 50
    cout << "q1 Capacity: " << q1.getCapacity() << endl;  // 8

    // Copy Constructor Test
    Queue q2 = q1;
    cout << "\n--- q2 Queue (Copied from q1) ---" << endl;
    cout << "q2 Front: " << q2.front() << endl;  // 20

    // Unified Assignment Operator Test
    Queue q3;
    q3.push(999);
    q3 = q1;
    cout << "\n--- q3 Queue (Assigned from q1) ---" << endl;
    cout << "q3 Front: " << q3.front() << endl;  // 20

    // Move Constructor Test
    Queue q4 = move(q2);
    cout << "\n--- q4 Queue (Moved from q2) ---" << endl;
    cout << "q4 Front: " << q4.front() << endl;  // 20
    cout << "q2 Size after move: " << q2.size()
         << " (Empty: " << (q2.empty() ? "Yes" : "No") << ")" << endl;

    // Move Assignment Operator Test
    Queue q5;
    q5 = move(q3);
    cout << "\n--- q5 Queue (Move-Assigned from q3) ---" << endl;
    cout << "q5 Front: " << q5.front() << endl;  // 20
    cout << "q3 Size after move: " << q3.size()
         << " (Empty: " << (q3.empty() ? "Yes" : "No") << ")" << endl;

    // Error Handling Test
    cout << "\n--- Underflow Exception Testing ---" << endl;
    Queue emptyQueue;
    emptyQueue.pop();  // Triggers the underflow exception
  } catch (const exception& e) {
    cerr << "[Exception Caught] " << e.what() << endl;
  }

  return 0;
}