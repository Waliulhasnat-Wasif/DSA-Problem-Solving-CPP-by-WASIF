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
  ListNode* addTwoNumbersSafe(ListNode* l1, ListNode* l2) {
    ListNode dummy(-1);
    ListNode* curr = &dummy;
    int carry = 0;

    while (l1 != nullptr || l2 != nullptr || carry > 0) {
      int sum = carry;
      if (l1 != nullptr) {
        sum += l1->val;
        l1 = l1->next;
      }
      if (l2 != nullptr) {
        sum += l2->val;
        l2 = l2->next;
      }

      carry = sum / 10;
      curr->next = new ListNode(sum % 10);
      curr = curr->next;
    }

    return dummy.next;
  }

  ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
    ListNode* curr = l1;
    ListNode* prev = nullptr;
    int carry = 0;

    while (curr != nullptr || l2 != nullptr || carry > 0) {
      int sum = carry;

      if (curr != nullptr) {
        sum += curr->val;
      }
      if (l2 != nullptr) {
        sum += l2->val;
        l2 = l2->next;
      }

      carry = sum / 10;

      if (curr != nullptr) {
        curr->val = sum % 10;
        prev = curr;
        curr = curr->next;
      } else {
        prev->next = new ListNode(sum % 10);
        prev = prev->next;
      }
    }

    return l1;
  }

  ListNode* addTwoNumbersStrictO1(ListNode* l1, ListNode* l2) {
    ListNode* p1 = l1;
    ListNode* p2 = l2;
    ListNode* prev = nullptr;
    int carry = 0;

    while (p1 != nullptr || p2 != nullptr || carry > 0) {
      if (p1 == nullptr && p2 != nullptr) {
        prev->next = p2;
        p1 = p2;
        p2 = nullptr;
      }

      int sum = carry;
      if (p1 != nullptr) sum += p1->val;
      if (p2 != nullptr) sum += p2->val;

      carry = sum / 10;

      if (p1 != nullptr) {
        p1->val = sum % 10;
        prev = p1;
        p1 = p1->next;
      } else {
        prev->next = new ListNode(sum % 10);
        prev = prev->next;
      }

      if (p2 != nullptr) {
        p2 = p2->next;
      }
    }

    return l1;
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

void runTestCase(const vector<int>& input1, const vector<int>& input2,
                 const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* l1 = createLinkedList(input1);
  ListNode* l2 = createLinkedList(input2);

  cout << "List 1 (l1) : ";
  printLinkedList(l1);
  cout << "List 2 (l2) : ";
  printLinkedList(l2);

  ListNode* resultHead = sol.addTwoNumbers(l1, l2);
  // ListNode *resultHead = sol.addTwoNumbersStrictO1(l1, l2);

  cout << "Sum List    : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";
  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
  deleteLinkedListSafely(l2);
}

int main() {
  cout << "--- Testing LeetCode 2: Add Two Numbers ---\n\n";

  runTestCase({2, 4, 3}, {5, 6, 4}, "Example 1 (Same length, no carry at end)");
  runTestCase({0}, {0}, "Example 2 (Zeroes)");
  runTestCase({9, 9, 9, 9, 9, 9, 9}, {9, 9, 9, 9},
              "Example 3 (Different lengths with cascading carry)");
  runTestCase({1}, {9, 9, 9}, "Edge Case 1 (l1 much shorter than l2)");
  runTestCase({5}, {5},
              "Edge Case 2 (Single digit resulting in carry expansion)");

  return 0;
}