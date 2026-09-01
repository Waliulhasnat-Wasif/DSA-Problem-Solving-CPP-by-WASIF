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
    void deleteNode(ListNode* node) {
        ListNode* nodeToDelete = node->next;

        node->val = nodeToDelete->val;

        node->next = nodeToDelete->next;

        delete nodeToDelete;
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

ListNode* getNodeByValue(ListNode* head, int targetVal) {
    ListNode* current = head;

    while (current != nullptr) {
        if (current->val == targetVal) {
            return current;
        }
        current = current->next;
    }

    return nullptr;
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

void runTestCase(const vector<int>& input, int nodeValToDelete, const string& testName) {
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode* head = createLinkedList(input);

    ListNode* targetNode = getNodeByValue(head, nodeValToDelete);

    cout << "Original List : ";
    printLinkedList(head);
    cout << "Node to delete: " << nodeValToDelete << "\n";

    if (targetNode != nullptr && targetNode->next != nullptr) {
        sol.deleteNode(targetNode);

        cout << "Modified List : ";
        printLinkedList(head);
        cout << " -> [PASS]\n";
    } else {
        cout << "Invalid input: Node not found or is the tail node.\n";
    }

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(head);
}

int main() {
    cout << "--- Testing LeetCode 237: Delete Node in a Linked List ---\n\n";

    runTestCase({4, 5, 1, 9}, 5, "Example 1 (Delete second node)");
    runTestCase({4, 5, 1, 9}, 1, "Example 2 (Delete third node)");

    return 0;
}