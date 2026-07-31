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
    ListNode *swapPairsRecursive(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return head;
        }

        ListNode *firstNode = head;
        ListNode *secondNode = head->next;

        firstNode->next = swapPairsRecursive(secondNode->next);

        secondNode->next = firstNode;

        return secondNode;
    }

    ListNode *swapPairs(ListNode *head)
    {
        ListNode dummy(-1);
        dummy.next = head;

        ListNode *prev = &dummy;
        ListNode *curr = head;

        while (curr != nullptr && curr->next != nullptr)
        {
            ListNode *nextNode = curr->next;
            ListNode *nextPair = nextNode->next;

            nextNode->next = curr;
            curr->next = nextPair;
            prev->next = nextNode;

            prev = curr;
            curr = nextPair;
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

void runTestCase(const vector<int> &input, const string &testName)
{
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode *head = createLinkedList(input);

    cout << "Original List : ";
    printLinkedList(head);

    ListNode *resultHead = sol.swapPairs(head);
    // ListNode *resultHead = sol.swapPairsRecursive(head);

    cout << "Modified List : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
}

int main()
{
    cout << "--- Testing LeetCode 24: Swap Nodes in Pairs ---\n\n";

    runTestCase({1, 2, 3, 4}, "Example 1 (Even number of nodes)");
    runTestCase({}, "Example 2 (Empty List)");
    runTestCase({1}, "Example 3 (Single Node)");
    runTestCase({1, 2, 3}, "Example 4 (Odd number of nodes)");

    return 0;
}