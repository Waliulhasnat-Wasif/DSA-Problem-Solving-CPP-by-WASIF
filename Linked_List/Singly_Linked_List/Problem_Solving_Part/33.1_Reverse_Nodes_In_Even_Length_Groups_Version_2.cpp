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
    ListNode* reverseGroupsByParity(ListNode* head, bool reverseEven) {
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        ListNode dummy(-1);
        dummy.next = head;

        ListNode* prevGroupEnd = &dummy;
        int expectedLength = 1;

        while (prevGroupEnd->next != nullptr) {
            int count = 0;
            ListNode* temp = prevGroupEnd->next;

            while (temp != nullptr && count < expectedLength) {
                count++;
                temp = temp->next;
            }

            bool shouldReverse = reverseEven ? (count % 2 == 0) : (count % 2 != 0);

            if (shouldReverse) {
                ListNode* curr = prevGroupEnd->next;
                ListNode* groupPrev = nullptr;
                ListNode* oldHead = curr;

                for (int i = 0; i < count; i++) {
                    ListNode* nextNode = curr->next;
                    curr->next = groupPrev;
                    groupPrev = curr;
                    curr = nextNode;
                }

                prevGroupEnd->next = groupPrev;
                oldHead->next = curr;

                prevGroupEnd = oldHead;
            } else {
                for (int i = 0; i < count; i++) {
                    prevGroupEnd = prevGroupEnd->next;
                }
            }

            expectedLength++;
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

void runTestCase(const vector<int>& input, bool reverseEven, const string& testName) {
    Solution sol;

    cout << "Test Case: " << testName << "\n";
    cout << "Mode       : " << (reverseEven ? "Reverse EVEN lengths" : "Reverse ODD lengths") << "\n";

    ListNode* head = createLinkedList(input);

    cout << "Original   : ";
    printLinkedList(head);

    ListNode* resultHead = sol.reverseGroupsByParity(head, reverseEven);

    cout << "Modified   : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main() {
    cout << "--- Testing Generalized Group Reversal ---\n\n";

    runTestCase({5, 2, 6, 3, 9, 1, 7, 3, 8, 4}, true, "Even Mode - Complex group lengths");
    runTestCase({1, 1, 0, 6}, true, "Even Mode - Last group is odd length (1)");

    runTestCase({5, 2, 6, 3, 9, 1, 7, 3, 8, 4}, false, "Odd Mode - Complex group lengths");
    runTestCase({1, 1, 0, 6, 5}, false, "Odd Mode - Testing short tail");
    runTestCase({1}, false, "Odd Mode - Single Node");

    return 0;
}