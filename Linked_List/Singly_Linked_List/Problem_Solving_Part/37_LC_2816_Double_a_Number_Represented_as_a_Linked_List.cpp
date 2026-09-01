#include <iostream>
#include <stack>
#include <string>
#include <vector>

using std::cout;
using std::stack;
using std::string;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;

    explicit ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class Solution {
public:
    ListNode* doubleItSafe(ListNode* head) {
        if (head == nullptr) {
            return head;
        }

        stack<int> s;
        ListNode* curr = head;
        while (curr != nullptr) {
            s.push(curr->val);
            curr = curr->next;
        }

        ListNode* newHead = nullptr;
        int carry = 0;

        while (!s.empty() || carry > 0) {
            int sum = carry;
            if (!s.empty()) {
                sum += s.top() * 2;
                s.pop();
            }

            carry = sum / 10;

            ListNode* newNode = new ListNode(sum % 10);
            newNode->next = newHead;
            newHead = newNode;
        }

        curr = head;
        while (curr != nullptr) {
            ListNode* toDelete = curr;
            curr = curr->next;
            delete toDelete;
        }

        return newHead;
    }

    ListNode* doubleIt(ListNode* head) {
        if (head == nullptr) {
            return head;
        }

        ListNode* resultHead = (head->val >= 5) ? new ListNode(1, head) : head;

        ListNode* curr = head;
        while (curr != nullptr) {
            int doubled = curr->val * 2;

            if (curr->next != nullptr && curr->next->val >= 5) {
                curr->val = (doubled % 10) + 1;
            } else {
                curr->val = doubled % 10;
            }

            curr = curr->next;
        }

        return resultHead;
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

void runTestCase(const vector<int>& input, const string& testName) {
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode* head = createLinkedList(input);

    cout << "Original List : ";
    printLinkedList(head);

    // Calling the optimal O(1) space method (In-place modification)
    ListNode* resultHead = sol.doubleIt(head);

    cout << "Doubled List  : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";
    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main() {
    cout << "--- Testing LeetCode 2816: Double a Number Represented as a Linked "
            "List ---\n\n";

    runTestCase({1, 8, 9}, "Example 1 (No new head required)");
    runTestCase({9, 9, 9}, "Example 2 (Cascading carry, new head required)");
    runTestCase({0}, "Edge Case 1 (Zero)");
    runTestCase({4, 9, 9}, "Edge Case 2 (Almost carry over to head)");
    runTestCase({5}, "Edge Case 3 (Single digit >= 5)");

    return 0;
}