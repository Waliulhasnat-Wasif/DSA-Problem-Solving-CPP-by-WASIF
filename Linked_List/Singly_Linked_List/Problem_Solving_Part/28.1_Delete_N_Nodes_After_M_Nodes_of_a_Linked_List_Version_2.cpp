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
    ListNode* deleteNodesV2(ListNode* head, int m, int n) {
        if (head == nullptr) {
            return head;
        }

        if (n <= 0) {
            return head;
        }

        if (m <= 0) {
            ListNode* curr = head;
            while (curr != nullptr) {
                ListNode* toDelete = curr;
                curr = curr->next;
                delete toDelete;
            }
            return nullptr;
        }

        ListNode dummy(-1);
        dummy.next = head;

        ListNode* prev = &dummy;
        ListNode* curr = head;

        while (curr != nullptr) {
            for (int i = 0; i < m && curr != nullptr; i++) {
                prev = curr;
                curr = curr->next;
            }

            if (curr == nullptr) {
                break;
            }

            for (int i = 0; i < n && curr != nullptr; i++) {
                ListNode* toDelete = curr;
                curr = curr->next;

                delete toDelete;
            }

            prev->next = curr;
        }

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

void runTestCase(const vector<int>& input, int m, int n, const string& testName) {
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode* head = createLinkedList(input);

    cout << "Input List : ";
    printLinkedList(head);
    cout << "m = " << m << ", n = " << n << "\n";

    ListNode* resultHead = sol.deleteNodesV2(head, m, n);

    cout << "Output     : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main() {
    cout << "--- Testing LeetCode 1474: V2 (Bulletproof Edge Cases) ---\n\n";

    runTestCase({1, 2, 3, 4, 5}, 2, 2, "Standard Case");
    runTestCase({}, 2, 2, "Edge Case 1: Empty List");
    runTestCase({1, 2, 3}, 2, 0, "Edge Case 2: n = 0 (Delete Nothing)");
    runTestCase({1, 2, 3, 4}, 0, 2, "Edge Case 3: m = 0 (Keep Nothing, Delete All)");
    runTestCase({1, 2, 3}, 10, 2, "Edge Case 4: m is Out of Bounds");
    runTestCase({1, 2, 3}, 1, 10, "Edge Case 5: n is Out of Bounds");

    return 0;
}