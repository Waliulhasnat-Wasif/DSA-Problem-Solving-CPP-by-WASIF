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
    ListNode *mergeInBetween(ListNode *list1, int a, int b, ListNode *list2)
    {
        ListNode *startNode = list1;

        for (int i = 0; i < a - 1; i++)
        {
            startNode = startNode->next;
        }

        ListNode *curr = startNode->next;
        for (int i = 0; i < b - a + 1; i++)
        {
            ListNode *toDelete = curr;
            curr = curr->next;

            delete toDelete;
        }

        ListNode *endNode = curr;

        ListNode *list2Tail = list2;
        while (list2Tail->next != nullptr)
        {
            list2Tail = list2Tail->next;
        }

        startNode->next = list2;
        list2Tail->next = endNode;

        return list1;
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

void runTestCase(const vector<int> &input1, int a, int b, const vector<int> &input2, const string &testName)
{
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode *list1 = createLinkedList(input1);
    ListNode *list2 = createLinkedList(input2);

    cout << "List 1 : ";
    printLinkedList(list1);
    cout << "List 2 : ";
    printLinkedList(list2);
    cout << "a = " << a << ", b = " << b << "\n";

    ListNode *resultHead = sol.mergeInBetween(list1, a, b, list2);

    cout << "Output : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main()
{
    cout << "--- Testing LeetCode 1669: Merge In Between Linked Lists ---\n\n";

    runTestCase({10, 1, 13, 6, 9, 5}, 3, 4, {1000000, 1000001, 1000002}, "Example 1");
    runTestCase({0, 1, 2, 3, 4, 5, 6}, 2, 5, {1000000, 1000001, 1000002, 1000003, 1000004}, "Example 2");

    return 0;
}