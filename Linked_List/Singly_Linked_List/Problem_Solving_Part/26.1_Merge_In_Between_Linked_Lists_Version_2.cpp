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
  ListNode* mergeInBetweenV2(ListNode* list1, int a, int b, ListNode* list2) {
    if (list1 == nullptr) {
      return list2;
    }

    ListNode dummy(-1);
    dummy.next = list1;

    ListNode* preA = &dummy;

    for (int i = 0; i < a; i++) {
      if (preA->next == nullptr) {
        break;
      }
      preA = preA->next;
    }

    ListNode* curr = preA->next;
    for (int i = 0; i < b - a + 1; i++) {
      if (curr == nullptr) {
        break;
      }
      ListNode* toDelete = curr;
      curr = curr->next;
      delete toDelete;
    }

    ListNode* postB = curr;

    if (list2 == nullptr) {
      preA->next = postB;
      return dummy.next;
    }

    ListNode* list2Tail = list2;
    while (list2Tail->next != nullptr) {
      list2Tail = list2Tail->next;
    }

    preA->next = list2;
    list2Tail->next = postB;

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

void runTestCase(const vector<int>& input1, int a, int b,
                 const vector<int>& input2, const string& testName) {
  Solution sol;

  cout << "Test Case: " << testName << "\n";

  ListNode* list1 = createLinkedList(input1);
  ListNode* list2 = createLinkedList(input2);

  cout << "List 1 : ";
  printLinkedList(list1);
  cout << "List 2 : ";
  printLinkedList(list2);
  cout << "a = " << a << ", b = " << b << "\n";

  ListNode* resultHead = sol.mergeInBetweenV2(list1, a, b, list2);

  cout << "Output : ";
  printLinkedList(resultHead);
  cout << " -> [PASS]\n";

  cout << string(50, '-') << "\n";

  deleteLinkedListSafely(resultHead);
}

int main() {
  cout << "--- Testing LeetCode 1669: Merge In Between (V3 - Bulletproof) "
          "---\n\n";

  runTestCase({0, 1, 2, 3}, 1, 2, {},
              "Edge Case: list2 is Empty (Should just delete nodes)");
  runTestCase({}, 1, 2, {99, 100},
              "Edge Case: list1 is Empty (Should return list2)");
  runTestCase({}, 0, 5, {}, "Edge Case: Both lists are Empty");
  runTestCase({0, 1, 2}, 10, 15, {99}, "Edge Case: a and b are Out of Bounds");
  runTestCase({0, 1, 2, 3, 4, 5}, 3, 100, {99},
              "Edge Case: b is Out of Bounds");

  return 0;
}