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
    bool isPalindromeSafe(ListNode *head)
    {
        vector<int> vals;
        ListNode *current = head;

        while (current != nullptr)
        {
            vals.push_back(current->val);
            current = current->next;
        }

        int left = 0;
        int right = static_cast<int>(vals.size()) - 1;

        while (left < right)
        {
            if (vals[left] != vals[right])
            {
                return false;
            }
            left++;
            right--;
        }

        return true;
    }

    bool isPalindrome(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return true;
        }

        ListNode *slow = head;
        ListNode *fast = head;

        while (fast != nullptr && fast->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode *secondHalfHead = reverseList(slow);

        ListNode *p1 = head;
        ListNode *p2 = secondHalfHead;
        bool result = true;

        while (p2 != nullptr)
        {
            if (p1->val != p2->val)
            {
                result = false;
                break;
            }
            p1 = p1->next;
            p2 = p2->next;
        }

        reverseList(secondHalfHead);

        return result;
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

void runTestCase(const vector<int> &input, bool expected, const string &testName)
{
    Solution sol;

    cout << "Test Case: " << testName << "\n";

    ListNode *head = createLinkedList(input);

    cout << "Original List  : ";
    printLinkedList(head);

    bool result = sol.isPalindrome(head);

    cout << "Output: " << (result ? "true" : "false")
         << " | Expected: " << (expected ? "true" : "false");

    if (result == expected)
    {
        cout << " -> [PASS]\n";
    }
    else
    {
        cout << " -> [FAIL]\n";
    }

    cout << "Restored List  : ";
    printLinkedList(head);

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(head);
}

int main()
{
    cout << "--- Testing LeetCode 234: Palindrome Linked List ---\n\n";

    runTestCase({1, 2, 2, 1}, true, "Example 1 (Even Length Palindrome)");
    runTestCase({1, 2}, false, "Example 2 (Not a Palindrome)");
    runTestCase({1, 2, 3, 2, 1}, true, "Example 3 (Odd Length Palindrome)");
    runTestCase({1}, true, "Edge Case (Single node)");

    return 0;
}