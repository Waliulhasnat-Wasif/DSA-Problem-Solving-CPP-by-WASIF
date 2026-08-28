#include <iostream>
#include <stdexcept>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;
using std::underflow_error;

class Stack
{
private:
    int *arr;
    int capacity;
    int currentSize;

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
    explicit Stack(int initialCapacity = 2) : capacity(initialCapacity), currentSize(0)
    {
        if (initialCapacity <= 0)
        {
            throw invalid_argument("Stack capacity must be greater than zero.");
        }
        arr = new int[capacity];
    }

    ~Stack()
    {
        delete[] arr;
    }

    Stack(const Stack &other) : capacity(other.capacity), currentSize(other.currentSize)
    {
        arr = new int[capacity];

        for (int i = 0; i < currentSize; i++)
        {
            arr[i] = other.arr[i];
        }
    }

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

    void push(int val)
    {
        if (currentSize == capacity)
        {
            resize();
        }
        arr[currentSize] = val;
        currentSize++;
    }

    void pop()
    {
        if (currentSize == 0)
        {
            throw underflow_error("Error: Stack Underflow! Cannot pop from an empty stack.");
        }
        currentSize--;
    }

    int top() const
    {
        if (currentSize == 0)
        {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        return arr[currentSize - 1];
    }

    bool empty() const
    {
        return currentSize == 0;
    }

    int size() const
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