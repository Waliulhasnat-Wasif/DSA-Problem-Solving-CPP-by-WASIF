#include <cctype>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::isdigit;
using std::stack;
using std::string;
using std::underflow_error;
using std::vector;

class SolutionBaseline {
public:
    string decodeString(const string& s) const {
        int index = 0;
        return decodeRecursive(s, index);
    }

private:
    string decodeRecursive(const string& s, int& index) const {
        string res;
        int k = 0;
        int n = static_cast<int>(s.length());

        while (index < n) {
            char c = s[index];
            if (isdigit(c)) {
                k = k * 10 + (c - '0');
                index++;
            } else if (c == '[') {
                index++;  // Skip '['
                // Recursive context switch
                string decoded_inner = decodeRecursive(s, index);
                while (k > 0) {
                    res += decoded_inner;
                    k--;
                }
            } else if (c == ']') {
                index++;  // Skip ']' and return to outer context
                return res;
            } else {
                res += c;
                index++;
            }
        }
        return res;
    }
};

class SolutionOptimized {
public:
    string decodeString(const string& s) const {
        vector<int> count_stack;
        vector<string> string_stack;

        string current_string = "";
        int k = 0;

        size_t n = s.length();

        for (size_t i = 0; i < n; ++i) {
            char c = s[i];

            if (isdigit(c)) {
                k = k * 10 + (c - '0');
            } else if (c == '[') {
                count_stack.push_back(k);
                string_stack.push_back(std::move(current_string));

                current_string = "";
                k = 0;
            } else if (c == ']') {
                if (string_stack.empty() || count_stack.empty()) {
                    throw underflow_error("Error: Malformed encoded string.");
                }

                string prev_string = std::move(string_stack.back());
                string_stack.pop_back();

                int repeat_times = count_stack.back();
                count_stack.pop_back();

                size_t total_size = prev_string.size() + (current_string.size() * repeat_times);
                prev_string.reserve(total_size);

                for (int j = 0; j < repeat_times; ++j) {
                    prev_string += current_string;
                }

                current_string = std::move(prev_string);
            } else {
                current_string += c;
            }
        }

        return current_string;
    }
};

void runComparativeTest(const string& test_name, const string& s, const string& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Encoded String: \"" << s << "\"\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    string res_opt = sol_opt.decodeString(s);
    string res_base = sol_base.decodeString(s);

    cout << "Expected:   \"" << expected << "\"\n";
    cout << "Optimized:  \"" << res_opt << "\"\n";
    cout << "Baseline:   \"" << res_base << "\"\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LeetCode 394: Decode String ---\n\n";

    try {
        runComparativeTest("Example 1 (Sequential)", "3[a]2[bc]", "aaabcbc");

        runComparativeTest("Example 2 (Nested)", "3[a2[c]]", "accaccacc");

        runComparativeTest("Example 3 (Mixed Plain)", "2[abc]3[cd]ef", "abcabccdcdcdef");

        runComparativeTest("Edge Case 1 (Multi-digit)", "10[a]", "aaaaaaaaaa");

        runComparativeTest("Edge Case 2 (Deeply Nested)", "2[2[2[b]]]", "bbbbbbbb");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}