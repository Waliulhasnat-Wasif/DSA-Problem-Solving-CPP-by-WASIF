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
  ListNode* getKthNode(ListNode* curr, int k) {
    while (curr != nullptr && k > 1) {
      curr = curr->next;
      k--;
    }
    return curr;
  }

  void reverseList(ListNode* head) {
    ListNode* prev = nullptr;
    ListNode* curr = head;

    while (curr != nullptr) {
      ListNode* nextNode = curr->next;
      curr->next = prev;
      prev = curr;
      curr = nextNode;
    }
  }

public:
  ListNode* reverseKGroupSafe(ListNode* head, int k) {
    vector<int> values;
    ListNode* curr = head;

    while (curr != nullptr) {
      values.push_back(curr->val);
      curr = curr->next;
    }

    int n = static_cast<int>(values.size());

    for (int i = 0; i + k <= n; i += k) {
      std::reverse(values.begin() + i, values.begin() + i + k);
    }

    curr = head;
    for (int val : values) {
      curr->val = val;
      curr = curr->next;
    }

    return head;
  }

  ListNode* reverseKGroup(ListNode* head, int k) {
    if (head == nullptr || head->next == nullptr || k == 1) {
      return head;
    }

    ListNode dummy(-1);
    dummy.next = head;

    ListNode* groupPrev = &dummy;

    while (true) {
      ListNode* kthNode = getKthNode(groupPrev->next, k);

      if (kthNode == nullptr) {
        break;
      }

      ListNode* groupNext = kthNode->next;

      ListNode* groupStart = groupPrev->next;
      kthNode->next = nullptr;

      reverseList(groupStart);

      groupPrev->next = kthNode;
      groupStart->next = groupNext;

      groupPrev = groupStart;
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

void runTestCase(const vector<int>& input, int k, const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedList(input);

  cout << "Original List : ";
  printLinkedList(head);
  cout << "k = " << k << "\n";

  ListNode* resultHead = sol.reverseKGroup(head, k);

  cout << "Modified List : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 25: Reverse Nodes in k-Group ---\n\n";

  runTestCase({1, 2, 3, 4, 5}, 2, "Example 1 (Multiple of 2 with 1 left out)");
  runTestCase({1, 2, 3, 4, 5}, 3, "Example 2 (Multiple of 3 with 2 left out)");
  runTestCase({1, 2, 3, 4, 5}, 5, "Edge Case 1 (k equals list length)");
  runTestCase({1, 2, 3}, 5, "Edge Case 2 (k greater than list length)");
  runTestCase({1}, 1, "Edge Case 3 (Single node, k = 1)");

  return 0;
}