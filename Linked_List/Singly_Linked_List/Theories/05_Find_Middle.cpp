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

Node *findMiddle(Node *head)
{
    Node *fast = head;
    Node *slow = head;

    if (head == nullptr || head->next == nullptr)
    {
        return head;
    }

    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }

    return slow;
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
    Node *middle = nullptr;

    insertAtTail(head, tail, 20);
    insertAtTail(head, tail, 30);
    insertAtTail(head, tail, 40);
    insertAtTail(head, tail, 50);
    insertAtTail(head, tail, 60);
    // insertAtTail(head, tail, 70);

    cout << "Linked List elements: ";
    printLinkedList(head);

    cout << "The middle of the list element is: ";
    middle = findMiddle(head);

    if (middle != nullptr)
        cout << middle->val;
    else
        cout << "List is empty";

    // prevention for memory leak
    deleteLinkedList(head);

    return 0;
}
