#include <iostream>
#include <unordered_set>
using std ::cin;
using std ::cout;
using std ::endl;
using std ::unordered_set;

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

bool createCycle(Node *head, int startIdx, int endIdx)
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
            endNode = temp;
        if (startNode != nullptr && endNode != nullptr)
            break;
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
    cout << "Cycle is created successfully between index " << startIdx << " to index " << endIdx << endl;

    while (garbage != nullptr)
    {
        Node *deleteNode = garbage;
        garbage = garbage->next;
        delete deleteNode;
    }

    return true;
}

void printLinkedList(Node *head)
{
    Node *temp = head;
    if (temp == nullptr)
    {
        cout << "List is empty" << endl;
        return;
    }

    unordered_set<Node *> visitedNodes;

    while (temp != nullptr)
    {
        if (visitedNodes.find(temp) != visitedNodes.end())
        {
            cout << "...(Cycle loop back to Node: " << temp->val << ")" << endl;
            return;
        }
        cout << temp->val << " -> ";
        visitedNodes.insert(temp);
        temp = temp->next;
    }

    cout << "NULL" << endl;
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
    createCycle(head, 2, 4);
    printLinkedList(head);

    return 0;
}
