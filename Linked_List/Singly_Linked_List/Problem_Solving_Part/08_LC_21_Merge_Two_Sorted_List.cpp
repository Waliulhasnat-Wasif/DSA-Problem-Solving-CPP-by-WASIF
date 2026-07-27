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
    ListNode *mergeTwoLists(ListNode *list1, ListNode *list2)
    {
        ListNode dummy(-1);
        ListNode *tail = &dummy;

        while (list1 != nullptr && list2 != nullptr)
        {
            if (list1->val <= list2->val)
            {
                tail->next = list1;
                list1 = list1->next;
            }
            else
            {
                tail->next = list2;
                list2 = list2->next;
            }

            tail = tail->next;
        }

        tail->next = (list1 != nullptr) ? list1 : list2;

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

void deleteLinkedList(ListNode *head)
{
    while (head != nullptr)
    {
        ListNode *temp = head;
        head = head->next;
        delete temp;
    }
}

void runTestCase(const vector<int> &nums1, const vector<int> &nums2, const string &testName)
{
    cout << "Test Case: " << testName << "\n";

    ListNode *list1 = createLinkedList(nums1);
    ListNode *list2 = createLinkedList(nums2);

    cout << "List 1 : ";
    printLinkedList(list1);

    cout << "List 2 : ";
    printLinkedList(list2);

    Solution sol;
    ListNode *mergedHead = sol.mergeTwoLists(list1, list2);

    cout << "Merged : ";
    printLinkedList(mergedHead);
    cout << string(50, '-') << "\n";

    deleteLinkedList(mergedHead);
}

int main()
{
    cout << "--- Testing LeetCode 21: Merge Two Sorted Lists ---\n\n";

    runTestCase({1, 2, 4}, {1, 3, 4}, "Example 1 (Standard Merge)");
    runTestCase({}, {}, "Example 2 (Both Empty)");
    runTestCase({}, {0}, "Example 3 (One Empty)");
    runTestCase({1, 5, 9, 10, 20}, {2, 3}, "Edge Case (Different Sizes)");

    return 0;
}