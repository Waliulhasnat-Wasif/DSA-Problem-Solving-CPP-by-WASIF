#include <iostream>
#include <string>
#include <vector>

using std::cout;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;

    ListNode() : val(0), next(nullptr) {}
    explicit ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* reverseList(ListNode* head) {
        ListNode* previous = nullptr;
        ListNode* current = head;

        while (current != nullptr) {
            ListNode* nextNode = current->next;

            current->next = previous;

            previous = current;
            current = nextNode;
        }

        return previous;
    }

    ListNode* reverseListRecursive(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        ListNode* newHead = reverseListRecursive(head->next);

        head->next->next = head;
        head->next = nullptr;

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

void deleteLinkedList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

void runTestCase(const vector<int>& input, const string& testName) {
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode* head = createLinkedList(input);
    cout << "Original List : ";
    printLinkedList(head);

    // ListNode *resultHead = sol.reverseList(head);
    ListNode* resultHead = sol.reverseListRecursive(head);

    cout << "Reversed List : ";
    printLinkedList(resultHead);
    cout << string(50, '-') << "\n";

    deleteLinkedList(resultHead);
}

int main() {
    cout << "--- Testing LeetCode 206: Reverse Linked List ---\n\n";

    runTestCase({1, 2, 3, 4, 5}, "Example 1 (Standard 1 to 5)");
    runTestCase({1, 2}, "Example 2 (Two nodes)");
    runTestCase({}, "Example 3 (Empty List)");
    runTestCase({42}, "Edge Case (Single Node)");

    return 0;
}