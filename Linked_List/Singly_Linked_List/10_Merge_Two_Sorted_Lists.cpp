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

Node *mergeTwoSortedListRecursive(Node *list1, Node *list2)
{
    if (list1 == nullptr)
        return list2;
    if (list2 == nullptr)
        return list1;

    if (list1->val <= list2->val)
    {
        Node *mergedRemaining = mergeTwoSortedListRecursive(list1->next, list2);
        list1->next = mergedRemaining;
        return list1;
    }

    Node *mergedRemaining = mergeTwoSortedListRecursive(list1, list2->next);
    list2->next = mergedRemaining;
    return list2;
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

int main()
{
    Node *head1 = nullptr;
    Node *tail1 = nullptr;
    Node *head2 = nullptr;
    Node *tail2 = nullptr;

    insertAtTail(head1, tail1, 10);
    insertAtTail(head1, tail1, 20);
    insertAtTail(head1, tail1, 30);
    insertAtTail(head1, tail1, 40);
    insertAtTail(head1, tail1, 50);
    insertAtTail(head1, tail1, 60);

    cout << "The Sorted List No 1 is: ";
    printLinkedList(head1);

    insertAtTail(head2, tail2, 15);
    insertAtTail(head2, tail2, 25);
    insertAtTail(head2, tail2, 35);
    insertAtTail(head2, tail2, 45);
    insertAtTail(head2, tail2, 65);

    cout << "The Sorted List No 2 is: ";
    printLinkedList(head2);

    cout << "After merging the two sorted list: ";
    // Node *mergedHead = mergeTwoSortedList(head1, head2);
    Node *mergedHead = mergeTwoSortedListRecursive(head1, head2);
    printLinkedList(mergedHead);

    return 0;
}