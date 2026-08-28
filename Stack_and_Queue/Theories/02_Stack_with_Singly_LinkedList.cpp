#include <iostream>
#include <stdexcept>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::move;
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

    Stack(Stack &&other) noexcept : head(other.head), currentSize(other.currentSize)
    {
        other.head = nullptr;
        other.currentSize = 0;
    }

    friend void swap(Stack &first, Stack &second) noexcept
    {
        Node *tempHead = first.head;
        first.head = second.head;
        second.head = tempHead;

        size_t tempSize = first.currentSize;
        first.currentSize = second.currentSize;
        second.currentSize = tempSize;
    }

    Stack &operator=(Stack temp)
    {
        swap(*this, temp);

        return *this;
    }

    /*Stack& operator=(const Stack& other) {
        if (this == &other) {
            return *this;
        }

        // Create a temporary deep copy. If 'new' fails here, the original object remains untouched.
        Stack temp(other);

        // Swap pointers and sizes
        swap(head, temp.head);
        swap(currentSize, temp.currentSize);

        // temp's destructor will automatically clean up the old memory of this object
        return *this;
    }
    */

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

        // Move Constructor Test (O(1) Ownership Transfer)
        Stack s4 = move(s2);
        cout << "\n--- s4 Stack (Moved from s2) ---" << endl;
        cout << "s4 Top: " << s4.top() << endl;   // 30
        cout << "s4 Size: " << s4.size() << endl; // 3
        cout << "s2 Size after move: " << s2.size() << " (Empty: " << (s2.empty() ? "Yes" : "No") << ")" << endl;

        // Move Assignment Operator Test (O(1) Ownership Transfer)
        Stack s5;
        s5.push(888);
        s5 = move(s3); // s5 gets s3's data, old memory of s5 is safely destroyed
        cout << "\n--- s5 Stack (Move-Assigned from s3) ---" << endl;
        cout << "s5 Top: " << s5.top() << endl;   // 20
        cout << "s5 Size: " << s5.size() << endl; // 2
        cout << "s3 Size after move: " << s3.size() << " (Empty: " << (s3.empty() ? "Yes" : "No") << ")" << endl;

        // Custom Friend Swap Test (Outside Call)
        s1.push(555);
        cout << "\n--- Direct Friend Swap Test (s1 <-> s5) ---" << endl;
        cout << "Before Swap -> s1 Top: " << s1.top() << ", s5 Top: " << s5.top() << endl;
        swap(s1, s5); // Direct non-member call
        cout << "After Swap  -> s1 Top: " << s1.top() << ", s5 Top: " << s5.top() << endl;

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