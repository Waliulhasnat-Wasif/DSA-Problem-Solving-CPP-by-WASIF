#include <iostream>
#include <vector>
#include <string>

using std::cout;
using std::string;
using std::vector;

struct ListNode
{
    int val;
    ListNode *next;

    ListNode() : val(0), next(nullptr) {}
    explicit ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

class Solution
{
public:
    ListNode *deleteDuplicates(ListNode *head)
    {
        ListNode *current = head;

        while (current != nullptr && current->next != nullptr)
        {
            if (current->val == current->next->val)
            {
                ListNode *duplicateNode = current->next;
                current->next = duplicateNode->next;
                delete duplicateNode;
            }
            else
            {
                current = current->next;
            }
        }

        return head;
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

void deleteLinkedList(ListNode *head)
{
    while (head != nullptr)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
}

void runTestCase(const vector<int> &input, const string &testName)
{
    cout << "Test Case: " << testName << "\n";

    ListNode *head = createLinkedList(input);
    cout << "Original List : ";
    printLinkedList(head);

    Solution sol;
    ListNode *resultHead = sol.deleteDuplicates(head);
    cout << "Modified List : ";
    printLinkedList(resultHead);
    cout << string(50, '-') << "\n";

    deleteLinkedList(resultHead);
}

int main()
{
    cout << "--- Testing LeetCode 83: Remove Duplicates from Sorted List ---\n\n";

    runTestCase({1, 1, 2}, "Example 1 (Standard)");
    runTestCase({1, 1, 2, 3, 3}, "Example 2 (Multiple Duplicates)");
    runTestCase({}, "Edge Case 1 (Empty List)");
    runTestCase({1, 1, 1, 1, 1}, "Edge Case 2 (All Identical)");
    runTestCase({1, 2, 3, 4, 5}, "Edge Case 3 (No Duplicates)");
    runTestCase({-10, -10, -5, 0, 0}, "Edge Case 4 (Negative Numbers)");

    return 0;
}