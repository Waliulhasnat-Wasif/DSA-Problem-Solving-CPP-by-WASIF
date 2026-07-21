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
        next = nullptr;
    }
};

void insertAtTail(Node *&head, Node *&tail, int val)
{
    Node *newNode = new Node(val);
    if (head == nullptr)
    {
        head = newNode;
        tail = newNode;
        return;
    }
    tail->next = newNode;
    tail = newNode;
}

int getLength(Node *head)
{
    int len = 0;
    while (head != nullptr)
    {
        len++;
        head = head->next;
    }
    return len;
}

Node *split(Node *head, int step)
{
    if (head == nullptr)
        return nullptr;
    for (int i = 1; head->next != nullptr && i < step; i++)
    {
        head = head->next;
    }
    Node *rest = head->next;
    head->next = nullptr;
    return rest;
}

Node *mergeTwoSortedList(Node *list1, Node *list2, Node *&tailRef)
{
    Node dummy(-1);
    Node *tail = &dummy;

    while (list1 != nullptr && list2 != nullptr)
    {
        if (list1->val <= list2->val)
        {
            tail->next = list1;
            list1 = list1->next;
        }
        else
        {
            tail->next = list2;
            list2 = list2->next;
        }
        tail = tail->next;
    }

    tail->next = (list1 != nullptr) ? list1 : list2;

    while (tail->next != nullptr)
    {
        tail = tail->next;
    }
    tailRef = tail;

    return dummy.next;
}

Node *sortListIterative(Node *head)
{
    if (head == nullptr || head->next == nullptr)
        return head;

    int length = getLength(head);
    Node dummy(-1);
    dummy.next = head;

    for (int step = 1; step < length; step *= 2)
    {
        Node *curr = dummy.next;
        Node *prevTail = &dummy;

        while (curr != nullptr)
        {
            Node *left = curr;
            Node *right = split(left, step);
            curr = split(right, step);

            Node *mergedTail = nullptr;
            prevTail->next = mergeTwoSortedList(left, right, mergedTail);
            prevTail = mergedTail;
        }
    }

    return dummy.next;
}

void printLinkedList(Node *head)
{
    Node *temp = head;
    while (temp != nullptr)
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
    Node *head = nullptr;
    Node *tail = nullptr;

    insertAtTail(head, tail, 10);
    insertAtTail(head, tail, 8);
    insertAtTail(head, tail, 7);
    insertAtTail(head, tail, 15);
    insertAtTail(head, tail, 20);
    insertAtTail(head, tail, 18);
    insertAtTail(head, tail, 30);
    insertAtTail(head, tail, 22);

    cout << "The Unsorted List No is: ";
    printLinkedList(head);

    cout << "After sorted the list is: ";
    Node *newHead = sortListIterative(head);
    printLinkedList(newHead);

    // prevention for memory leak
    deleteLinkedList(newHead);

    return 0;
}