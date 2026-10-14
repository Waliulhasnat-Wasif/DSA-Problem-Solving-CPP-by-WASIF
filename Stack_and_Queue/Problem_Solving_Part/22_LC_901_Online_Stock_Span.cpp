#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::pair;
using std::string;
using std::vector;

class StockSpanner {
public:
    StockSpanner() {
        st.reserve(100000);
    }

    int next(int price) {
        int current_span = 1;

        while (!st.empty() && st.back().first <= price) {
            current_span += st.back().second;
            st.pop_back();
        }
        st.emplace_back(price, current_span);

        return current_span;
    }

private:
    vector<pair<int, int>> st;
};

string format_vector(const vector<int>& v) {
    if (v.empty()) {
        return "[]";
    }

    string res = "[";
    res.reserve(v.size() * 6 + 2);

    for (size_t i = 0; i < v.size() - 1; ++i) {
        res += std::to_string(v[i]);
        res += ", ";
    }

    res += std::to_string(v.back());
    res += "]";
    return res;
}

void runTest(const string& test_name, const vector<int>& prices, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Prices:  " << format_vector(prices) << "\n";

    StockSpanner spanner;
    vector<int> res;
    res.reserve(prices.size());

    for (int p : prices) {
        res.push_back(spanner.next(p));
    }

    cout << "Expected: " << format_vector(expected) << "\n";
    cout << "Output:   " << format_vector(res) << "\n";
    cout << "Result:   " << (res == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 901: Online Stock Span (Interview Mode) ---\n\n";

    try {
        runTest("Example 1 (Standard Fluctuations)", {100, 80, 60, 70, 60, 75, 85}, {1, 1, 1, 2, 1, 4, 6});

        runTest("Example 2 (Strict Ascending)", {10, 20, 30, 40}, {1, 2, 3, 4});

        runTest("Edge Case 1 (Strict Descending)", {50, 40, 30, 20}, {1, 1, 1, 1});

        runTest("Edge Case 2 (Identical Prices)", {10, 10, 10, 10}, {1, 2, 3, 4});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}