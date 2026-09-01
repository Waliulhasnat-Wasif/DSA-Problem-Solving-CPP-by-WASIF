#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout;
using std::string;
using std::unordered_set;
using std::vector;

struct ListNode {
  int val;
  ListNode* next;

  explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
  ListNode* detectCycleHash(ListNode* head) {
    unordered_set<ListNode*> visitedNodes;
    ListNode* current = head;

    while (current != nullptr) {
      if (visitedNodes.find(current) != visitedNodes.end()) {
        return current;
      }

      visitedNodes.insert(current);
      current = current->next;
    }

    return nullptr;
  }

  ListNode* detectCycle(ListNode* head) {
    ListNode* slow = head;
    ListNode* fast = head;

    while (fast != nullptr && fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next->next;

      if (slow == fast) {
        slow = head;

        while (slow != fast) {
          slow = slow->next;
          fast = fast->next;
        }

        return slow;
      }
    }

    return nullptr;
  }
};

ListNode* createLinkedListWithCycle(const vector<int>& nums, int pos) {
  if (nums.empty()) {
    return nullptr;
  }

  ListNode* head = new ListNode(nums[0]);
  ListNode* current = head;
  ListNode* cycleNode = (pos == 0) ? head : nullptr;

  for (size_t i = 1; i < nums.size(); i++) {
    current->next = new ListNode(nums[i]);
    current = current->next;

    if (static_cast<int>(i) == pos) {
      cycleNode = current;
    }
  }

  if (pos != -1) {
    current->next = cycleNode;
  }

  return head;
}

ListNode* getNodeAtIndex(ListNode* head, int index) {
  if (index < 0) {
    return nullptr;
  }

  ListNode* current = head;
  for (int i = 0; i < index && current != nullptr; i++) {
    current = current->next;
  }

  return current;
}

void printLinkedListSafely(ListNode* head) {
  if (head == nullptr) {
    cout << "[]\n";
    return;
  }

  unordered_set<ListNode*> visited;

  cout << "[";

  while (head != nullptr) {
    if (visited.count(head)) {
      cout << "... (Cycle loops back to node with val: " << head->val << ")]\n";
      return;
    }

    visited.insert(head);
    cout << head->val;

    if (head->next != nullptr) {
      cout << " -> ";
    }

    head = head->next;
  }

  cout << "]\n";
}

void deleteLinkedListSafely(ListNode* head) {
  unordered_set<ListNode*> visited;
  ListNode* current = head;

  while (current != nullptr) {
    if (visited.count(current)) {
      break;
    }

    visited.insert(current);

    ListNode* toDelete = current;
    current = current->next;
    delete toDelete;
  }
}

void runTestCase(const vector<int>& input, int pos, const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* head = createLinkedListWithCycle(input, pos);
  ListNode* expectedNode = getNodeAtIndex(head, pos);

  cout << "Original List : ";
  printLinkedListSafely(head);

  ListNode* resultNode = sol.detectCycle(head);

  string expectedStr =
      (expectedNode != nullptr) ? std::to_string(expectedNode->val) : "null";
  string resultStr =
      (resultNode != nullptr) ? std::to_string(resultNode->val) : "null";

  cout << "Input sizes: " << input.size() << ", Pos: " << pos << "\n";
  cout << "Output points to val: " << resultStr
       << " | Expected points to val: " << expectedStr;

  if (resultNode == expectedNode) {
    cout << " -> [PASS]\n";
  } else {
    cout << " -> [FAIL]\n";
  }

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(head);
}

int main() {
  cout << "--- Testing LeetCode 142: Linked List Cycle II ---\n\n";

  runTestCase({3, 2, 0, -4}, 1, "Example 1 (Cycle at index 1)");
  runTestCase({1, 2}, 0, "Example 2 (Cycle at index 0)");
  runTestCase({1}, -1, "Example 3 (No cycle)");
  runTestCase({}, -1, "Edge Case (Empty List)");
  runTestCase({10, 20, 30, 40, 50}, 4, "Edge Case (Cycle at the very end)");

  return 0;
}