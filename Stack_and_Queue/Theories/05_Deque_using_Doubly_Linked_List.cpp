#include <iostream>
#include <stdexcept>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::move;
using std::underflow_error;

// Doubly Linked List Node
struct Node {
  int data;
  Node* next;
  Node* prev;  // Additional pointer for backward traversal

  explicit Node(int val) : data(val), next(nullptr), prev(nullptr) {}
};

class Deque {
private:
  Node* head;
  Node* tail;
  size_t currentSize;

  void clear() {
    while (head != nullptr) {
      Node* temp = head;
      head = head->next;
      delete temp;
    }
    tail = nullptr;
    currentSize = 0;
  }

public:
  // 1. Constructor
  Deque() : head(nullptr), tail(nullptr), currentSize(0) {}

  // 2. Destructor
  ~Deque() {
    clear();
  }

  // 3. Copy Constructor (Deep Copy for Doubly Linked List)
  Deque(const Deque& other) : head(nullptr), tail(nullptr), currentSize(0) {
    if (other.head == nullptr) {
      return;
    }

    head = new Node(other.head->data);
    tail = head;
    Node* currentOther = other.head->next;
    currentSize = 1;

    while (currentOther != nullptr) {
      Node* newNode = new Node(currentOther->data);
      tail->next = newNode;
      newNode->prev = tail;  // Maintaining the reverse link
      tail = newNode;
      currentOther = currentOther->next;
      currentSize++;
    }
  }

  // 4. Move Constructor (O(1) Ownership Transfer)
  Deque(Deque&& other) noexcept
      : head(other.head), tail(other.tail), currentSize(other.currentSize) {
    other.head = nullptr;
    other.tail = nullptr;
    other.currentSize = 0;
  }

  // 5. Custom Swap Function (ADL-enabled)
  friend void swap(Deque& first, Deque& second) noexcept {
    using std::swap;
    swap(first.head, second.head);
    swap(first.tail, second.tail);
    swap(first.currentSize, second.currentSize);
  }

  // 6. Unified Assignment Operator (Canonical Copy-and-Swap)
  Deque& operator=(Deque temp) {
    swap(*this, temp);
    return *this;
  }

  // Push at Front -> Strict O(1) Time
  void pushFront(int val) {
    Node* newNode = new Node(val);
    if (empty()) {
      head = tail = newNode;
    } else {
      newNode->next = head;
      head->prev = newNode;
      head = newNode;
    }
    currentSize++;
  }

  // Push at Back -> Strict O(1) Time
  void pushBack(int val) {
    Node* newNode = new Node(val);
    if (empty()) {
      head = tail = newNode;
    } else {
      tail->next = newNode;
      newNode->prev = tail;
      tail = newNode;
    }
    currentSize++;
  }

  // Pop from Front -> Strict O(1) Time
  void popFront() {
    if (empty()) {
      throw underflow_error(
          "Error: Deque Underflow! Cannot popFront from an empty deque.");
    }

    Node* temp = head;
    head = head->next;

    if (head != nullptr) {
      head->prev = nullptr;  // Severing the backward link to the deleted node
    } else {
      tail = nullptr;  // If deque becomes empty after pop
    }

    delete temp;
    currentSize--;
  }

  // Pop from Back -> Strict O(1) Time (Only possible efficiently due to 'prev'
  // pointer)
  void popBack() {
    if (empty()) {
      throw underflow_error(
          "Error: Deque Underflow! Cannot popBack from an empty deque.");
    }

    Node* temp = tail;
    tail = tail->prev;

    if (tail != nullptr) {
      tail->next = nullptr;  // Severing the forward link to the deleted node
    } else {
      head = nullptr;  // If deque becomes empty after pop
    }

    delete temp;
    currentSize--;
  }

  // Front Operation -> Strict O(1) Time
  int front() const {
    if (empty()) {
      throw underflow_error("Error: Deque is empty! No front element.");
    }
    return head->data;
  }

  // Back Operation -> Strict O(1) Time
  int back() const {
    if (empty()) {
      throw underflow_error("Error: Deque is empty! No back element.");
    }
    return tail->data;
  }

  // Empty Check -> Strict O(1) Time
  bool empty() const {
    return currentSize == 0;
  }

  // Size Check -> Strict O(1) Time
  size_t size() const {
    return currentSize;
  }
};

int main() {
  try {
    Deque dq1;

    // Testing Push Operations
    dq1.pushBack(10);  // [10]
    dq1.pushBack(20);  // [10, 20]
    dq1.pushFront(5);  // [5, 10, 20]
    dq1.pushFront(1);  // [1, 5, 10, 20]

    cout << "--- dq1 Deque (Doubly Linked List) ---" << endl;
    cout << "dq1 Front: " << dq1.front() << endl;  // 1
    cout << "dq1 Back: " << dq1.back() << endl;    // 20
    cout << "dq1 Size: " << dq1.size() << endl;    // 4

    // Copy Constructor Test
    Deque dq2 = dq1;
    cout << "\n--- dq2 Deque (Copied from dq1) ---" << endl;
    cout << "dq2 Front: " << dq2.front() << ", dq2 Back: " << dq2.back()
         << endl;  // 1, 20

    // Testing Pop Operations
    cout << "\n--- Pop Operations on dq1 ---" << endl;
    dq1.popFront();  // Removes 1 -> [5, 10, 20]
    cout << "After popFront(), New Front: " << dq1.front() << endl;  // 5

    dq1.popBack();  // Removes 20 -> [5, 10]
    cout << "After popBack(), New Back: " << dq1.back() << endl;  // 10
    cout << "dq1 Size is now: " << dq1.size() << endl;            // 2

    // Verify Deep Copy Isolation
    cout << "dq2 Front remains: " << dq2.front() << " (Unaffected)"
         << endl;  // 1

    // Move Semantics Test
    Deque dq3 = move(dq2);
    cout << "\n--- dq3 Deque (Moved from dq2) ---" << endl;
    cout << "dq3 Front: " << dq3.front() << endl;                      // 1
    cout << "dq2 is empty: " << (dq2.empty() ? "Yes" : "No") << endl;  // Yes

    // Underflow Exception Test
    cout << "\n--- Edge Case Testing ---" << endl;
    Deque emptyDeque;
    emptyDeque.popBack();  // Triggers the underflow exception
  } catch (const exception& e) {
    cerr << "[Exception Caught] " << e.what() << endl;
  }

  return 0;
}