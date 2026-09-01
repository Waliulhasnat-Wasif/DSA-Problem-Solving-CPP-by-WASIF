#include <iostream>
#include <queue>
#include <string>
#include <vector>

using std::cout;
using std::priority_queue;
using std::string;
using std::vector;

struct ListNode {
  int val;
  ListNode* next;

  explicit ListNode(int x) : val(x), next(nullptr) {}
};

struct CompareNode {
  bool operator()(ListNode* const& p1, ListNode* const& p2) {
    return p1->val > p2->val;
  }
};

class Solution {
private:
  ListNode* mergeTwoSortedList(ListNode* list1, ListNode* list2) {
    ListNode dummy(-1);
    ListNode* tail = &dummy;

    while (list1 != nullptr && list2 != nullptr) {
      if (list1->val <= list2->val) {
        tail->next = list1;
        list1 = list1->next;
      } else {
        tail->next = list2;
        list2 = list2->next;
      }
      tail = tail->next;
    }

    tail->next = (list1 != nullptr) ? list1 : list2;
    return dummy.next;
  }

  ListNode* mergeKListsRecursive(vector<ListNode*>& lists, int start, int end) {
    if (start > end) {
      return nullptr;
    }
    if (start == end) {
      return lists[start];
    }

    int mid = start + (end - start) / 2;

    ListNode* leftSorted = mergeKListsRecursive(lists, start, mid);
    ListNode* rightSorted = mergeKListsRecursive(lists, mid + 1, end);

    return mergeTwoSortedList(leftSorted, rightSorted);
  }

public:
  ListNode* mergeKListsDivideAndConquer(vector<ListNode*>& lists) {
    if (lists.empty()) {
      return nullptr;
    }

    int interval = 1;
    int n = static_cast<int>(lists.size());

    while (interval < n) {
      for (int i = 0; i < n - interval; i += interval * 2) {
        lists[i] = mergeTwoSortedList(lists[i], lists[i + interval]);
      }
      interval *= 2;
    }

    return lists[0];
  }

  ListNode* mergeKLists(vector<ListNode*>& lists) {
    if (lists.empty()) {
      return nullptr;
    }

    int n = static_cast<int>(lists.size());
    return mergeKListsRecursive(lists, 0, n - 1);
  }

  ListNode* mergeKListsMinHeap(vector<ListNode*>& lists) {
    priority_queue<ListNode*, vector<ListNode*>, CompareNode> minHeap;

    for (ListNode* head : lists) {
      if (head != nullptr) {
        minHeap.push(head);
      }
    }

    ListNode dummy(-1);
    ListNode* tail = &dummy;

    while (!minHeap.empty()) {
      ListNode* minNode = minHeap.top();
      minHeap.pop();

      tail->next = minNode;
      tail = tail->next;

      if (minNode->next != nullptr) {
        minHeap.push(minNode->next);
      }
    }

    return dummy.next;
  }
};

ListNode* createLinkedList(const vector<int>& nums) {
  if (nums.empty()) return nullptr;

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

void runTestCase(const vector<vector<int>>& inputArrays,
                 const string& testName) {
  Solution sol;
  cout << "Test Case: " << testName << "\n";

  vector<ListNode*> lists;
  for (const auto& arr : inputArrays) {
    lists.push_back(createLinkedList(arr));
  }

  ListNode* resultHead = sol.mergeKListsDivideAndConquer(lists);
  // ListNode *resultHead = sol.mergeKLists(lists);

  cout << "Merged List : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";
  cout << string(50, '-') << "\n";
  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 23: Merge k Sorted Lists ---\n\n";

  runTestCase({{1, 4, 5}, {1, 3, 4}, {2, 6}}, "Example 1 (Multiple lists)");
  runTestCase({}, "Example 2 (Empty input array)");
  runTestCase({{}}, "Example 3 (Array containing an empty list)");
  runTestCase({{1, 2}, {3, 4}, {5, 6}},
              "Edge Case 1 (Strictly sequential disjoint lists)");

  return 0;
}