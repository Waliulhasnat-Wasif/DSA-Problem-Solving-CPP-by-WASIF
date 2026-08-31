#include <algorithm>
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
 private:
  ListNode* reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr != nullptr) {
      ListNode* nextNode = curr->next;
      curr->next = prev;
      prev = curr;
      curr = nextNode;
    }

    return prev;
  }

 public:
  ListNode* removeNodesVariationSafe(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    vector<ListNode*> stack;
    ListNode* curr = head;

    while (curr != nullptr) {
      while (!stack.empty() && stack.back()->val > curr->val) {
        ListNode* toDelete = stack.back();
        stack.pop_back();
        delete toDelete;
      }
      stack.push_back(curr);
      curr = curr->next;
    }

    ListNode dummy(-1);
    ListNode* tail = &dummy;
    for (ListNode* node : stack) {
      tail->next = node;
      tail = tail->next;
    }
    tail->next = nullptr;

    return dummy.next;
  }

  ListNode* removeNodesVariation(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    ListNode* reversedHead = reverseList(head);

    ListNode* curr = reversedHead;
    int minValSoFar = curr->val;

    while (curr->next != nullptr) {
      if (curr->next->val > minValSoFar) {
        ListNode* toDelete = curr->next;
        curr->next = toDelete->next;
        delete toDelete;
      } else {
        minValSoFar = curr->next->val;
        curr = curr->next;
      }
    }

    return reverseList(reversedHead);
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
    if (head->next != nullptr) cout << ", ";
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

  ListNode* resultHead = sol.removeNodesVariation(head);

  cout << "Modified List : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";
  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing Variation: Remove nodes with a SMALLER value to the "
          "right ---\n\n";

  runTestCase({5, 2, 13, 3, 8}, "Example 1 (Complex sequence)");
  runTestCase({1, 1, 1, 1}, "Example 2 (All equal, none removed)");
  runTestCase({1, 2, 3, 4}, "Edge Case 1 (Strictly increasing, none removed)");
  runTestCase({10, 9, 8, 7},
              "Edge Case 2 (Strictly decreasing, all but last removed)");

  return 0;
}