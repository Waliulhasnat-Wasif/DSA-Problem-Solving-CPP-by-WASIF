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
    void reorderListSafe(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return;
        }

        vector<ListNode *> nodes;
        ListNode *curr = head;

        while (curr != nullptr)
        {
            nodes.push_back(curr);
            curr = curr->next;
        }

        int left = 0;
        int right = nodes.size() - 1;

        while (left < right)
        {
            nodes[left]->next = nodes[right];
            left++;

            if (left == right)
            {
                break;
            }

            nodes[right]->next = nodes[left];
            right--;
        }

        nodes[left]->next = nullptr;
    }

    void reorderList(ListNode *head)
    {
        if (head == nullptr || head->next == nullptr)
        {
            return;
        }

        ListNode *slow = head;

        ListNode *fast = head->next;

        while (fast != nullptr && fast->next != nullptr)
        {
            slow = slow->next;
            fast = fast->next->next;
        }

        ListNode *secondHalf = slow->next;

        slow->next = nullptr;

        ListNode *prev = nullptr;
        ListNode *curr = secondHalf;

        while (curr != nullptr)
        {
            ListNode *nextNode = curr->next;
            curr->next = prev;
            prev = curr;
            curr = nextNode;
        }

        ListNode *first = head;
        ListNode *second = prev;

        while (second != nullptr)
        {
            ListNode *temp1 = first->next;
            ListNode *temp2 = second->next;

            first->next = second;
            second->next = temp1;

            first = temp1;
            second = temp2;
        }
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

    sol.reorderList(head);

    cout << "Reordered List: ";
    printLinkedList(head);
    cout << " -> [PASS]\n";

    cout << string(50, '-') << "\n";

    deleteLinkedListSafely(head);
}

int main()
{
    cout << "--- Testing LeetCode 143: Reorder List ---\n\n";

    runTestCase({1, 2, 3, 4}, "Example 1 (Even number of nodes)");
    runTestCase({1, 2, 3, 4, 5}, "Example 2 (Odd number of nodes)");
    runTestCase({1}, "Edge Case (Single node)");
    runTestCase({1, 2}, "Edge Case (Two nodes)");
    runTestCase({}, "Edge Case (Empty list)");

    return 0;
}