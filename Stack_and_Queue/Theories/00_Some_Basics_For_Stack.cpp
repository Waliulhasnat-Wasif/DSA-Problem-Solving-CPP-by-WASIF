#include <iostream>
using namespace std;

class Stack
{
private:
    int *arr;
    int capacity;
    int topIndex;

public:
    // সাধারণ কনস্ট্রাক্টর (Object Creation)
    Stack(int size)
    {
        capacity = size;
        arr = new int[capacity];
        topIndex = -1;
        cout << "[Constructor] New memory created at Heap: " << arr << endl;
    }

    // ১. ডেসট্রাক্টর (Memory Clean up)
    ~Stack()
    {
        cout << "[Destructor] Freeing memory blocks from: " << arr << endl;
        delete[] arr;
    }

    // ২. কপি কনস্ট্রাক্টর (নতুন অবজেক্ট তৈরিতে কপি)
    Stack(const Stack &other)
    {
        capacity = other.capacity;
        topIndex = other.topIndex;

        arr = new int[capacity]; // সম্পূর্ণ নতুন মেমরি লোকেশন
        cout << "[Copy Constructor] Deep Copying! New memory created at: " << arr << endl;

        for (int i = 0; i <= topIndex; i++)
        {
            arr[i] = other.arr[i];
        }
    }

    // ৩. কপি অ্যাসাইনমেন্ট অপারেটর (আগে থেকে তৈরি অবজেক্টে কপি)
    Stack &operator=(const Stack &other)
    {
        cout << "[Copy Assignment Operator] Triggered!" << endl;

        // সেলফ-অ্যাসাইনমেন্ট চেক
        if (this == &other)
        {
            cout << "  -> Self-assignment detected (s1 = s1). Safe exit." << endl;
            return *this;
        }

        // পুরাতন মেমরি রিলিজ (মেমরি লিক আটকানো)
        cout << "  -> Deleting old memory of this object at: " << this->arr << endl;
        delete[] arr;

        // নতুন মেমরি ও ডেটা কপি
        capacity = other.capacity;
        topIndex = other.topIndex;
        arr = new int[capacity];
        cout << "  -> Memory allocated for new data at: " << arr << endl;

        for (int i = 0; i <= topIndex; i++)
        {
            arr[i] = other.arr[i];
        }

        return *this;
    }

    void push(int val)
    {
        arr[++topIndex] = val;
    }

    void printTop() const
    {
        if (topIndex >= 0)
        {
            cout << "Top element is: " << arr[topIndex] << endl;
        }
    }
};

int main()
{
    cout << "=== STEP 1: Creating 's1' and 's2' Independently ===" << endl;
    Stack s1(5);
    s1.push(100);

    Stack s2(5);
    s2.push(500);

    cout << "\n=== STEP 2: Executing COPY CONSTRUCTOR (Stack s3 = s1) ===" << endl;
    // s3 আগে থেকে তৈরি ছিল না। সে মাত্র জন্ম নিচ্ছে এবং সাথে সাথে s1 এর কপি নিচ্ছে।
    // এখানে কল হবে: Copy Constructor
    Stack s3 = s1;
    cout << "s3 -> ";
    s3.printTop();

    cout << "\n=== STEP 3: Executing COPY ASSIGNMENT OPERATOR (s2 = s1) ===" << endl;
    // s2 অলরেডি STEP 1 এ তৈরি হয়ে মেমরি দখল করে বসে আছে।
    // এখন মাঝপথে তাকে s1 এর ডেটা দেওয়া হচ্ছে। তাই তার আগের মেমরি ডিলিট হতে হবে।
    // এখানে কল হবে: Copy Assignment Operator (=)
    s2 = s1;
    cout << "s2 -> ";
    s2.printTop();

    cout << "\n=== STEP 4: Testing Self-Assignment (s1 = s1) ===" << endl;
    // নিজের মেমরি নিজে ডিলিট করা থেকে বাঁচতে সেফটি গার্ড চেক হবে
    s1 = s1;

    cout << "\n=== STEP 5: Program Ending (Destructors will fire in reverse order) ===" << endl;
    // main ফাংশন শেষ। এবার s3, s2, s1 এর ডেসট্রাক্টর সিরিয়ালি কল হবে।
    return 0;
}
