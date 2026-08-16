#include <iostream>
#include <vector>
#include <string>
#include <stack>

using std::cout;
using std::stack;
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
private:
    ListNode *reverseList(ListNode *head)
    {
        ListNode *prev = nullptr;
        ListNode *curr = head;

        while (curr != nullptr)
        {
            ListNode *nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }

        return prev;
    }

public:
    ListNode *addTwoNumbersSafe(ListNode *l1, ListNode *l2)
    {
        l1 = reverseList(l1);
        l2 = reverseList(l2);

        ListNode *head = nullptr;
        int carry = 0;

        ListNode *curr1 = l1;
        ListNode *curr2 = l2;

        while (curr1 != nullptr || curr2 != nullptr || carry > 0)
        {
            int sum = carry;
            if (curr1 != nullptr)
            {
                sum += curr1->val;
                curr1 = curr1->next;
            }
            if (curr2 != nullptr)
            {
                sum += curr2->val;
                curr2 = curr2->next;
            }

            carry = sum / 10;

            ListNode *newNode = new ListNode(sum % 10);
            newNode->next = head;
            head = newNode;
        }

        l1 = reverseList(l1);
        l2 = reverseList(l2);

        return head;
    }

    ListNode *addTwoNumbers(ListNode *l1, ListNode *l2)
    {
        stack<int> s1, s2;

        while (l1 != nullptr)
        {
            s1.push(l1->val);
            l1 = l1->next;
        }
        while (l2 != nullptr)
        {
            s2.push(l2->val);
            l2 = l2->next;
        }

        ListNode *head = nullptr;
        int carry = 0;

        while (!s1.empty() || !s2.empty() || carry > 0)
        {
            int sum = carry;

            if (!s1.empty())
            {
                sum += s1.top();
                s1.pop();
            }
            if (!s2.empty())
            {
                sum += s2.top();
                s2.pop();
            }

            carry = sum / 10;

            ListNode *newNode = new ListNode(sum % 10);
            newNode->next = head;
            head = newNode;
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

void runTestCase(const vector<int> &input1, const vector<int> &input2, const string &testName)
{
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode *l1 = createLinkedList(input1);
    ListNode *l2 = createLinkedList(input2);

    cout << "List 1 (l1) : ";
    printLinkedList(l1);
    cout << "List 2 (l2) : ";
    printLinkedList(l2);

    ListNode *resultHead = sol.addTwoNumbers(l1, l2);

    cout << "Sum List    : ";
    printLinkedList(resultHead);
    cout << " -> [PASS]\n";
    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(resultHead);
    deleteLinkedListSafely(l1);
    deleteLinkedListSafely(l2);
}

int main()
{
    cout << "--- Testing LeetCode 445: Add Two Numbers II ---\n\n";

    runTestCase({7, 2, 4, 3}, {5, 6, 4}, "Example 1 (Different lengths, no carry at end)");
    runTestCase({2, 4, 3}, {5, 6, 4}, "Example 2 (Same lengths)");
    runTestCase({0}, {0}, "Example 3 (Zeroes)");
    runTestCase({5}, {5}, "Edge Case 1 (Single digit resulting in carry expansion)");
    runTestCase({9, 9, 9, 9}, {1}, "Edge Case 2 (Cascading carry)");

    return 0;
}