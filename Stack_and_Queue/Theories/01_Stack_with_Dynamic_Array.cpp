#include <iostream>
#include <stdexcept>
#include <utility>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;
using std::move;
using std::underflow_error;

class Stack
{
private:
    int *arr;
    size_t capacity;
    size_t currentSize;

    void resize()
    {
        size_t newCapacity = capacity * 2;
        int *newArr = new int[newCapacity];

        for (size_t i = 0; i < currentSize; i++)
        {
            newArr[i] = arr[i];
        }

        delete[] arr;
        arr = newArr;
        capacity = newCapacity;
    }

public:
    // 1. Constructor (with Member Initializer List and Fail-Fast Guard)
    explicit Stack(size_t initialCapacity = 2) : capacity(initialCapacity), currentSize(0)
    {
        if (initialCapacity == 0)
        {
            throw invalid_argument("Stack capacity must be greater than zero.");
        }
        arr = new int[capacity];
    }

    // 2. Destructor
    ~Stack()
    {
        delete[] arr;
    }

    // 3. Copy Constructor (Deep Copy)
    Stack(const Stack &other) : capacity(other.capacity), currentSize(other.currentSize)
    {
        arr = new int[capacity];
        for (size_t i = 0; i < currentSize; i++)
        {
            arr[i] = other.arr[i];
        }
    }

    // 4. Move Constructor (O(1) Ownership Transfer - Rule of Five)
    Stack(Stack &&other) noexcept : arr(other.arr), capacity(other.capacity), currentSize(other.currentSize)
    {
        // Nullify the source object to prevent its destructor from freeing the memory
        other.arr = nullptr;
        other.capacity = 0;
        other.currentSize = 0;
    }

    // 5. Custom Swap Function (Idiomatic C++ for ADL)
    friend void swap(Stack &first, Stack &second) noexcept
    {
        // Swapping all internal members safely
        int *tempArr = first.arr;
        first.arr = second.arr;
        second.arr = tempArr;

        size_t tempCap = first.capacity;
        first.capacity = second.capacity;
        second.capacity = tempCap;

        size_t tempSize = first.currentSize;
        first.currentSize = second.currentSize;
        second.currentSize = tempSize;
    }

    /*
    friend void swap(Stack &first, Stack &second) noexcept
    {
        // For Enabling ADL (Argument-Dependent Lookup)
        using std::swap;

        // arr pointer swap (changing dynamic array ownership)
        swap(first.arr, second.arr);

        // ২. capacity swap
        swap(first.capacity, second.capacity);

        // ৩. currentSize swap
        swap(first.currentSize, second.currentSize);
    }
    */

    // 6. Unified Assignment Operator (Canonical Copy-and-Swap)
    // Handles BOTH Copy Assignment (by taking l-value) and Move Assignment (by taking r-value) safely.
    Stack &operator=(Stack temp)
    {
        swap(*this, temp);
        return *this;
        // temp's destructor cleans up the old memory of 'this' when it goes out of scope
    }

    /*
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
    */

    // Push Operation -> Amortized O(1) Time
    void push(int val)
    {
        if (currentSize == capacity)
        {
            resize();
        }
        arr[currentSize] = val;
        currentSize++;
    }

    // Pop Operation -> O(1) Time
    void pop()
    {
        if (currentSize == 0)
        {
            throw underflow_error("Error: Stack Underflow! Cannot pop from an empty stack.");
        }
        currentSize--;
    }

    // Top Operation (Read-only -> const) -> O(1) Time
    int top() const
    {
        if (currentSize == 0)
        {
            throw underflow_error("Error: Stack is empty! No top element.");
        }
        return arr[currentSize - 1];
    }

    // Empty Check (Read-only -> const) -> O(1) Time
    bool empty() const
    {
        return currentSize == 0;
    }

    // Size Check (Read-only -> const) -> O(1) Time
    size_t size() const
    {
        return currentSize;
    }

    // Capacity Check (Optional)
    size_t getCapacity() const
    {
        return capacity;
    }
};

int main()
{
    try
    {
        Stack s1;
        s1.push(10);
        s1.push(20);
        s1.push(30); // Triggers resize

        cout << "--- s1 Stack (Dynamic Array) ---" << endl;
        cout << "s1 Top: " << s1.top() << endl;              // 30
        cout << "s1 Size: " << s1.size() << endl;            // 3
        cout << "s1 Capacity: " << s1.getCapacity() << endl; // 4

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