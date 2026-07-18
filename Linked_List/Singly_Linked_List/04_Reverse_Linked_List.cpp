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

Node *reverseLinkedList(Node *head)
{
    Node *prev = nullptr;
    Node *curr = head;
    Node *nextNode = nullptr;

    while (curr != nullptr)
    {
        nextNode = curr->next;
        curr->next = prev;
        prev = curr;
        curr = nextNode;
    }
    return prev;
}

Node *reverseLinkedListRecursive(Node *head)
{
    if (head == nullptr || head->next == nullptr)
        return head;

    Node *nextNode = head->next;
    Node *newHead = reverseLinkedListRecursive(nextNode);
    nextNode->next = head;
    head->next = nullptr;

    return newHead;
}

void printLinkedList(Node *head)
{
    Node *temp = head;
    while (temp != NULL)
    {
        cout << temp->val << " -> ";
        temp = temp->next;
    }
    cout << "NULL" << endl;
}

void deleteLinkedList(Node *head)
{
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

    insertAtTail(head, tail, 20);
    insertAtTail(head, tail, 30);
    insertAtTail(head, tail, 40);
    insertAtTail(head, tail, 50);

    cout << "Linked List elements: ";
    printLinkedList(head);

    cout << "After iteratively reversing, the List elements: ";
    head = reverseLinkedList(head);
    printLinkedList(head);

    cout << "After recursively reversing, the List elements: ";
    head = reverseLinkedListRecursive(head);
    printLinkedList(head);

    // prevention for memory leak
    deleteLinkedList(head);

    return 0;
}
