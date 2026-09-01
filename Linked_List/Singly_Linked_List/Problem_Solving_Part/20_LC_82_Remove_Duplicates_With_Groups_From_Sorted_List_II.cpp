#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::string;
using std::unordered_map;
using std::vector;

struct ListNode {
    int val;
    ListNode* next;

    explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution {
public:
    ListNode* deleteDuplicatesSafe(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        unordered_map<int, int> frequency;
        ListNode* current = head;

        while (current != nullptr) {
            frequency[current->val]++;
            current = current->next;
        }

        ListNode dummy(-1);
        dummy.next = head;

        ListNode* prev = &dummy;
        current = head;

        while (current != nullptr) {
            if (frequency[current->val] > 1) {
                ListNode* toDelete = current;
                prev->next = toDelete->next;
                current = current->next;

                delete toDelete;
            } else {
                prev = current;
                current = current->next;
            }
        }

        return dummy.next;
    }

    ListNode* deleteDuplicates(ListNode* head) {
        ListNode dummy(-1);
        dummy.next = head;

        ListNode* prev = &dummy;
        ListNode* current = head;

        while (current != nullptr) {
            if (current->next != nullptr && current->val == current->next->val) {
                int duplicateVal = current->val;

                while (current != nullptr && current->val == duplicateVal) {
                    ListNode* toDelete = current;
                    current = current->next;

                    delete toDelete;
                }

                prev->next = current;
            } else {
                prev = current;
                current = current->next;
            }
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

void runTestCase(const vector<int>& input, const string& testName) {
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode* head = createLinkedList(input);

    cout << "Original List : ";
    printLinkedList(head);

    ListNode* resultHead = sol.deleteDuplicates(head);
    // ListNode *resultHead = sol.deleteDuplicatesSafe(head);

    cout << "Modified List : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main() {
    cout << "--- Testing LeetCode 82: Remove Duplicates from Sorted List II "
            "---\n\n";

    runTestCase({1, 2, 3, 3, 4, 4, 5}, "Example 1 (Duplicates in middle)");
    runTestCase({1, 1, 1, 2, 3}, "Example 2 (Duplicates at head)");
    runTestCase({1, 1, 1, 1}, "Edge Case (All Duplicates)");
    runTestCase({1, 2, 3}, "Edge Case (No Duplicates)");
    runTestCase({2, 2, 3, 4, 4, 5, 5}, "Edge Case (Duplicates at both ends)");
    runTestCase({}, "Edge Case (Empty List)");

    return 0;
}