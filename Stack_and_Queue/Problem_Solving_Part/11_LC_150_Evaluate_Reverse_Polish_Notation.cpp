#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::domain_error;
using std::endl;
using std::exception;
using std::stack;
using std::string;
using std::underflow_error;
using std::vector;

class SolutionBaseline {
public:
    int evalRPN(const vector<string>& tokens) const {
        stack<int> st;

        for (const string& token : tokens) {
            if (token == "+" || token == "-" || token == "*" || token == "/") {
                if (st.size() < 2) {
                    throw underflow_error("Error: Invalid RPN expression.");
                }
                int right_operand = st.top();
                st.pop();

                int left_operand = st.top();
                st.pop();

                if (token == "+")
                    st.push(left_operand + right_operand);
                else if (token == "-")
                    st.push(left_operand - right_operand);
                else if (token == "*")
                    st.push(left_operand * right_operand);
                else if (token == "/") {
                    if (right_operand == 0) {
                        throw domain_error("Error: Division by zero.");
                    }
                    st.push(left_operand / right_operand);
                }
            } else {
                st.push(std::stoi(token));
            }
        }

        return st.top();
    }
};

class SolutionOptimized {
public:
    int evalRPN(const vector<string>& tokens) const {
        vector<int> st;

        st.reserve((tokens.size() / 2) + 1);

        for (const string& token : tokens) {
            if (token.length() == 1 && token[0] < '0') {
                if (st.size() < 2) {
                    throw underflow_error("Error: Invalid RPN expression.");
                }

                int right_operand = st.back();
                st.pop_back();

                int left_operand = st.back();
                st.pop_back();

                switch (token[0]) {
                    case '+':
                        st.push_back(left_operand + right_operand);
                        break;
                    case '-':
                        st.push_back(left_operand - right_operand);
                        break;
                    case '*':
                        st.push_back(left_operand * right_operand);
                        break;
                    case '/':
                        if (right_operand == 0) {
                            throw domain_error("Error: Division by zero.");
                        }
                        st.push_back(left_operand / right_operand);
                        break;
                }
            } else {
                st.push_back(std::stoi(token));
            }
        }

        return st.back();
    }
};

void runComparativeTest(const string& test_name, const vector<string>& tokens, int expected) {
    cout << "Test Case: " << test_name << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    int res_opt = sol_opt.evalRPN(tokens);
    int res_base = sol_base.evalRPN(tokens);

    cout << "Expected: " << expected << "\n";
    cout << "Optimized Output: " << res_opt << "\n";
    cout << "Baseline Output:  " << res_base << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 150: Evaluate Reverse Polish Notation ---\n\n";

    try {
        // 1. Example 1: Basic Operations
        runComparativeTest("Example 1 (Basic Operations)", {"2", "1", "+", "3", "*"}, 9);

        // 2. Example 2: Integer Division Truncation
        runComparativeTest("Example 2 (Division Truncation)", {"4", "13", "5", "/", "+"}, 6);

        // 3. Example 3: Complex Nested Expression with Negative Numbers
        runComparativeTest("Example 3 (Complex Nested & Negative)", {"10", "6", "9", "3", "+", "-11", "*", "/", "*", "17", "+", "5", "+"}, 22);

        // 4. Edge Case: Single Element Array
        runComparativeTest("Edge Case 1 (Single Number)", {"18"}, 18);

        // 5. Edge Case: Deep Stack Depth
        // 2 - (3 + (4 * 5)) = 2 - 23 = -21
        runComparativeTest("Edge Case 2 (Deep Stack Unloading)", {"2", "3", "4", "5", "*", "+", "-"}, -21);

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}