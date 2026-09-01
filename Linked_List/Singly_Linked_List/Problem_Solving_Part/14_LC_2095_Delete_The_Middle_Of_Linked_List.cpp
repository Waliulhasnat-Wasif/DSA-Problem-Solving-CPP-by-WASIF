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
  ListNode* deleteMiddleTwoPasses(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      delete head;
      return nullptr;
    }

    int length = 0;
    ListNode* current = head;

    while (current != nullptr) {
      length++;
      current = current->next;
    }

    int midIndex = length / 2;
    current = head;

    for (int i = 1; i < midIndex; i++) {
      current = current->next;
    }

    ListNode* toDelete = current->next;
    current->next = current->next->next;
    delete toDelete;

    return head;
  }

  ListNode* deleteMiddle(ListNode* head) {
    ListNode dummy(-1);
    dummy.next = head;

    ListNode* slow = &dummy;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next->next;
    }

    ListNode* toDelete = slow->next;

    if (toDelete != nullptr) {
      slow->next = toDelete->next;
      delete toDelete;
    }

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

void runTestCase(const vector<int>& input, const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedList(input);

  cout << "Original List : ";
  printLinkedList(head);

  ListNode* resultHead = sol.deleteMiddle(head);

  cout << "Modified List : ";
  printLinkedList(resultHead);

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 2095: Delete the Middle Node of a Linked List "
          "---\n\n";

  runTestCase({1, 3, 4, 7, 1, 2, 6}, "Example 1 (Odd Length)");
  runTestCase({1, 2, 3, 4}, "Example 2 (Even Length)");
  runTestCase({2, 1}, "Example 3 (Two nodes)");
  runTestCase({1}, "Edge Case (Single node)");
  runTestCase({}, "Edge Case (Empty List)");

  return 0;
}