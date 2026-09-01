#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

struct ListNode {
  int val;
  ListNode* next;

  ListNode() : val(0), next(nullptr) {}
  explicit ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
public:
  ListNode* removeElements(ListNode* head, int val) {
    ListNode dummy(-1);
    dummy.next = head;

    ListNode* current = &dummy;

    while (current->next != nullptr) {
      if (current->next->val == val) {
        ListNode* toDelete = current->next;
        current->next = toDelete->next;
        delete toDelete;
      } else {
        current = current->next;
      }
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

void deleteLinkedList(ListNode* head) {
  while (head != nullptr) {
    ListNode* temp = head;
    head = head->next;
    delete temp;
  }
}

void runTestCase(const vector<int>& input, int targetVal,
                 const string& testName) {
  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedList(input);
  cout << "Original List : ";
  printLinkedList(head);
  cout << "Target Value  : " << targetVal << "\n";

  Solution sol;
  ListNode* resultHead = sol.removeElements(head, targetVal);
  cout << "Modified List : ";
  printLinkedList(resultHead);
  cout << string(50, '-') << "\n";

  deleteLinkedList(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 203: Remove Linked List Elements ---\n\n";

  runTestCase({1, 2, 6, 3, 4, 5, 6}, 6, "Example 1 (Multiple targets)");
  runTestCase({}, 1, "Example 2 (Empty list)");
  runTestCase({7, 7, 7, 7}, 7, "Example 3 (All targets)");
  runTestCase({2, 2, 3, 4}, 2, "Target at head");

  return 0;
}