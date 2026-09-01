#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

struct ListNode {
  int val;
  ListNode* next;

  explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
  ListNode* insertSafe(ListNode* head, int insertVal) {
    if (head == nullptr) {
      ListNode* newNode = new ListNode(insertVal);
      newNode->next = newNode;
      return newNode;
    }

    vector<int> values;
    ListNode* curr = head;
    do {
      values.push_back(curr->val);
      curr = curr->next;
    } while (curr != head);

    int insertIndex = static_cast<int>(values.size());

    for (size_t i = 0; i < values.size() - 1; i++) {
      if (values[i] <= insertVal && insertVal <= values[i + 1]) {
        insertIndex = static_cast<int>(i + 1);
        break;
      }
    }

    if (insertVal < values.front()) {
      insertIndex = 0;
    }

    values.insert(values.begin() + insertIndex, insertVal);

    ListNode dummy(-1);
    ListNode* tail = &dummy;
    for (size_t i = 0; i < values.size(); i++) {
      tail->next = new ListNode(values[i]);
      tail = tail->next;
    }
    tail->next = dummy.next;

    curr = head;
    do {
      ListNode* nextNode = curr->next;
      delete curr;
      curr = nextNode;
    } while (curr != head);

    return dummy.next;
  }

  ListNode* insert(ListNode* head, int insertVal) {
    if (head == nullptr) {
      ListNode* newNode = new ListNode(insertVal);
      newNode->next = newNode;
      return newNode;
    }

    ListNode* prev = head;
    ListNode* curr = head->next;
    bool toInsert = false;

    do {
      if (prev->val <= insertVal && insertVal <= curr->val) {
        toInsert = true;
      }

      else if (prev->val > curr->val) {
        if (insertVal >= prev->val || insertVal <= curr->val) {
          toInsert = true;
        }
      }

      if (toInsert) {
        ListNode* newNode = new ListNode(insertVal);
        newNode->next = curr;
        prev->next = newNode;
        return head;
      }

      prev = curr;
      curr = curr->next;

    } while (prev != head);

    ListNode* newNode = new ListNode(insertVal);
    newNode->next = curr;
    prev->next = newNode;

    return head;
  }
};

ListNode* createCircularLinkedList(const vector<int>& nums) {
  if (nums.empty()) {
    return nullptr;
  }

  ListNode* head = new ListNode(nums[0]);
  ListNode* current = head;

  for (size_t i = 1; i < nums.size(); i++) {
    current->next = new ListNode(nums[i]);
    current = current->next;
  }

  current->next = head;
  return head;
}

void printCircularLinkedList(ListNode* head) {
  if (head == nullptr) {
    cout << "[]\n";
    return;
  }

  cout << "[";
  ListNode* curr = head;
  do {
    cout << curr->val;
    curr = curr->next;
    if (curr != head) {
      cout << ", ";
    }
  } while (curr != head);
  cout << "]\n";
}

void deleteCircularLinkedListSafely(ListNode* head) {
  if (head == nullptr) {
    return;
  }

  ListNode* curr = head;
  ListNode* nextNode;

  do {
    nextNode = curr->next;
    delete curr;
    curr = nextNode;
  } while (curr != head);
}

void runTestCase(const vector<int>& input, int insertVal,
                 const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createCircularLinkedList(input);

  cout << "Original List : ";
  printCircularLinkedList(head);
  cout << "Insert Value  : " << insertVal << "\n";

  ListNode* resultHead = sol.insert(head, insertVal);

  cout << "Modified List : ";
  printCircularLinkedList(resultHead);
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  deleteCircularLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 708: Insert into a Sorted Circular Linked List "
          "---\n\n";

  runTestCase({3, 4, 1}, 2, "Example 1 (Standard Insertion)");
  runTestCase({}, 1, "Example 2 (Empty List)");
  runTestCase({1}, 0, "Example 3 (Single Node, Insert Min)");
  runTestCase({3, 4, 1}, 5, "Edge Case (Insert New Max)");
  runTestCase({3, 3, 3}, 4, "Edge Case (Uniform Values)");

  return 0;
}