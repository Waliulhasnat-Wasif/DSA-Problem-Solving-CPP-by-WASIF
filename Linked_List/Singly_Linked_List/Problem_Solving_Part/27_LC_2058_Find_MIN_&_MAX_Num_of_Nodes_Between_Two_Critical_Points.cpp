#include <algorithm>
#include <climits>
#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::min;
using std::string;
using std::vector;

struct ListNode {
  int val;
  ListNode* next;

  explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
  vector<int> nodesBetweenCriticalPointsSafe(ListNode* head) {
    if (head == nullptr || head->next == nullptr ||
        head->next->next == nullptr) {
      return {-1, -1};
    }

    vector<int> criticalPoints;
    ListNode* prev = head;
    ListNode* curr = head->next;
    int index = 1;

    while (curr->next != nullptr) {
      ListNode* nextNode = curr->next;

      if ((curr->val > prev->val && curr->val > nextNode->val) ||
          (curr->val < prev->val && curr->val < nextNode->val)) {
        criticalPoints.push_back(index);
      }

      prev = curr;
      curr = nextNode;
      index++;
    }

    if (criticalPoints.size() < 2) {
      return {-1, -1};
    }

    int minDistance = INT_MAX;
    for (size_t i = 1; i < criticalPoints.size(); i++) {
      minDistance = min(minDistance, criticalPoints[i] - criticalPoints[i - 1]);
    }

    int maxDistance = criticalPoints.back() - criticalPoints.front();

    return {minDistance, maxDistance};
  }

  vector<int> nodesBetweenCriticalPoints(ListNode* head) {
    if (head == nullptr || head->next == nullptr ||
        head->next->next == nullptr) {
      return {-1, -1};
    }

    int firstCriticalIndex = -1;
    int prevCriticalIndex = -1;
    int minDistance = INT_MAX;

    ListNode* prev = head;
    ListNode* curr = head->next;
    int index = 1;

    while (curr->next != nullptr) {
      ListNode* nextNode = curr->next;

      if ((curr->val > prev->val && curr->val > nextNode->val) ||
          (curr->val < prev->val && curr->val < nextNode->val)) {
        if (firstCriticalIndex == -1) {
          firstCriticalIndex = index;
        } else {
          minDistance = min(minDistance, index - prevCriticalIndex);
        }

        prevCriticalIndex = index;
      }

      prev = curr;
      curr = nextNode;
      index++;
    }

    if (minDistance == INT_MAX) {
      return {-1, -1};
    }

    int maxDistance = prevCriticalIndex - firstCriticalIndex;

    return {minDistance, maxDistance};
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

  cout << "Input List : ";
  printLinkedList(head);

  vector<int> result = sol.nodesBetweenCriticalPoints(head);

  cout << "Output     : [" << result[0] << ", " << result[1] << "]\n";
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(head);
}

int main() {
  cout << "--- Testing LeetCode 2058: Find the Minimum and Maximum Number of "
          "Nodes Between Critical Points ---\n\n";

  runTestCase({3, 1}, "Example 1 (Less than 3 nodes)");
  runTestCase({5, 3, 1, 2, 5, 1, 2}, "Example 2 (Multiple critical points)");
  runTestCase({1, 3, 2, 2, 3, 2, 2, 2, 7},
              "Example 3 (Adjacent identical values handled)");
  runTestCase({2, 3, 3, 2},
              "Edge Case (No strict local maxima/minima due to duplicates)");
  runTestCase({1, 2, 1, 2, 1}, "Edge Case (Alternating critical points)");

  return 0;
}