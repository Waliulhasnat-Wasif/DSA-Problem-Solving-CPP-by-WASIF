#include <iostream>
using std ::cin;
using std ::cout;
using std ::endl;

class Node
{
public:
    int val;
    Node *next;

    Node(int v)
    {
        val = v;
        next = NULL;
    }
};

void insertAtTail(Node *&head, Node *&tail, int val)
{
    Node *newNode = new Node(val);

    if (head == NULL)
    {
        head = newNode;
        tail = newNode;
        return;
    }

    tail->next = newNode;
    tail = newNode;
}

bool createCycle(Node *head, Node *&tail, int startIdx, int endIdx)
{
    if (head == nullptr)
    {
        cout << "Your List is empty and can't create a cycle" << endl;
        return false;
    }

    if (startIdx < 0 || endIdx < 0)
    {
        cout << "Indices can't be negative" << endl;
        return false;
    }

    if (startIdx > endIdx)
    {
        cout << "Start index can't be greater than end index" << endl;
        return false;
    }

    Node *temp = head;
    Node *startNode = nullptr;
    Node *endNode = nullptr;
    int currIdx = 0;

    while (temp != nullptr)
    {
        if (currIdx == startIdx)
            startNode = temp;
        if (currIdx == endIdx)
        {
            endNode = temp;
            break;
        }
        temp = temp->next;
        currIdx++;
    }

    if (startNode == nullptr || endNode == nullptr)
    {
        cout << "Indices are out of bound. Please try for valid indices" << endl;
        return false;
    }

    Node *garbage = endNode->next;
    endNode->next = startNode;
    tail = endNode;
    cout << "Cycle is created successfully between index " << startIdx << " to index " << endIdx << endl;

    while (garbage != nullptr)
    {
        Node *deleteNode = garbage;
        garbage = garbage->next;
        delete deleteNode;
    }

    return true;
}

Node *detectCycleStart(Node *head)
{
    if (head == nullptr || head->next == nullptr)
        return nullptr;
    Node *fast = head;
    Node *slow = head;
    bool hasCycle = false;

    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
        if (slow == fast)
        {
            hasCycle = true;
            break;
        }
    }

    if (hasCycle)
    {
        fast = head;
        while (fast != slow)
        {
            fast = fast->next;
            slow = slow->next;
        }
        return slow;
    }

    return nullptr;
}

void printLinkedList(Node *head)
{
    if (head == nullptr)
    {
        cout << "List is empty" << endl;
        return;
    }

    Node *startCycle = detectCycleStart(head);

    if (startCycle == nullptr)
    {
        Node *temp = head;
        while (temp != nullptr)
        {
            cout << temp->val << " -> ";
            temp = temp->next;
        }
        cout << "NULL" << endl;
        return;
    }

    int cycleCount = 0;
    Node *temp = head;
    while (temp != nullptr)
    {
        if (temp == startCycle)
        {
            cycleCount++;

            if (cycleCount > 1)
            {
                cout << "[Cycle loop back to Node: " << temp->val << "]" << endl;
                return;
            }
        }
        cout << temp->val << " -> ";
        temp = temp->next;
    }
}

void deleteLinkedList(Node *head)
{
    Node *startCycle = detectCycleStart(head);
    if (startCycle != nullptr)
    {
        Node *temp = startCycle;
        while (temp->next != startCycle)
        {
            temp = temp->next;
        }
        temp->next = nullptr;
    }

    Node *curr = head;
    while (curr != nullptr)
    {
        Node *deleteNode = curr;
        curr = curr->next;
        delete deleteNode;
    }
}

int main()
{
    Node *head = NULL;
    Node *tail = NULL;
    Node *middle = nullptr;

    insertAtTail(head, tail, 20);
    insertAtTail(head, tail, 30);
    insertAtTail(head, tail, 40);
    insertAtTail(head, tail, 50);
    insertAtTail(head, tail, 60);
    insertAtTail(head, tail, 70);

    cout << "Linked List elements: ";
    printLinkedList(head);
    cout << "After creating cycle: ";
    createCycle(head, tail, 2, 4);
    printLinkedList(head);

    // prevention for memory leak
    deleteLinkedList(head);

    return 0;
}
