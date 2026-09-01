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
    ListNode* rotateLeftSafe(ListNode* head, int k) {
        if (head == nullptr || head->next == nullptr || k == 0) {
            return head;
        }

        vector<int> vals;
        ListNode* curr = head;

        while (curr != nullptr) {
            vals.push_back(curr->val);
            curr = curr->next;
        }

        int n = static_cast<int>(vals.size());
        k = k % n;

        if (k == 0) {
            return head;
        }

        std::rotate(vals.begin(), vals.begin() + k, vals.end());

        curr = head;
        for (int val : vals) {
            curr->val = val;
            curr = curr->next;
        }

        return head;
    }

    ListNode* rotateLeft(ListNode* head, int k) {
        if (head == nullptr || head->next == nullptr || k == 0) {
            return head;
        }

        ListNode* tail = head;
        int length = 1;
        while (tail->next != nullptr) {
            tail = tail->next;
            length++;
        }

        k = k % length;

        if (k == 0) {
            return head;
        }

        tail->next = head;

        int stepsToNewTail = k;

        ListNode* newTail = head;
        for (int i = 1; i < stepsToNewTail; i++) {
            newTail = newTail->next;
        }

        ListNode* newHead = newTail->next;
        newTail->next = nullptr;

        return newHead;
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
    cout << "Left Rotations (k) : " << k << "\n";

    ListNode* resultHead = sol.rotateLeft(head, k);

    cout << "Modified List : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main() {
    cout << "--- Testing Left Rotate Linked List ---\n\n";

    runTestCase({1, 2, 3, 4, 5}, 2, "Example 1 (Basic left rotation)");
    runTestCase({0, 1, 2}, 4, "Example 2 (k > length)");

    return 0;
}