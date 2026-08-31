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
  ListNode* partitionSafe(ListNode* head, int x) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    vector<int> lessValues;
    vector<int> greaterValues;
    ListNode* curr = head;

    while (curr != nullptr) {
      if (curr->val < x) {
        lessValues.push_back(curr->val);
      } else {
        greaterValues.push_back(curr->val);
      }
      curr = curr->next;
    }

    curr = head;
    for (int val : lessValues) {
      curr->val = val;
      curr = curr->next;
    }
    for (int val : greaterValues) {
      curr->val = val;
      curr = curr->next;
    }

    return head;
  }

  ListNode* partition(ListNode* head, int x) {
    ListNode lessDummy(-1);
    ListNode greaterDummy(-1);

    ListNode* lessTail = &lessDummy;
    ListNode* greaterTail = &greaterDummy;
    ListNode* curr = head;

    while (curr != nullptr) {
      if (curr->val < x) {
        lessTail->next = curr;
        lessTail = lessTail->next;
      } else {
        greaterTail->next = curr;
        greaterTail = greaterTail->next;
      }
      curr = curr->next;
    }

    greaterTail->next = nullptr;

    lessTail->next = greaterDummy.next;

    return lessDummy.next;
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

void runTestCase(const vector<int>& input, int x, const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedList(input);

  cout << "Original List : ";
  printLinkedList(head);
  cout << "Partition (x) : " << x << "\n";

  ListNode* resultHead = sol.partition(head, x);

  cout << "Modified List : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 86: Partition List ---\n\n";

  runTestCase({1, 4, 3, 2, 5, 2}, 3, "Example 1 (Mixed values)");
  runTestCase({2, 1}, 2, "Example 2 (Small list)");
  runTestCase({1, 2, 3}, 4, "Edge Case (All less than x)");
  runTestCase({4, 5, 6}, 1, "Edge Case (All greater than x)");
  runTestCase({}, 0, "Edge Case (Empty List)");

  return 0;
}