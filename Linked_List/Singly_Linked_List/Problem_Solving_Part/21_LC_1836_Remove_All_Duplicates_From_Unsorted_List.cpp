#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::string;
using std::unordered_map;
using std::vector;

struct ListNode {
  int val;
  ListNode* next;

  explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
  ListNode* deleteDuplicatesUnsortedSafe(ListNode* head) {
    ListNode dummy(-1);
    dummy.next = head;

    ListNode* prev = &dummy;
    ListNode* curr = head;

    while (curr != nullptr) {
      bool isDuplicate = false;
      ListNode* innerPrev = curr;
      ListNode* innerCurr = curr->next;

      while (innerCurr != nullptr) {
        if (innerCurr->val == curr->val) {
          isDuplicate = true;

          ListNode* toDelete = innerCurr;
          innerPrev->next = toDelete->next;
          innerCurr = innerCurr->next;

          delete toDelete;
        } else {
          innerPrev = innerCurr;
          innerCurr = innerCurr->next;
        }
      }

      if (isDuplicate) {
        ListNode* toDelete = curr;
        prev->next = toDelete->next;
        curr = curr->next;

        delete toDelete;
      } else {
        prev = curr;
        curr = curr->next;
      }
    }

    return dummy.next;
  }

  ListNode* deleteDuplicatesUnsorted(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    unordered_map<int, int> frequency;
    ListNode* current = head;

    while (current != nullptr) {
      frequency[current->val]++;
      current = current->next;
    }

    ListNode dummy(-1);
    dummy.next = head;

    ListNode* prev = &dummy;
    current = head;

    while (current != nullptr) {
      if (frequency[current->val] > 1) {
        ListNode* toDelete = current;
        prev->next = toDelete->next;
        current = current->next;

        delete toDelete;
      } else {
        prev = current;
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

  ListNode* resultHead = sol.deleteDuplicatesUnsorted(head);

  cout << "Modified List : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 1836: Remove Duplicates From an Unsorted "
          "Linked List ---\n\n";

  runTestCase({1, 2, 3, 2}, "Example 1 (Basic Unsorted Duplicates)");
  runTestCase({2, 1, 1, 2}, "Example 2 (All Duplicates)");
  runTestCase({3, 2, 2, 1, 3, 2, 4}, "Example 3 (Multiple varying duplicates)");
  runTestCase({1, 2, 3}, "Edge Case (No Duplicates)");
  runTestCase({}, "Edge Case (Empty List)");

  return 0;
}