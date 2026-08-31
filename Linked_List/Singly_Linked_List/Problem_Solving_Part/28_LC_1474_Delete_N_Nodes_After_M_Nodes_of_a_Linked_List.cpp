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
  ListNode* deleteNodesSafe(ListNode* head, int m, int n) {
    if (head == nullptr) {
      return nullptr;
    }

    vector<int> values;
    ListNode* curr = head;
    while (curr != nullptr) {
      values.push_back(curr->val);
      curr = curr->next;
    }

    ListNode dummy(-1);
    ListNode* tail = &dummy;
    int i = 0;
    int size = static_cast<int>(values.size());

    while (i < size) {
      for (int keep = 0; keep < m && i < size; keep++, i++) {
        tail->next = new ListNode(values[i]);
        tail = tail->next;
      }

      for (int skip = 0; skip < n && i < size; skip++, i++) {
      }
    }

    curr = head;
    while (curr != nullptr) {
      ListNode* toDelete = curr;
      curr = curr->next;
      delete toDelete;
    }

    return dummy.next;
  }

  ListNode* deleteNodes(ListNode* head, int m, int n) {
    ListNode* curr = head;

    while (curr != nullptr) {
      for (int i = 1; i < m && curr != nullptr; i++) {
        curr = curr->next;
      }

      if (curr == nullptr) {
        break;
      }

      ListNode* nextNode = curr->next;
      for (int i = 0; i < n && nextNode != nullptr; i++) {
        ListNode* toDelete = nextNode;
        nextNode = nextNode->next;

        delete toDelete;
      }

      curr->next = nextNode;
      curr = nextNode;
    }

    return head;
  }
};

ListNode* createLinkedList(const vector<int>& nums) {
  if (nums.empty()) {
    return nullptr;
  }

  ListNode* head = new ListNode(nums[0]);
  ListNode* current = head;

  for (size_t i = 1; i < nums.size(); i++) {
    current->next = new ListNode(nums[i]);
    current = current->next;
  }

  return head;
}

void printLinkedList(ListNode* head) {
  if (head == nullptr) {
    cout << "[]\n";
    return;
  }

  cout << "[";

  while (head != nullptr) {
    cout << head->val;

    if (head->next != nullptr) {
      cout << ", ";
    }

    head = head->next;
  }

  cout << "]\n";
}

void deleteLinkedListSafely(ListNode* head) {
  ListNode* current = head;

  while (current != nullptr) {
    ListNode* toDelete = current;
    current = current->next;
    delete toDelete;
  }
}

void runTestCase(const vector<int>& input, int m, int n,
                 const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedList(input);

  cout << "Input List : ";
  printLinkedList(head);
  cout << "m = " << m << ", n = " << n << "\n";

  ListNode* resultHead = sol.deleteNodes(head, m, n);

  cout << "Output     : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 1474: Delete N Nodes After M Nodes ---\n\n";

  runTestCase({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13}, 2, 3,
              "Example 1 (Standard Case)");
  runTestCase({1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11}, 1, 3,
              "Example 2 (Keep 1, delete 3)");
  runTestCase({1, 2, 3, 4}, 2, 10,
              "Edge Case (Delete more nodes than remaining)");

  return 0;
}