#include <iostream>
#include <stdexcept>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::underflow_error;

class Stack
{
private:
    int *arr;
    int capacity;
    int currentSize;

    // Dynamic memory resizing logic
    void resize()
    {
        int newCapacity = capacity * 2;
        int *newArr = new int[newCapacity];

        for (int i = 0; i < currentSize; i++)
        {
            newArr[i] = arr[i];
        }

        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }

public:
    // 1. Default Constructor
    Stack()
    {
        capacity = 2;
        currentSize = 0;
        arr = new int[capacity];
    }

    // 2. Destructor
    ~Stack()
    {
        delete[] arr;
    }

    // 3. Copy Constructor (Deep Copy)
    Stack(const Stack &other)
    {
        capacity = other.capacity;
        currentSize = other.currentSize;
        arr = new int[capacity];

        for (int i = 0; i < currentSize; i++)
        {
            arr[i] = other.arr[i];
        }
    }

    // 4. Copy Assignment Operator (Strong Exception Safety)
    Stack &operator=(const Stack &other)
    {
        if (this == &other)
        {
            return *this;
        }

        int *newArr = new int[other.capacity];
        for (int i = 0; i < other.currentSize; i++)
        {
            newArr[i] = other.arr[i];
        }

        delete[] arr;

        arr = newArr;
        capacity = other.capacity;
        currentSize = other.currentSize;

        return *this;
    }

    // 5. Push Operation
    void push(int val)
    {
        if (currentSize == capacity)
        {
            resize();
        }
        arr[currentSize] = val;
        currentSize++;
    }

    // 6. Pop Operation
    void pop()
    {
        if (currentSize == 0)
        {
            throw underflow_error("Error: Stack Underflow! Cannot pop from an empty stack.");
        }
        currentSize--;
    }

    // 7. Top Operation (Read-only -> const)
    int top() const
    {
        if (currentSize == 0)
        {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        return arr[currentSize - 1];
    }

    // 8. Empty Check (Read-only -> const)
    bool empty() const
    {
        return currentSize == 0;
    }

    // 9. Size Check (Read-only -> const)
    int size() const
    {
        return currentSize;
    }
};

// ==========================================
// Test Driver
// ==========================================
int main()
{
    try
    {
        Stack s1;
        s1.push(10);
        s1.push(20);
        s1.push(30);

        cout << "--- s1 Stack ---" << endl;
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

        // Error Handling Test
        cout << "\n--- Underflow Exception Testing ---" << endl;
        Stack emptyStack;
        emptyStack.pop(); // Triggers the underflow exception
    }
    catch (const exception &e)
    {
        cerr << e.what() << endl;
    }

    return 0;
}