#include <iostream>
#include <string>
#include <utility>
#include <vector>

using std::cout;
using std::string;
using std::swap;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;

    explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode* swapNodesSafe(ListNode* head, int k) {
        /*
        if (head == nullptr)
        {
            return head;
        }
        */
        int length = 0;
        ListNode* current = head;

        while (current != nullptr) {
            length++;
            current = current->next;
        }

        ListNode* node1 = head;
        for (int i = 0; i < k - 1; i++) {
            node1 = node1->next;
        }

        ListNode* node2 = head;
        for (int i = 0; i < length - k; i++) {
            node2 = node2->next;
        }

        swap(node1->val, node2->val);

        return head;
    }

    ListNode* swapNodes(ListNode* head, int k) {
        /*
        if (head == nullptr)
        {
            return head;
        }
        */
        ListNode* current = head;

        for (int i = 0; i < k - 1; i++) {
            current = current->next;
        }

        ListNode* node1 = current;

        ListNode* node2 = head;

        while (current->next != nullptr) {
            current = current->next;
            node2 = node2->next;
        }

        swap(node1->val, node2->val);

        return head;
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
    cout << "k = " << k << "\n";

    // ListNode *resultHead = sol.swapNodesSafe(head, k);
    ListNode* resultHead = sol.swapNodes(head, k);

    cout << "Modified List : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main() {
    cout << "--- Testing LeetCode 1721: Swapping Nodes in a Linked List ---\n\n";

    runTestCase({1, 2, 3, 4, 5}, 2, "Example 1 (Swap 2nd and 4th)");
    runTestCase({7, 9, 6, 6, 7, 8, 3, 0, 9, 5}, 5, "Example 2 (Swap middle elements)");
    runTestCase({1}, 1, "Edge Case (Single Element)");
    runTestCase({1, 2}, 1, "Edge Case (Swap head and tail)");
    // runTestCase({}, 1, "Defensive Engineering (Empty List Guard Test)");

    return 0;
}