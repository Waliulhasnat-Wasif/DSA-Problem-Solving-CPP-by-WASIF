#include <iostream>
using std ::cout;
using std ::endl;

class MyLinkedList
{
private:
    class Node
    {
    public:
        int val;
        Node *prev;
        Node *next;

        explicit Node(int v)
        {
            val = v;
            prev = nullptr;
            next = nullptr;
        }
    };

    Node *dummyHead;
    Node *dummyTail;
    int size;

public:
    MyLinkedList()
    {
        size = 0;
        dummyHead = new Node(-1);
        dummyTail = new Node(-1);

        dummyHead->next = dummyTail;
        dummyTail->prev = dummyHead;
    }

    ~MyLinkedList()
    {
        Node *curr = dummyHead;

        while (curr != nullptr)
        {
            Node *deleteNode = curr;
            curr = curr->next;
            delete deleteNode;
        }
    }

    int get(int index)
    {
        if (index < 0 || index >= size)
            return -1;

        Node *curr = dummyHead->next;

        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }

        return curr->val;
    }

    void addAtHead(int val)
    {
        Node *newNode = new Node(val);
        Node *nextNode = dummyHead->next;

        dummyHead->next = newNode;
        newNode->prev = dummyHead;

        newNode->next = nextNode;
        nextNode->prev = newNode;

        size++;
    }

    void addAtTail(int val)
    {
        Node *newNode = new Node(val);
        Node *prevNode = dummyTail->prev;

        prevNode->next = newNode;
        newNode->prev = prevNode;

        newNode->next = dummyTail;
        dummyTail->prev = newNode;

        size++;
    }

    void addAtIndex(int index, int val)
    {
        if (index < 0 || index > size)
            return;

        if (index == 0)
        {
            addAtHead(val);
            return;
        }

        if (index == size)
        {
            addAtTail(val);
            return;
        }

        Node *curr = dummyHead->next;

        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }

        Node *newNode = new Node(val);
        Node *prevNode = curr->prev;

        prevNode->next = newNode;
        newNode->prev = prevNode;

        newNode->next = curr;
        curr->prev = newNode;

        size++;
    }

    void deleteAtIndex(int index)
    {
        if (index < 0 || index >= size)
            return;

        Node *curr = dummyHead->next;

        for (int i = 0; i < index; i++)
        {
            curr = curr->next;
        }

        Node *prevNode = curr->prev;
        Node *nextNode = curr->next;

        prevNode->next = nextNode;
        nextNode->prev = prevNode;

        delete curr;
        size--;
    }

    void printLinkedList()
    {
        Node *curr = dummyHead->next;

        while (curr != dummyTail)
        {
            cout << curr->val << " -> ";
            curr = curr->next;
        }

        cout << "NULL" << endl;
    }

    int getSize()
    {
        return size;
    }
};

int main()
{
    MyLinkedList linkedList;

    cout << "Initially: ";
    linkedList.printLinkedList();

    linkedList.addAtHead(1);
    cout << "After adding 1 at head: ";
    linkedList.printLinkedList();

    linkedList.addAtTail(3);
    cout << "After adding 3 at tail: ";
    linkedList.printLinkedList();

    linkedList.addAtIndex(1, 2);
    cout << "After adding 2 at [index 1]: ";
    linkedList.printLinkedList();

    cout << "The value of [index 1] is: " << linkedList.get(1) << endl;
    cout << "Current Size: " << linkedList.getSize() << endl;

    // Additional edge-case tests
    linkedList.addAtIndex(2, 5);
    cout << "After adding 5 at [index 2]: ";
    linkedList.printLinkedList();

    linkedList.addAtIndex(10, 100);
    cout << "After invalidly adding 100 at [index 10]: ";
    linkedList.printLinkedList();

    linkedList.deleteAtIndex(2);
    cout << "After deleting the node at [index 2]: ";
    linkedList.printLinkedList();

    linkedList.deleteAtIndex(2);
    cout << "After deleting the tail at [index 2]: ";
    linkedList.printLinkedList();

    linkedList.deleteAtIndex(0);
    linkedList.deleteAtIndex(0);
    cout << "After deleting all nodes: ";
    linkedList.printLinkedList();

    cout << "The value of index 0 is: " << linkedList.get(0) << endl;

    return 0;
}