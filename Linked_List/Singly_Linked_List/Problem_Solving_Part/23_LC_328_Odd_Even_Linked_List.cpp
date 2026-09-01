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
    ListNode* oddEvenListSafe(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        vector<int> oddValues;
        vector<int> evenValues;

        ListNode* curr = head;
        int index = 1;

        while (curr != nullptr) {
            if (index % 2 != 0) {
                oddValues.push_back(curr->val);
            } else {
                evenValues.push_back(curr->val);
            }
            curr = curr->next;
            index++;
        }

        curr = head;
        for (int val : oddValues) {
            curr->val = val;
            curr = curr->next;
        }
        for (int val : evenValues) {
            curr->val = val;
            curr = curr->next;
        }

        return head;
    }

    ListNode* oddEvenList(ListNode* head) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        ListNode* odd = head;
        ListNode* even = head->next;

        ListNode* evenHead = even;

        while (even != nullptr && even->next != nullptr) {
            odd->next = even->next;
            odd = odd->next;

            even->next = odd->next;
            even = even->next;
        }

        odd->next = evenHead;

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

void runTestCase(const vector<int>& input, const string& testName) {
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode* head = createLinkedList(input);

    cout << "Original List : ";
    printLinkedList(head);

    ListNode* resultHead = sol.oddEvenList(head);

    cout << "Modified List : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main() {
    cout << "--- Testing LeetCode 328: Odd Even Linked List ---\n\n";

    runTestCase({1, 2, 3, 4, 5}, "Example 1 (Odd total nodes)");
    runTestCase({2, 1, 3, 5, 6, 4, 7}, "Example 2 (Complex arrangement)");
    runTestCase({1, 2, 3, 4}, "Example 3 (Even total nodes)");
    runTestCase({1}, "Edge Case (Single Node)");
    runTestCase({}, "Edge Case (Empty List)");

    return 0;
}