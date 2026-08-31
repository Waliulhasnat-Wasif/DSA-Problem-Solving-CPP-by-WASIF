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
  ListNode* removeNthFromEndSafe(ListNode* head, int n) {
    ListNode dummy(-1);
    dummy.next = head;

    int length = 0;
    ListNode* current = head;

    while (current != nullptr) {
      length++;
      current = current->next;
    }

    int stepsToTake = length - n;
    current = &dummy;

    for (int i = 0; i < stepsToTake; i++) {
      current = current->next;
    }

    ListNode* toDelete = current->next;
    current->next = toDelete->next;
    delete toDelete;

    return dummy.next;
  }

  ListNode* removeNthFromEnd(ListNode* head, int n) {
    ListNode dummy(-1);
    dummy.next = head;

    ListNode* slow = &dummy;
    ListNode* fast = &dummy;

    for (int i = 0; i < n; i++) {
      fast = fast->next;
    }

    while (fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next;
    }

    ListNode* toDelete = slow->next;
    slow->next = toDelete->next;

    delete toDelete;

    return dummy.next;
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

void runTestCase(const vector<int>& input, int n, const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedList(input);

  cout << "Original List : ";
  printLinkedList(head);
  cout << "n = " << n << "\n";

  ListNode* resultHead = sol.removeNthFromEnd(head, n);
  // ListNode *resultHead = sol.removeNthFromEndSafe(head, n);

  cout << "Modified List : ";
  printLinkedList(resultHead);

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 19: Remove Nth Node From End of List ---\n\n";

  runTestCase({1, 2, 3, 4, 5}, 3, "Example 1 (Remove middle element)");
  runTestCase({1}, 1, "Example 2 (Remove the only element)");
  runTestCase({1, 2}, 1, "Example 3 (Remove the last element)");
  runTestCase({1, 2, 3, 4, 5}, 5, "Edge Case (Remove the head element)");

  return 0;
}