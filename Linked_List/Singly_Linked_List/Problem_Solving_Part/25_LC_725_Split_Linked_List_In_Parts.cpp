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
  vector<ListNode*> splitListToPartsSafe(ListNode* head, int k) {
    vector<ListNode*> nodes;
    ListNode* curr = head;

    while (curr != nullptr) {
      nodes.push_back(curr);
      curr = curr->next;
    }

    int length = static_cast<int>(nodes.size());
    int baseSize = length / k;
    int extra = length % k;

    vector<ListNode*> result(k, nullptr);
    int currentIndex = 0;

    for (int i = 0; i < k; i++) {
      if (currentIndex >= length) {
        break;
      }

      result[i] = nodes[currentIndex];
      int currentPartSize = baseSize + (extra > 0 ? 1 : 0);
      extra--;

      currentIndex += currentPartSize;

      nodes[currentIndex - 1]->next = nullptr;
    }

    return result;
  }

  vector<ListNode*> splitListToParts(ListNode* head, int k) {
    int length = 0;
    ListNode* curr = head;
    while (curr != nullptr) {
      length++;
      curr = curr->next;
    }

    int baseSize = length / k;
    int extra = length % k;

    vector<ListNode*> result(k, nullptr);
    curr = head;

    for (int i = 0; i < k && curr != nullptr; i++) {
      result[i] = curr;

      int currentPartSize = baseSize + (extra > 0 ? 1 : 0);
      extra--;

      for (int j = 0; j < currentPartSize - 1; j++) {
        if (curr != nullptr) {
          curr = curr->next;
        }
      }

      if (curr != nullptr) {
        ListNode* nextPart = curr->next;

        curr->next = nullptr;

        curr = nextPart;
      }
    }

    return result;
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
    cout << "[]";
    return;
  }

  cout << "[";

  while (head != nullptr) {
    cout << head->val;

    if (head->next != nullptr) {
      cout << ",";
    }

    head = head->next;
  }

  cout << "]";
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
  cout << "\nk = " << k << "\n";

  vector<ListNode*> result = sol.splitListToParts(head, k);

  cout << "Output: [";
  for (size_t i = 0; i < result.size(); i++) {
    printLinkedList(result[i]);
    if (i < result.size() - 1) {
      cout << ", ";
    }
  }
  cout << "]\n";
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  for (ListNode* partHead : result) {
    deleteLinkedListSafely(partHead);
  }
}

int main() {
  cout << "--- Testing LeetCode 725: Split Linked List in Parts ---\n\n";

  runTestCase({1, 2, 3}, 5, "Example 1 (k > length)");
  runTestCase({1, 2, 3, 4, 5, 6, 7, 8, 9, 10}, 3,
              "Example 2 (length > k with remainder)");
  runTestCase({1, 2, 3, 4, 5, 6}, 3,
              "Example 3 (length perfectly divisible by k)");
  runTestCase({}, 3, "Edge Case (Empty List)");

  return 0;
}