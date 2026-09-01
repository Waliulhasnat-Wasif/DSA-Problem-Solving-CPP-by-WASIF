#include <iostream>
#include <stdexcept>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::move;
using std::underflow_error;

struct Node {
  int data;
  Node* next;

  explicit Node(int val) : data(val), next(nullptr) {}
};

class Queue {
private:
  Node* frontNode;
  Node* rearNode;
  size_t currentSize;

  void clear() {
    while (frontNode != nullptr) {
      Node* temp = frontNode;
      frontNode = frontNode->next;
      delete temp;
    }
    rearNode = nullptr;
    currentSize = 0;
  }

public:
  Queue() : frontNode(nullptr), rearNode(nullptr), currentSize(0) {}

  ~Queue() {
    clear();
  }

  Queue(const Queue& other)
      : frontNode(nullptr), rearNode(nullptr), currentSize(0) {
    if (other.frontNode == nullptr) {
      return;
    }

    frontNode = new Node(other.frontNode->data);
    rearNode = frontNode;
    Node* currentOther = other.frontNode->next;
    currentSize = 1;

    while (currentOther != nullptr) {
      rearNode->next = new Node(currentOther->data);
      rearNode = rearNode->next;
      currentOther = currentOther->next;
      currentSize++;
    }
  }

  Queue(Queue&& other) noexcept
      : frontNode(other.frontNode),
        rearNode(other.rearNode),
        currentSize(other.currentSize) {
    other.frontNode = nullptr;
    other.rearNode = nullptr;
    other.currentSize = 0;
  }

  friend void swap(Queue& first, Queue& second) noexcept {
    using std::swap;
    swap(first.frontNode, second.frontNode);
    swap(first.rearNode, second.rearNode);
    swap(first.currentSize, second.currentSize);
  }

  /*
  friend void swap(Queue &first, Queue &second) noexcept
  {
      // 1. frontNode Manual Swap
      Node *tempFront = first.frontNode;
      first.frontNode = second.frontNode;
      second.frontNode = tempFront;

      // 2. rearNode Manual Swap
      Node *tempRear = first.rearNode;
      first.rearNode = second.rearNode;
      second.rearNode = tempRear;

      // 3. currentSize Manual Swap
      size_t tempSize = first.currentSize;
      first.currentSize = second.currentSize;
      second.currentSize = tempSize;
  }
  */

  // Unified Assignment Operator (Canonical Copy-and-Swap)
  // Pass-by-value resolves both Copy Assignment AND Move Assignment
  // dynamically.
  Queue& operator=(Queue temp) {
    swap(*this, temp);
    return *this;
  }

  // Push / Enqueue Operation -> Strict O(1) Time
  void push(int val) {
    Node* newNode = new Node(val);
    if (empty()) {
      frontNode = rearNode = newNode;
    } else {
      rearNode->next = newNode;
      rearNode = newNode;
    }
    currentSize++;
  }

  // Pop / Dequeue Operation -> Strict O(1) Time
  void pop() {
    if (empty()) {
      throw underflow_error(
          "Error: Queue Underflow! Cannot pop from an empty queue.");
    }

    Node* temp = frontNode;
    frontNode = frontNode->next;
    delete temp;
    currentSize--;

    if (frontNode == nullptr) {
      rearNode = nullptr;
    }
  }

  int front() const {
    if (empty()) {
      throw underflow_error("Error: Queue is empty! No front element.");
    }
    return frontNode->data;
  }

  int back() const {
    if (empty()) {
      throw underflow_error("Error: Queue is empty! No back element.");
    }
    return rearNode->data;
  }

  bool empty() const {
    return currentSize == 0;
  }

  size_t size() const {
    return currentSize;
  }
};

int main() {
  try {
    Queue q1;
    q1.push(10);
    q1.push(20);
    q1.push(30);

    cout << "--- q1 Queue (Linked List) ---" << endl;
    cout << "q1 Front: " << q1.front() << endl;  // 10
    cout << "q1 Back: " << q1.back() << endl;    // 30
    cout << "q1 Size: " << q1.size() << endl;    // 3

    // Copy Constructor Test
    Queue q2 = q1;
    cout << "\n--- q2 Queue (Copied from q1) ---" << endl;
    cout << "q2 Front: " << q2.front() << endl;  // 10

    // FIFO Verification (Pop affects front, back remains same)
    q1.pop();
    cout << "\nAfter popping q1 (FIFO Principle):" << endl;
    cout << "q1 New Front: " << q1.front() << endl;      // 20
    cout << "q1 Back remains: " << q1.back() << endl;    // 30
    cout << "q2 Front remains: " << q2.front() << endl;  // 10 (Unaffected)

    // Unified Assignment Operator Test
    Queue q3;
    q3.push(999);
    q3 = q1;
    cout << "\n--- q3 Queue (Assigned from q1) ---" << endl;
    cout << "q3 Front: " << q3.front() << endl;  // 20

    // Move Constructor Test
    Queue q4 = move(q2);
    cout << "\n--- q4 Queue (Moved from q2) ---" << endl;
    cout << "q4 Front: " << q4.front() << endl;  // 10
    cout << "q2 Size after move: " << q2.size()
         << " (Empty: " << (q2.empty() ? "Yes" : "No") << ")" << endl;

    // Move Assignment Operator Test
    Queue q5;
    q5 = move(q3);
    cout << "\n--- q5 Queue (Move-Assigned from q3) ---" << endl;
    cout << "q5 Front: " << q5.front() << endl;  // 20
    cout << "q3 Size after move: " << q3.size()
         << " (Empty: " << (q3.empty() ? "Yes" : "No") << ")" << endl;

    // Critical Edge Case Validation: Pop until empty
    cout << "\n--- Edge Case: Emptying the Queue ---" << endl;
    q5.pop();  // Removes 20, front becomes 30
    q5.pop();  // Removes 30, queue should be fully empty
    cout << "q5 is fully empty: " << (q5.empty() ? "Yes" : "No") << endl;

    // Error Handling Test
    cout << "\n--- Underflow Exception Testing ---" << endl;
    Queue emptyQueue;
    emptyQueue.pop();  // Triggers the underflow exception
  } catch (const exception& e) {
    cerr << "[Exception Caught] " << e.what() << endl;
  }

  return 0;
}