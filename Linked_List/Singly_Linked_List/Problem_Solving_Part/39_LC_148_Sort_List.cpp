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
  int getLength(ListNode* head) {
    int len = 0;
    while (head != nullptr) {
      len++;
      head = head->next;
    }

    return len;
  }

  ListNode* split(ListNode* head, int step) {
    if (head == nullptr) {
      return head;
    }

    for (int i = 1; head->next != nullptr && i < step; i++) {
      head = head->next;
    }

    ListNode* rest = head->next;
    head->next = nullptr;

    return rest;
  }

  ListNode* mergeTwoSortedList(ListNode* list1, ListNode* list2,
                               ListNode*& tailRef) {
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

    while (tail->next != nullptr) {
      tail = tail->next;
    }
    tailRef = tail;

    return dummy.next;
  }

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

public:
  ListNode* sortListIterative(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    int length = getLength(head);
    ListNode dummy(-1);
    dummy.next = head;

    for (int step = 1; step < length; step *= 2) {
      ListNode* curr = dummy.next;
      ListNode* prevTail = &dummy;

      while (curr != nullptr) {
        ListNode* left = curr;
        ListNode* right = split(left, step);
        curr = split(right, step);

        ListNode* mergedTail = nullptr;
        prevTail->next = mergeTwoSortedList(left, right, mergedTail);
        prevTail = mergedTail;
      }
    }

    return dummy.next;
  }

  ListNode* sortListRecursive(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    ListNode* slow = head;
    ListNode* fast = head->next;
    while (fast != nullptr && fast->next != nullptr) {
      slow = slow->next;
      fast = fast->next->next;
    }

    ListNode* rightHead = slow->next;
    slow->next = nullptr;

    ListNode* leftSorted = sortListRecursive(head);
    ListNode* rightSorted = sortListRecursive(rightHead);

    return mergeTwoSortedList(leftSorted, rightSorted);
  }

  ListNode* sortListSafe(ListNode* head) {
    if (head == nullptr || head->next == nullptr) {
      return head;
    }

    vector<int> values;
    ListNode* curr = head;

    while (curr != nullptr) {
      values.push_back(curr->val);
      curr = curr->next;
    }

    std::sort(values.begin(), values.end());

    curr = head;
    for (int val : values) {
      curr->val = val;
      curr = curr->next;
    }

    return head;
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

int main() {
  Solution sol;

  cout << "--- Testing Sort List (Iterative vs Recursive) ---\n\n";

  cout << "Method: Iterative (O(1) Space)\n";
  ListNode* head1 = createLinkedList({4, 2, 1, 3});
  cout << "Original : ";
  printLinkedList(head1);
  head1 = sol.sortListIterative(head1);
  cout << "Sorted   : ";
  printLinkedList(head1);
  deleteLinkedListSafely(head1);
  cout << "------------------------------------------\n";

  cout << "Method: Recursive (O(log N) Space)\n";
  ListNode* head2 = createLinkedList({-1, 5, 3, 4, 0});
  cout << "Original : ";
  printLinkedList(head2);
  head2 = sol.sortListRecursive(head2);
  cout << "Sorted   : ";
  printLinkedList(head2);
  deleteLinkedListSafely(head2);

  return 0;
}