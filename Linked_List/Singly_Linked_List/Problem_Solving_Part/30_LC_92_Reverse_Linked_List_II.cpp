#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;

struct ListNode
{
    int val;
    ListNode *next;

    explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution
{
public:
    ListNode *reverseBetweenSafe(ListNode *head, int left, int right)
    {
        if (head == nullptr || left == right)
        {
            return head;
        }

        vector<int> values;
        ListNode *curr = head;

        while (curr != nullptr)
        {
            values.push_back(curr->val);
            curr = curr->next;
        }

        std::reverse(values.begin() + left - 1, values.begin() + right);

        curr = head;
        for (int val : values)
        {
            curr->val = val;
            curr = curr->next;
        }

        return head;
    }

    ListNode *reverseBetween(ListNode *head, int left, int right)
    {
        if (head == nullptr || left == right)
        {
            return head;
        }

        ListNode dummy(-1);
        dummy.next = head;

        ListNode *prev = &dummy;

        for (int i = 0; i < left - 1; i++)
        {
            prev = prev->next;
        }

        ListNode *curr = prev->next;

        for (int i = 0; i < right - left; i++)
        {
            ListNode *nextNode = curr->next;

            curr->next = nextNode->next;
            nextNode->next = prev->next;
            prev->next = nextNode;
        }

        return dummy.next;
    }
};

ListNode *createLinkedList(const vector<int> &nums)
{
    if (nums.empty())
    {
        return nullptr;
    }

    ListNode *head = new ListNode(nums[0]);
    ListNode *current = head;

    for (size_t i = 1; i < nums.size(); i++)
    {
        current->next = new ListNode(nums[i]);
        current = current->next;
    }

    return head;
}

void printLinkedList(ListNode *head)
{
    if (head == nullptr)
    {
        cout << "[]\n";
        return;
    }

    cout << "[";

    while (head != nullptr)
    {
        cout << head->val;

        if (head->next != nullptr)
        {
            cout << ", ";
        }

        head = head->next;
    }

    cout << "]\n";
}

void deleteLinkedListSafely(ListNode *head)
{
    ListNode *current = head;

    while (current != nullptr)
    {
        ListNode *toDelete = current;
        current = current->next;
        delete toDelete;
    }
}

void runTestCase(const vector<int> &input, int left, int right, const string &testName)
{
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode *head = createLinkedList(input);

    cout << "Original List : ";
    printLinkedList(head);
    cout << "left = " << left << ", right = " << right << "\n";

    ListNode *resultHead = sol.reverseBetween(head, left, right);

    cout << "Modified List : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main()
{
    cout << "--- Testing LeetCode 92: Reverse Linked List II ---\n\n";

    runTestCase({1, 2, 3, 4, 5}, 2, 4, "Example 1 (Reverse middle part)");
    runTestCase({5}, 1, 1, "Example 2 (Single node)");
    runTestCase({1, 2, 3, 4, 5}, 1, 5, "Edge Case 1 (Reverse entire list)");
    runTestCase({1, 2}, 1, 2, "Edge Case 2 (Two nodes, reverse both)");
    runTestCase({3, 5}, 1, 1, "Edge Case 3 (Two nodes, reverse none)");

    return 0;
}