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

Node *mergeTwoSortedList(Node *list1, Node *list2)
{
    if (list1 == nullptr)
        return list2;
    if (list2 == nullptr)
        return list1;

    Node *dummy = new Node(-1);
    Node *tail = dummy;

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

    if (list1 != nullptr)
    {
        tail->next = list1;
    }
    else
    {
        tail->next = list2;
    }

    Node *mergedHead = dummy->next;
    delete dummy;

    return mergedHead;
}

Node *sortListRecursive(Node *head)
{
    if (head == nullptr || head->next == nullptr)
    {
        return head;
    }

    Node *slow = head;
    Node *fast = head->next;
    while (fast != nullptr && fast->next != nullptr)
    {
        slow = slow->next;
        fast = fast->next->next;
    }
    Node *rightHead = slow->next;
    slow->next = nullptr;

    Node *leftSorted = sortListRecursive(head);
    Node *rightSorted = sortListRecursive(rightHead);

    return mergeTwoSortedList(leftSorted, rightSorted);
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
    Node *newHead = sortListRecursive(head);
    printLinkedList(newHead);

    // prevention for memory leak
    deleteLinkedList(newHead);

    return 0;
}