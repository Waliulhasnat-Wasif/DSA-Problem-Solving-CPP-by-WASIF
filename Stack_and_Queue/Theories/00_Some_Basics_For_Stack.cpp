#include <iostream>
#include <stdexcept>

using namespace std;

class Stack
{
private:
    int *arr;
    int capacity;
    int topIndex;

    // কনসেপ্ট ৩ (Amortized Complexity): মেমরি ফুল হলে সাইজ দ্বিগুণ করা
    void resize()
    {
        int oldCapacity = capacity;
        capacity = capacity * 2;

        cout << "\n  [Resize Triggered] -> Amortized Complexity Strategy" << endl;
        cout << "  -> Doubling capacity from " << oldCapacity << " to " << capacity << endl;

        // কনসেপ্ট ২ (Strong Exception Safety): পুরাতন মেমরি ডিলিট করার আগেই নতুন মেমরি নেওয়া
        int *temp_arr = new int[capacity];
        cout << "  -> [Safe Zone Step] Temporary large memory allocated at: " << temp_arr << endl;

        for (int i = 0; i <= topIndex; i++)
        {
            temp_arr[i] = arr[i];
        }

        cout << "  -> Deleting old small memory at: " << arr << endl;
        delete[] arr; // মেমরি ক্লিনিং

        arr = temp_arr;
        cout << "  -> Resize complete. Amortized cost paid. Next pushes will be O(1).\n"
             << endl;
    }

public:
    // সাধারণ কনস্ট্রাক্টর
    Stack(int size)
    {
        capacity = size;
        arr = new int[capacity];
        topIndex = -1;
        cout << "[Constructor] Memory allocated at Heap: " << arr << " (Capacity: " << capacity << ")" << endl;
    }

    // ১. Rule of Three: ডেসট্রাক্টর (Memory Clean up)
    ~Stack()
    {
        cout << "[Destructor] Freeing memory blocks from: " << arr << endl;
        delete[] arr;
    }

    // ২. Rule of Three: কপি কনস্ট্রাক্টর (Deep Copy on creation)
    // কনসেপ্ট ৪ (Const Correctness): parameter 'other' কে const& করা হয়েছে যেন রিড-অনলি এক্সেস থাকে
    Stack(const Stack &other)
    {
        capacity = other.capacity;
        topIndex = other.topIndex;

        arr = new int[capacity];
        cout << "[Copy Constructor] Deep Copying! New independent memory created at: " << arr << endl;

        for (int i = 0; i <= topIndex; i++)
        {
            arr[i] = other.arr[i];
        }
    }

    // ৩. Rule of Three: কপি অ্যাসাইনমেন্ট অপারেটর (Deep Copy on existing object)
    // কনসেপ্ট ২ (Strong Exception Safety): মেমরি লিক ও ক্র্যাশ এড়াতে আগে টেম্পোরারি অ্যালোকেশন
    Stack &operator=(const Stack &other)
    {
        cout << "[Copy Assignment Operator] Triggered!" << endl;

        if (this == &other)
        {
            cout << "  -> Self-assignment detected (s1 = s1). Skipping to prevent self-destruction." << endl;
            return *this;
        }

        cout << "  -> [Strong Exception Safety] Allocating new memory BEFORE deleting old memory..." << endl;
        int *temp_arr = new int[other.capacity];
        cout << "  -> New memory safely ready at: " << temp_arr << endl;

        for (int i = 0; i <= other.topIndex; i++)
        {
            temp_arr[i] = other.arr[i];
        }

        cout << "  -> Safe Zone reached. Now deleting old memory at: " << this->arr << endl;
        delete[] arr;

        arr = temp_arr;
        capacity = other.capacity;
        topIndex = other.topIndex;

        return *this;
    }

    // পুশ অপারেশন
    void push(int val)
    {
        if (topIndex == capacity - 1)
        {
            resize();
        }
        arr[++topIndex] = val;
        cout << "  Pushed: " << val << " at index " << topIndex << " (Size: " << (topIndex + 1) << "/" << capacity << ")" << endl;
    }

    // =========================================================================
    // কনসেপ্ট ৪: Const Correctness (Read-Only Methods)
    // ফাংশনগুলোর শেষে 'const' মেডেল দেওয়া হয়েছে। এরা ক্লাসের কোনো মেমরি বদলাতে পারবে না।
    // =========================================================================

    void printTop() const
    {
        cout << "  [Const Method - printTop() called]" << endl;
        if (topIndex >= 0)
        {
            cout << "  -> Top element is: " << arr[topIndex] << endl;
        }
        else
        {
            cout << "  -> Stack is empty!" << endl;
        }
        // topIndex++; // <- এই লাইনটি আনকমেন্ট করলে কম্পাইল এরর আসবে! (নিরাপত্তা দেয়াল)
    }

    int size() const
    {
        cout << "  [Const Method - size() called]" << endl;
        return (topIndex + 1);
    }
};

// এই গ্লোবাল ফাংশনটি আমাদের Const Correctness টেস্ট করতে সাহায্য করবে
// এখানে স্ট্যাকটিকে কপি না করে সরাসরি 'Alias' বা রেফারেন্সে আনা হয়েছে এবং 'const' লক করে দেওয়া হয়েছে
void printStackDiagnostics(const Stack &s)
{
    cout << "\n=== [Global Function] Diagnostics Triggered with a CONST Reference Stack ===" << endl;

    // s.push(999); // ERROR! অবজেক্টটি লক করা (const), তাই ডেটা পরিবর্তনকারী push() কল করা নিষিদ্ধ।

    // শুধু const মেডেল ওয়ালা Read-Only মেথডগুলোই এখান থেকে সফলভাবে কল হবে:
    cout << "Diagnostic - Stack Size: " << s.size() << endl;
    s.printTop();
    cout << "=== Diagnostics Complete ===\n"
         << endl;
}

int main()
{
    cout << "===== POINT 1 & 3: The Rule of Three & Amortized Complexity =====" << endl;
    Stack s1(2); // শুরুতে ধারণক্ষমতা মাত্র ২
    s1.push(10);
    s1.push(20);
    s1.push(30); // ধারণক্ষমতা ফুল হওয়ায় এখানে Amortized Resize (দ্বিগুণ) ট্রিগার হবে।

    cout << "\n===== POINT 1: Copy Constructor (Birth of s2 from s1) =====" << endl;
    Stack s2 = s1; // নতুন অবজেক্ট তৈরির সময় কপি (Copy Constructor)

    cout << "\n===== POINT 1 & 2: Copy Assignment & Strong Exception Safety =====" << endl;
    Stack s3(5);
    s3.push(500);
    s3 = s1; // পুরাতন অবজেক্টে কপি। আগে টেম্প মেমরি তৈরি হবে, পরে পুরাতন মেমরি ডিলিট হবে।

    cout << "\n===== POINT 4: Const Correctness Testing =====" << endl;
    // s1 স্ট্যাকটিকে আমরা একটি const রেফারেন্স ফাংশনে পাঠাচ্ছি
    printStackDiagnostics(s1);

    cout << "===== PROGRAM ENDING: Destructors will clean everything up =====" << endl;
    return 0;
}
