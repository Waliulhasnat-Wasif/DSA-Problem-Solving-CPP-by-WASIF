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

void deleteFromIndex(Node *&head, Node *&tail, int idx)
{
    if (idx < 0)
    {
        cout << "Index can't be negative" << endl;
        return;
    }

    if (head == NULL)
    {
        cout << "List is empty!" << endl;
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
            cout << "Invalid index" << endl;
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

    cout << "After deleting index 2: ";
    deleteFromIndex(head, tail, 2);
    printLinkedList(head);

    return 0;
}
