#include <iostream>
#include <string>
#include <vector>

using std ::cout;
using std ::string;
using std ::vector;

struct ListNode {
  int val;
  ListNode* next;

  ListNode() : val(0), next(nullptr) {}
  explicit ListNode(int x) : val(x), next(nullptr) {}
  ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
 public:
  ListNode* mergeNodes(ListNode* head) {
    ListNode dummy(-1);
    ListNode* writer = &dummy;
    ListNode* reader = head;

    while (reader != nullptr) {
      while (reader != nullptr && reader->val == 0) {
        ListNode* zeroNode = reader;
        reader = reader->next;
        delete zeroNode;
      }

      if (reader == nullptr) break;

      ListNode* sumNode = reader;
      int sum = 0;

      while (reader != nullptr && reader->val != 0) {
        sum += reader->val;
        ListNode* temp = reader;
        reader = reader->next;

        if (temp != sumNode) {
          delete temp;
        }
      }

      sumNode->val = sum;
      writer->next = sumNode;
      writer = sumNode;
    }

    writer->next = nullptr;

    return dummy.next;
  }
};

ListNode* createLinkedList(const vector<int>& nums) {
  if (nums.empty()) return nullptr;

  ListNode* head = new ListNode(nums[0]);
  ListNode* curr = head;

  for (size_t i = 1; i < nums.size(); i++) {
    curr->next = new ListNode(nums[i]);
    curr = curr->next;
  }

  return head;
}

void printLinkedList(ListNode* head) {
  while (head != nullptr) {
    cout << head->val << " -> ";
    head = head->next;
  }

  cout << "NULL" << "\n";
}

void deleteLinkedList(ListNode* head) {
  while (head != nullptr) {
    ListNode* temp = head;
    head = head->next;
    delete temp;
  }
}

void runTestCase(const vector<int>& input, const string& testName) {
  cout << "Test Case: " << testName << "\n";
  ListNode* head = createLinkedList(input);
  cout << "Original List: ";
  printLinkedList(head);

  Solution sol;
  ListNode* resultHead = sol.mergeNodes(head);
  cout << "Modified List: ";
  printLinkedList(resultHead);
  cout << string(40, '-') << "\n";

  deleteLinkedList(resultHead);
}

int main() {
  cout << "--- Testing special version of LeetCode 2181: Merge Nodes in "
          "Between Zeros ---\n\n";

  runTestCase({0, 1, 2, 0, 4, 5, 6, 0, 8, 9, 0}, "Test 01: Standard");
  runTestCase({0, 0, 0, 1, 2, 3, 0, 3, 0, 0},
              "Test 02: Multiple Zeros at the beginning & the end");
  runTestCase(
      {0, 0, 1, 2, 3, 0, 0, 5, 0, 0, 0, 6, 0, 2, 6, 8, 0, 0},
      "Test 03: Multiple zeros at the beginning, in between & at the end");
  runTestCase({1, 2, 3, 0, 0, 5, 0, 0, 0, 6, 0, 2, 6, 8},
              "Test 04: No zeros at the beginning & at the end");
}