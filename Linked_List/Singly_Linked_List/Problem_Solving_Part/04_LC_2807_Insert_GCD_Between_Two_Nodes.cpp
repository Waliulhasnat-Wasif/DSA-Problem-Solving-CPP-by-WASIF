#include <iostream>
#include <string>
#include <vector>

using std ::cout;
using std ::string;
using std ::vector;

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
private:
    int calculateGCD(int a, int b)
    {
        while (b != 0)
        {
            int remainder = a % b;
            a = b;
            b = remainder;
        }

        return a;
    }

public:
    ListNode *insertGreatestCommonDivisors(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
            return head;

        ListNode *current = head;

        while (current->next != nullptr)
        {
            ListNode *nextNode = current->next;

            int gcdValue = calculateGCD(current->val, nextNode->val);

            ListNode *gcdNode = new ListNode(gcdValue);

            current->next = gcdNode;
            gcdNode->next = nextNode;

            current = nextNode;
        }

        return head;
    }
};

ListNode *createLinkedList(const vector<int> &nums)
{
    if (nums.empty())
        return nullptr;

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
        cout << "List is empty" << "\n";
        return;
    }

    while (head != nullptr)
    {
        cout << head->val << " -> ";
        head = head->next;
    }

    cout << "NULL" << "\n";
}

void deleteLinkedList(ListNode *head)
{
    if (head == nullptr)
    {
        cout << "Nothing to delete. The List is already empty";
        return;
    }

    while (head != nullptr)
    {
        ListNode *toDelete = head;
        head = head->next;
        delete toDelete;
    }
}

void runTestCase(const vector<int> &input, const string &testName)
{
    cout << "Test case: " << testName << "\n";

    ListNode *head = createLinkedList(input);
    cout << "Original List: ";
    printLinkedList(head);

    Solution sol;
    ListNode *resultHead = sol.insertGreatestCommonDivisors(head);
    cout << "Modified List: ";
    printLinkedList(resultHead);
    cout << string(60, '-') << "\n";

    deleteLinkedList(resultHead);
}

int main()
{
    cout << "--- Testing LeetCode 2807: Insert Greatest Common Divisors ---\n\n";

    runTestCase({18, 6, 10, 3}, "Standard Example");
    runTestCase({7}, "Single Node (Base Case)");
    runTestCase({7, 11, 13}, "Prime Numbers");
    runTestCase({10, 10, 10}, "Identical Numbers");

    return 0;
}