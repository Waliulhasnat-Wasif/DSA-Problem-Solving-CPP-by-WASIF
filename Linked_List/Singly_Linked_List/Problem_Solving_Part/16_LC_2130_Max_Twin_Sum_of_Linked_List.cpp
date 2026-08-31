#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::max;
using std::string;
using std::vector;

struct ListNode {
  int val;
  ListNode* next;

  explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
 private:
  ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr != nullptr) {
      ListNode* nextTemp = curr->next;
      curr->next = prev;
      prev = curr;
      curr = nextTemp;
    }

    return prev;
  }

 public:
  int pairSumSafe(ListNode* head) {
    vector<int> vals;
    ListNode* current = head;

    while (current != nullptr) {
      vals.push_back(current->val);
      current = current->next;
    }

    int maxSum = 0;
    int left = 0;
    int right = static_cast<int>(vals.size()) - 1;

    while (left < right) {
      maxSum = max(maxSum, vals[left] + vals[right]);
      left++;
      right--;
    }

    return maxSum;
  }

  int pairSum(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next->next;
    }

    ListNode* secondHalfHead = reverseList(slow);

    int maxSum = 0;
    ListNode* p1 = head;
    ListNode* p2 = secondHalfHead;

    while (p2 != nullptr) {
      maxSum = max(maxSum, p1->val + p2->val);
      p1 = p1->next;
      p2 = p2->next;
    }

    reverseList(secondHalfHead);

    return maxSum;
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

void runTestCase(const vector<int>& input, int expected,
                 const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedList(input);

  cout << "Original List : ";
  printLinkedList(head);

  int result = sol.pairSum(head);

  cout << "Output: " << result << " | Expected: " << expected;

  if (result == expected) {
    cout << " -> [PASS]\n";
  } else {
    cout << " -> [FAIL]\n";
  }

  cout << "Restored List : ";
  printLinkedList(head);

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(head);
}

int main() {
  cout
      << "--- Testing LeetCode 2130: Maximum Twin Sum of a Linked List ---\n\n";

  runTestCase({5, 4, 5, 1}, 9, "Example 1 (Standard)");
  runTestCase({4, 2, 2, 3}, 7, "Example 2 (Different Max)");
  runTestCase({1, 100000}, 100001, "Example 3 (Two nodes)");

  return 0;
}