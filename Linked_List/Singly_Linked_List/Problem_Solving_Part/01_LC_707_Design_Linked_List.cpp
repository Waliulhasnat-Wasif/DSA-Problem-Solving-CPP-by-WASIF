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
        Node *next;

        explicit Node(int v)
        {
            val = v;
            next = nullptr;
        }
    };

    Node *head;
    Node *tail;
    int size;

public:
    MyLinkedList()
    {
        head = nullptr;
        tail = nullptr;
        size = 0;
    }

    ~MyLinkedList()
    {
        Node *curr = head;

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
        {
            return -1;
        }

        Node *temp = head;

        for (int i = 0; i < index; i++)
        {
            temp = temp->next;
        }

        return temp->val;
    }

    void addAtHead(int val)
    {
        Node *newNode = new Node(val);

        newNode->next = head;
        head = newNode;

        if (tail == nullptr)
            tail = newNode;

        size++;
    }

    void addAtTail(int val)
    {
        Node *newNode = new Node(val);

        if (head == nullptr)
        {
            head = newNode;
            tail = newNode;
            size++;
            return;
        }

        tail->next = newNode;
        tail = newNode;
        size++;
    }

    void addAtIndex(int index, int val)
    {
        if (index < 0 || index > size)
        {
            return;
        }

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

        Node *temp = head;

        for (int i = 1; i < index; i++)
        {
            temp = temp->next;
        }

        Node *newNode = new Node(val);
        newNode->next = temp->next;
        temp->next = newNode;

        size++;
    }

    void deleteAtIndex(int index)
    {
        if (index < 0 || index >= size)
        {
            return;
        }

        if (index == 0)
        {
            Node *deleteNode = head;
            head = head->next;
            delete deleteNode;

            size--;

            if (size == 0)
            {
                tail = nullptr;
            }

            return;
        }

        Node *temp = head;

        for (int i = 1; i < index; i++)
        {
            temp = temp->next;
        }

        Node *deleteNode = temp->next;
        temp->next = deleteNode->next;

        if (deleteNode == tail)
        {
            tail = temp;
        }

        delete deleteNode;
        size--;
    }

    void printLinkedList()
    {
        Node *temp = head;

        while (temp != nullptr)
        {
            cout << temp->val << " -> ";
            temp = temp->next;
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