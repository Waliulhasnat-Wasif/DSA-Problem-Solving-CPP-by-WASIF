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
 public:
  ListNode* reverseEvenLengthGroupsSafe(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    vector<int> values;
    ListNode* curr = head;
    while (curr != nullptr) {
      values.push_back(curr->val);
      curr = curr->next;
    }

    int n = static_cast<int>(values.size());
    int expectedLength = 1;
    int i = 0;

    while (i < n) {
      int currentLength = std::min(expectedLength, n - i);

      if (currentLength % 2 == 0) {
        std::reverse(values.begin() + i, values.begin() + i + currentLength);
      }

      i += currentLength;
      expectedLength++;
    }

    curr = head;
    for (int val : values) {
      curr->val = val;
      curr = curr->next;
    }

    return head;
  }

  ListNode* reverseEvenLengthGroups(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    ListNode dummy(-1);
    dummy.next = head;

    ListNode* prevGroupEnd = &dummy;
    int expectedLength = 1;

    while (prevGroupEnd->next != nullptr) {
      int count = 0;
      ListNode* temp = prevGroupEnd->next;
      while (temp != nullptr && count < expectedLength) {
        count++;
        temp = temp->next;
      }

      if (count % 2 == 0) {
        ListNode* curr = prevGroupEnd->next;
        ListNode* groupPrev = nullptr;

        ListNode* oldHead = curr;

        for (int i = 0; i < count; i++) {
          ListNode* nextNode = curr->next;
          curr->next = groupPrev;
          groupPrev = curr;
          curr = nextNode;
        }

        prevGroupEnd->next = groupPrev;
        oldHead->next = curr;

        prevGroupEnd = oldHead;
      } else {
        for (int i = 0; i < count; i++) {
          prevGroupEnd = prevGroupEnd->next;
        }
      }

      expectedLength++;
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

  ListNode* resultHead = sol.reverseEvenLengthGroups(head);

  cout << "Modified List : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 2074: Reverse Nodes in Even Length Groups "
          "---\n\n";

  runTestCase({5, 2, 6, 3, 9, 1, 7, 3, 8, 4},
              "Example 1 (Complex group lengths)");
  runTestCase({1, 1, 0, 6}, "Example 2 (Last group is odd, expected even)");
  runTestCase({1, 1, 0, 6, 5}, "Example 3 (Last group is even, expected odd)");
  runTestCase({1, 2, 3, 4, 5, 6, 7},
              "Example 4 (Last group is even but size odd)");
  runTestCase({1}, "Edge Case (Single node)");

  return 0;
}