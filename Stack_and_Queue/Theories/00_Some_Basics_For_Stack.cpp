#include <iostream>
#include <stdexcept>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;
using std::underflow_error;

class Stack {
 private:
  int* arr;
  int capacity;
  int topIndex;

  void resize() {
    int oldCapacity = capacity;
    capacity = capacity * 2;

    cout << "\n  [Resize Triggered] -> Amortized Complexity Strategy" << endl;
    cout << "  -> Doubling capacity from " << oldCapacity << " to " << capacity
         << endl;

    // Strong Exception Safety....Allocate new memory before deleting old memory
    int* tempArr = new int[capacity];
    cout << "  -> [Safe Zone Step] Temporary large memory allocated at: "
         << tempArr << endl;

    for (int i = 0; i <= topIndex; i++) {
      tempArr[i] = arr[i];
    }

    cout << "  -> Deleting old small memory at: " << arr << endl;
    delete[] arr;

    arr = tempArr;
    cout << "  -> Resize complete. Amortized cost paid. Next pushes will be "
            "O(1).\n"
         << endl;
  }

 public:
  // 1. Constructor with Member Initializer List and Fail-Fast Guard
  explicit Stack(int size = 2) : capacity(size), topIndex(-1) {
    if (size <= 0) {
      throw invalid_argument("Stack capacity must be greater than zero.");
    }
    arr = new int[capacity];
    cout << "[Constructor] Memory allocated at Heap: " << arr
         << " (Capacity: " << capacity << ")" << endl;
  }

  // 2. Destructor
  ~Stack() {
    cout << "[Destructor] Freeing memory blocks from: " << arr << endl;
    delete[] arr;
  }

  // 3. Copy Constructor
  Stack(const Stack& other)
      : capacity(other.capacity), topIndex(other.topIndex) {
    arr = new int[capacity];
    cout << "[Copy Constructor] Deep Copying! New independent memory created "
            "at: "
         << arr << endl;

    for (int i = 0; i <= topIndex; i++) {
      arr[i] = other.arr[i];
    }
  }

  // 4. Copy Assignment Operator
  Stack& operator=(const Stack& other) {
    cout << "[Copy Assignment Operator] Triggered!" << endl;

    if (this == &other) {
      cout << "  -> Self-assignment detected (s1 = s1). Skipping to prevent "
              "self-destruction."
           << endl;
      return *this;
    }

    cout << "  -> [Strong Exception Safety] Allocating new memory BEFORE "
            "deleting old memory..."
         << endl;
    int* tempArr = new int[other.capacity];
    cout << "  -> New memory safely ready at: " << tempArr << endl;

    for (int i = 0; i <= other.topIndex; i++) {
      tempArr[i] = other.arr[i];
    }

    cout << "  -> Safe Zone reached. Now deleting old memory at: " << this->arr
         << endl;
    delete[] arr;

    arr = tempArr;
    capacity = other.capacity;
    topIndex = other.topIndex;

    return *this;
  }

  // Push Operation
  void push(int val) {
    if (topIndex == capacity - 1) {
      resize();
    }
    arr[++topIndex] = val;
    cout << "  Pushed: " << val << " at index " << topIndex
         << " (Size: " << (topIndex + 1) << "/" << capacity << ")" << endl;
  }

  // Pop Operation
  void pop() {
    if (isEmpty()) {
      throw underflow_error("Stack Underflow! Cannot pop from an empty stack.");
    }
    cout << "  Popped: " << arr[topIndex] << " from index " << topIndex << endl;
    topIndex--;
  }

  // Top Operation (Const Correct)
  int top() const {
    if (isEmpty()) {
      throw underflow_error("Stack is empty! Cannot read top element.");
    }
    return arr[topIndex];
  }

  // Diagnostic Print (Const Correct)
  void printTop() const {
    cout << "  [Const Method - printTop() called]" << endl;
    if (!isEmpty()) {
      cout << "  -> Top element is: " << arr[topIndex] << endl;
    } else {
      cout << "  -> Stack is empty!" << endl;
    }
  }

  // Size (Const Correct)
  int size() const { return (topIndex + 1); }

  // Empty Check (Const Correct)
  bool isEmpty() const { return topIndex == -1; }
};

// Global Diagnostic Function
void printStackDiagnostics(const Stack& s) {
  cout << "\n=== [Global Function] Diagnostics Triggered with a CONST "
          "Reference Stack ==="
       << endl;

  // We are only allowed to call const functions here since we have received a
  // const reference of the Stack object.
  cout << "Diagnostic - Stack Size: " << s.size() << endl;
  s.printTop();

  cout << "=== Diagnostics Complete ===\n" << endl;
}

int main() {
  try {
    cout << "===== Step 1: The Rule of Three & Amortized Complexity ====="
         << endl;
    Stack s1(2);
    s1.push(10);
    s1.push(20);
    s1.push(30);  // Amortized Resize will trigger

    cout << "\n===== Step 2: Copy Constructor (Birth of s2 from s1) ====="
         << endl;
    Stack s2 = s1;  // Copy Constructor

    cout << "\n===== Step 3: Copy Assignment & Strong Exception Safety ====="
         << endl;
    Stack s3(5);
    s3.push(500);
    s3 = s1;  // Copy Assignment Operator

    cout << "\n===== Step 4: Const Correctness Testing =====" << endl;
    printStackDiagnostics(s1);

    cout << "===== EXTRA TEST: Pop & Underflow Test =====" << endl;
    s1.pop();
    s1.printTop();

    cout << "\n===== PROGRAM ENDING: Destructors will clean everything up ====="
         << endl;
  } catch (const exception& e) {
    cerr << "Exception caught: " << e.what() << endl;
  }

  return 0;
}