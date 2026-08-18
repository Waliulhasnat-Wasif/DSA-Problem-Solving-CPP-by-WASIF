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

    explicit ListNode(int x) : val(x), next(nullptr) {}
};

class Solution
{
public:
    ListNode *insertionSortListSafe(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
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

        int n = static_cast<int>(values.size());
        for (int i = 1; i < n; i++)
        {
            int key = values[i];
            int j = i - 1;

            while (j >= 0 && values[j] > key)
            {
                values[j + 1] = values[j];
                j--;
            }
            values[j + 1] = key;
        }

        curr = head;
        for (int v : values)
        {
            curr->val = v;
            curr = curr->next;
        }

        return head;
    }

    ListNode *insertionSortList(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return head;
        }

        ListNode dummy(-1);
        dummy.next = head;

        ListNode *lastSorted = head;
        ListNode *curr = lastSorted->next;

        while (curr != nullptr)
        {
            if (lastSorted->val <= curr->val)
            {
                lastSorted = lastSorted->next;
                curr = curr->next;
            }
            else
            {
                ListNode *prev = &dummy;

                while (prev->next->val <= curr->val)
                {
                    prev = prev->next;
                }

                lastSorted->next = curr->next;
                curr->next = prev->next;
                prev->next = curr;

                curr = lastSorted->next;
            }
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
            cout << ", ";
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

void runTestCase(const vector<int> &input, const string &testName)
{
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode *head = createLinkedList(input);

    cout << "Original List : ";
    printLinkedList(head);

    ListNode *resultHead = sol.insertionSortList(head);

    cout << "Sorted List   : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";
    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main()
{
    cout << "--- Testing LeetCode 147: Insertion Sort List ---\n\n";

    runTestCase({4, 2, 1, 3}, "Example 1 (Unsorted positive integers)");
    runTestCase({-1, 5, 3, 4, 0}, "Example 2 (Contains negative integers)");
    runTestCase({1, 2, 3, 4, 5}, "Edge Case 1 (Already sorted)");
    runTestCase({5, 4, 3, 2, 1}, "Edge Case 2 (Reverse sorted - worst case)");
    runTestCase({1}, "Edge Case 3 (Single element)");

    return 0;
}