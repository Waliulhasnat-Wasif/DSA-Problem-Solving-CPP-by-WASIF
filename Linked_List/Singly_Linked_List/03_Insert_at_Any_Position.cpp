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

void insertAtIndex(Node *&head, Node *&tail, int idx, int val)
{
    if (idx < 0)
    {
        cout << "Index can't be negative. Invalid index..." << endl;
        return;
    }

    if (idx == 0)
    {
        Node *newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        if (tail == NULL)
        {
            tail = newNode;
        }
        return;
    }

    Node *temp = head;
    for (int i = 1; i < idx; i++)
    {
        if (temp == NULL)
        {
            cout << "Error! Index out of bounds!" << endl;
            return;
        }
        temp = temp->next;
    }

    if (temp == NULL)
    {
        cout << "Error! Index out of bounds!" << endl;
        return;
    }

    Node *newNode = new Node(val);
    newNode->next = temp->next;
    temp->next = newNode;

    if (newNode->next == NULL)
    {
        tail = newNode;
    }
}

void deleteFromIndex(Node *&head, Node *&tail, int idx)
{
    if (idx < 0)
    {
        cout << "Invalid index. Index cannot be negative." << endl;
        return;
    }

    if (head == NULL)
    {
        cout << "The list is empty." << endl;
        return;
    }

    if (idx == 0)
    {
        Node *deleteNode = head;
        head = head->next;
        delete deleteNode;

        if (head == NULL)
        {
            tail = NULL;
        }
        return;
    }

    Node *temp = head;

    for (int i = 1; i < idx; i++)
    {
        if (temp == NULL)
        {
            cout << "Invalid index." << endl;
            return;
        }
        temp = temp->next;
    }

    if (temp == NULL || temp->next == NULL)
    {
        cout << "Invalid index." << endl;
        return;
    }

    Node *deleteNode = temp->next;
    temp->next = deleteNode->next;

    if (deleteNode == tail)
    {
        tail = temp;
    }
    delete deleteNode;
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

int main()
{
    Node *head = NULL;
    Node *tail = NULL;

    insertAtTail(head, tail, 20);
    insertAtTail(head, tail, 30);
    insertAtTail(head, tail, 40);
    insertAtTail(head, tail, 50);
    insertAtTail(head, tail, 60);
    insertAtTail(head, tail, 70);

    cout << "Original Linked List elements: ";
    printLinkedList(head);
    cout << "After deleting index 0: ";
    deleteFromIndex(head, tail, 0);
    printLinkedList(head);

    cout << "After deleting index 4: ";
    deleteFromIndex(head, tail, 4);
    printLinkedList(head);

    cout << "After deleting index 3: ";
    deleteFromIndex(head, tail, 3);
    printLinkedList(head);

    cout << "After inserting value 20 at index 0: ";
    insertAtIndex(head, tail, 0, 20);
    printLinkedList(head);

    cout << "After inserting value 25 at index 1: ";
    insertAtIndex(head, tail, 1, 25);
    printLinkedList(head);

    cout << "After inserting value 35 at index 3: ";
    insertAtIndex(head, tail, 3, 35);
    printLinkedList(head);

    cout << "After inserting value 45 at index 5: ";
    insertAtIndex(head, tail, 5, 45);
    printLinkedList(head);

    cout << "After inserting value 55 at index 7: ";
    insertAtIndex(head, tail, 7, 55);
    printLinkedList(head);

    return 0;
}
