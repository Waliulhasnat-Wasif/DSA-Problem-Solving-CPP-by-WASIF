#include <iostream>
#include <string>
#include <unordered_set>
#include <vector>

using std::cout;
using std::string;
using std::unordered_set;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;

    explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    bool hasCycleHash(ListNode* head) {
        unordered_set<ListNode*> visitedNodes;
        ListNode* current = head;

        while (current != nullptr) {
            if (visitedNodes.find(current) != visitedNodes.end()) {
                return true;
            }

            visitedNodes.insert(current);
            current = current->next;
        }

        return false;
    }

    bool hasCycle(ListNode* head) {
        ListNode* slow = head;
        ListNode* fast = head;

        while (fast != nullptr && fast->next != nullptr) {
            slow = slow->next;
            fast = fast->next->next;

            if (slow == fast) {
                return true;
            }
        }

        return false;
    }
};

ListNode* createLinkedListWithCycle(const vector<int>& nums, int pos) {
    if (nums.empty()) {
        return nullptr;
    }

    ListNode* head = new ListNode(nums[0]);
    ListNode* current = head;
    ListNode* cycleNode = (pos == 0) ? head : nullptr;

    for (size_t i = 1; i < nums.size(); i++) {
        current->next = new ListNode(nums[i]);
        current = current->next;

        if (static_cast<int>(i) == pos) {
            cycleNode = current;
        }
    }

    if (pos != -1) {
        current->next = cycleNode;
    }

    return head;
}

void printLinkedListSafely(ListNode* head) {
    if (head == nullptr) {
        cout << "[]\n";
        return;
    }

    unordered_set<ListNode*> visited;

    cout << "[";

    while (head != nullptr) {
        if (visited.count(head)) {
            cout << "... (Cycle loops back to node with val: " << head->val << ")]\n";
            return;
        }

        visited.insert(head);
        cout << head->val;

        if (head->next != nullptr) {
            cout << " -> ";
        }

        head = head->next;
    }

    cout << "]\n";
}

void deleteLinkedListSafely(ListNode* head) {
    unordered_set<ListNode*> visited;
    ListNode* current = head;

    while (current != nullptr) {
        if (visited.count(current)) {
            break;
        }

        visited.insert(current);

        ListNode* toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

void runTestCase(const vector<int>& input, int pos, bool expected, const string& testName) {
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode* head = createLinkedListWithCycle(input, pos);

    cout << "Original List : ";
    printLinkedListSafely(head);

    // bool result = sol.hasCycle(head);
    bool result = sol.hasCycleHash(head);

    cout << "Input sizes: " << input.size() << ", Pos: " << pos << "\n";
    cout << "Output: " << (result ? "true" : "false") << " | Expected: " << (expected ? "true" : "false");

    if (result == expected) {
        cout << " -> [PASS]\n";
    } else {
        cout << " -> [FAIL]\n";
    }

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(head);
}

int main() {
    cout << "--- Testing LeetCode 141: Linked List Cycle ---\n\n";

    runTestCase({3, 2, 0, -4}, 1, true, "Example 1 (Cycle in middle)");
    runTestCase({1, 2}, 0, true, "Example 2 (Cycle at head)");
    runTestCase({1}, -1, false, "Example 3 (No cycle)");
    runTestCase({}, -1, false, "Edge Case (Empty List)");

    return 0;
}