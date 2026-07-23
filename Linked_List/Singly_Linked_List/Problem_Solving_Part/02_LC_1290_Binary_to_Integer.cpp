#include <iostream>
#include <vector>
#include <string>

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
public:
    int getDecimalValue(ListNode *head)
    {
        int result = 0;

        while (head != nullptr)
        {
            result = (result << 1) | head->val;
            head = head->next;
        }

        return result;
    }
};

ListNode *createLinkedList(const vector<int> &nums)
{
    if (nums.empty())
        return nullptr;

    ListNode *head = new ListNode(nums[0]);
    ListNode *curr = head;

    for (size_t i = 1; i < nums.size(); i++)
    {
        curr->next = new ListNode(nums[i]);
        curr = curr->next;
    }

    return head;
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

void runTestCase(const vector<int> &input, int expectedOutput, const string &testName)
{
    Solution sol;
    ListNode *head = createLinkedList(input);

    int result = sol.getDecimalValue(head);

    cout << "Test Case: " << testName << "\n";
    cout << "Input bits: ";

    for (int bit : input)
        cout << bit;

    cout << "\nOutput: " << result << " | Expected: " << expectedOutput;

    if (result == expectedOutput)
        cout << " -> [PASS] \n";
    else
        cout << " -> [FAIL] \n";

    cout << string(40, '-') << "\n";

    deleteLinkedList(head);
}

int main()
{
    cout << "--- Testing LeetCode 1290: Convert Binary Number to Integer ---\n\n";

    runTestCase({1, 0, 1}, 5, "Standard Example");

    runTestCase({0}, 0, "Single Zero");

    runTestCase({1}, 1, "Single One");

    runTestCase({1, 0, 0, 1, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0}, 18880, "Large Binary String");

    vector<int> maxConstraint(30, 1);

    runTestCase(maxConstraint, 1073741823, "Max Constraints (30 ones)");

    return 0;
}