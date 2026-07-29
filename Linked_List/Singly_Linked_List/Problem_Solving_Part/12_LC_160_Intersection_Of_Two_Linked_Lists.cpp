#include <iostream>
#include <vector>
#include <unordered_set>
#include <string>

using std::cout;
using std::string;
using std::unordered_set;
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
    ListNode *getIntersectionNodeHash(ListNode *headA, ListNode *headB)
    {
        unordered_set<ListNode *> visitedNodes;
        ListNode *currentA = headA;

        while (currentA != nullptr)
        {
            visitedNodes.insert(currentA);
            currentA = currentA->next;
        }

        ListNode *currentB = headB;

        while (currentB != nullptr)
        {
            if (visitedNodes.find(currentB) != visitedNodes.end())
            {
                return currentB;
            }
            currentB = currentB->next;
        }

        return nullptr;
    }

    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB)
    {
        /*
        if (headA == nullptr || headB == nullptr)
        {
            return nullptr;
        }
        */

        ListNode *ptrA = headA;
        ListNode *ptrB = headB;

        while (ptrA != ptrB)
        {
            ptrA = (ptrA == nullptr) ? headB : ptrA->next;

            ptrB = (ptrB == nullptr) ? headA : ptrB->next;
        }

        return ptrA;
    }
};

void createIntersectedLists(const vector<int> &valsA, const vector<int> &valsB,
                            int skipA, int skipB,
                            ListNode *&headA, ListNode *&headB, ListNode *&intersectNode)
{
    headA = nullptr;
    headB = nullptr;
    intersectNode = nullptr;

    if (valsA.empty() || valsB.empty())
    {
        return;
    }

    headA = new ListNode(valsA[0]);
    ListNode *currentA = headA;
    for (size_t i = 1; i < valsA.size(); i++)
    {
        currentA->next = new ListNode(valsA[i]);
        currentA = currentA->next;
    }

    headB = new ListNode(valsB[0]);
    ListNode *currentB = headB;
    for (size_t i = 1; i < valsB.size(); i++)
    {
        currentB->next = new ListNode(valsB[i]);
        currentB = currentB->next;
    }

    ListNode *targetA = headA;
    for (int i = 0; i < skipA && targetA != nullptr; i++)
    {
        targetA = targetA->next;
    }

    ListNode *targetB = headB;
    for (int i = 0; i < skipB && targetB != nullptr; i++)
    {
        targetB = targetB->next;
    }

    if (targetA != nullptr && targetB != nullptr && skipA < valsA.size() && skipB < valsB.size())
    {
        intersectNode = targetA;

        if (skipB == 0)
        {
            headB = targetA;
        }
        else
        {
            ListNode *temp = headB;
            for (int i = 1; i < skipB; i++)
            {
                temp = temp->next;
            }

            ListNode *toDelete = temp->next;
            while (toDelete != nullptr)
            {
                ListNode *nextDel = toDelete->next;
                delete toDelete;
                toDelete = nextDel;
            }

            temp->next = targetA;
        }
    }
}

void deleteIntersectedListsSafely(ListNode *headA, ListNode *headB)
{
    unordered_set<ListNode *> visited;

    ListNode *currentA = headA;
    while (currentA != nullptr)
    {
        visited.insert(currentA);

        ListNode *toDelete = currentA;
        currentA = currentA->next;
        delete toDelete;
    }

    ListNode *currentB = headB;
    while (currentB != nullptr)
    {
        if (visited.count(currentB))
        {
            break;
        }

        ListNode *toDelete = currentB;
        currentB = currentB->next;
        delete toDelete;
    }
}

void printList(ListNode *head)
{
    cout << "[";
    while (head != nullptr)
    {
        cout << head->val;
        if (head->next != nullptr)
        {
            cout << " -> ";
        }
        head = head->next;
    }
    cout << "]\n";
}

void runTestCase(const vector<int> &valsA, const vector<int> &valsB,
                 int skipA, int skipB, const string &testName)
{
    Solution sol;
    cout << "Test Case: " << testName << "\n";

    ListNode *headA = nullptr;
    ListNode *headB = nullptr;
    ListNode *expectedNode = nullptr;

    createIntersectedLists(valsA, valsB, skipA, skipB, headA, headB, expectedNode);

    cout << "List A: ";
    printList(headA);

    cout << "List B: ";
    printList(headB);

    ListNode *resultNode = sol.getIntersectionNode(headA, headB);

    string expectedStr = (expectedNode != nullptr) ? std::to_string(expectedNode->val) : "null";
    string resultStr = (resultNode != nullptr) ? std::to_string(resultNode->val) : "null";

    cout << "Output points to val: " << resultStr
         << " | Expected points to val: " << expectedStr;

    if (resultNode == expectedNode)
    {
        cout << " -> [PASS]\n";
    }
    else
    {
        cout << " -> [FAIL]\n";
    }

    cout << string(50, '-') << "\n";

    deleteIntersectedListsSafely(headA, headB);
}

int main()
{
    cout << "--- Testing LeetCode 160: Intersection of Two Linked Lists ---\n\n";

    runTestCase({4, 1, 8, 4, 5}, {5, 6, 1, 8, 4, 5}, 2, 3, "Example 1 (Intersect at 8)");
    runTestCase({1, 9, 1, 2, 4}, {3, 2, 4}, 3, 1, "Example 2 (Intersect at 2)");
    runTestCase({2, 6, 4}, {1, 5}, 3, 2, "Example 3 (No intersection)");

    return 0;
}