/**
 * @file Stack_LinkedList.cpp
 * @brief Enterprise-grade Linked List implementation of a Stack.
 * @details Features Strict O(1) operations, Rule of Three, Const Correctness,
 *          and the Canonical Copy-and-Swap Idiom for Strong Exception Safety.
 */

#include <iostream>
#include <stdexcept>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::swap;
using std::underflow_error;

struct Node
{
    int data;
    Node *next;

    explicit Node(int val) : data(val), next(nullptr) {}
};

class Stack
{
private:
    Node *head;
    size_t currentSize;

    void clear()
    {
        while (head != nullptr)
        {
            Node *temp = head;
            head = head->next;
            delete temp;
        }
        currentSize = 0;
    }

public:
    Stack() : head(nullptr), currentSize(0) {}

    ~Stack()
    {
        clear();
    }

    Stack(const Stack &other) : head(nullptr), currentSize(0)
    {
        if (other.head == nullptr)
        {
            return;
        }

        head = new Node(other.head->data);
        Node *currentNew = head;
        Node *currentOther = other.head->next;
        currentSize = 1;

        while (currentOther != nullptr)
        {
            currentNew->next = new Node(currentOther->data);
            currentNew = currentNew->next;
            currentOther = currentOther->next;
            currentSize++;
        }
    }

    Stack &operator=(Stack temp)
    {
        swap(head, temp.head);
        swap(currentSize, temp.currentSize);

        return *this;
    }

    void push(int val)
    {
        Node *newNode = new Node(val);
        newNode->next = head;
        head = newNode;
        currentSize++;
    }

    void pop()
    {
        if (empty())
        {
            throw underflow_error("Error: Stack Underflow! Cannot pop from an empty stack.");
        }
        Node *temp = head;
        head = head->next;
        delete temp;
        currentSize--;
    }

    int top() const
    {
        if (empty())
        {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        return head->data;
    }

    bool empty() const
    {
        return head == nullptr;
    }

    size_t size() const
    {
        return currentSize;
    }
};

int main()
{
    try
    {
        Stack s1;
        s1.push(10);
        s1.push(20);
        s1.push(30);

        cout << "--- s1 Stack (Linked List) ---" << endl;
        cout << "s1 Top: " << s1.top() << endl;   // 30
        cout << "s1 Size: " << s1.size() << endl; // 3

        // Copy Constructor Test (Deep Copy)
        Stack s2 = s1;
        cout << "\n--- s2 Stack (Copied from s1) ---" << endl;
        cout << "s2 Top: " << s2.top() << endl; // 30

        // Modify s1 to ensure s2 remains unaffected
        s1.pop();
        cout << "\nAfter popping s1:" << endl;
        cout << "s1 Top: " << s1.top() << endl;         // 20
        cout << "s2 Top remains: " << s2.top() << endl; // 30 (Unaffected)

        // Canonical Copy Assignment Operator Test
        Stack s3;
        s3.push(999);
        s3 = s1; // The old memory of s3 (999) is safely destroyed by temp's destructor
        cout << "\n--- s3 Stack (Assigned from s1) ---" << endl;
        cout << "s3 Top: " << s3.top() << endl; // 20

        // Error Handling Test
        cout << "\n--- Underflow Exception Testing ---" << endl;
        Stack emptyStack;
        emptyStack.pop(); // Triggers the underflow exception
    }
    catch (const exception &e)
    {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}