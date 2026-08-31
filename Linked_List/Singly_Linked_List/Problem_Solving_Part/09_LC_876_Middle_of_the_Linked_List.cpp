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
  ListNode* middleNode(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next->next;
    }

    return slow;
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

void runTestCase(const vector<int>& input, const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedList(input);
  cout << "Original List  : ";
  printLinkedList(head);

  ListNode* middle = sol.middleNode(head);

  cout << "Middle Onward  : ";
  printLinkedList(middle);
  cout << string(50, '-') << "\n";

  deleteLinkedList(head);
}

int main() {
  cout << "--- Testing LeetCode 876: Middle of the Linked List ---\n\n";

  runTestCase({1, 2, 3, 4, 5}, "Example 1 (Odd Length)");
  runTestCase({1, 2, 3, 4, 5, 6}, "Example 2 (Even Length)");
  runTestCase({42}, "Edge Case 1 (Single Node)");
  runTestCase({10, 20}, "Edge Case 2 (Two Nodes)");

  return 0;
}