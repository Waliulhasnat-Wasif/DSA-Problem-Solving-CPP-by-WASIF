#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::string;
using std::vector;

class SolutionBaseline {
public:
    vector<int> finalPrices(const vector<int>& prices) const {
        size_t n = prices.size();
        vector<int> res = prices;

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                if (prices[j] <= prices[i]) {
                    res[i] -= prices[j];
                    break;
                }
            }
        }
        return res;
    }
};

class SolutionExtreme {
public:
    vector<int> finalPrices(const vector<int>& prices) const {
        size_t n = prices.size();
        vector<int> res = prices;

        vector<size_t> st;
        st.reserve(n);

        for (size_t i = 0; i < n; ++i) {
            while (!st.empty() && prices[st.back()] >= prices[i]) {
                size_t waiting_idx = st.back();
                st.pop_back();

                res[waiting_idx] -= prices[i];
            }

            st.push_back(i);
        }

        return res;
    }
};

class SolutionExtreme_2 {
public:
    vector<int> finalPrices(const vector<int>& prices) const {
        size_t n = prices.size();
        vector<int> res(n, 0);

        vector<size_t> st;
        st.reserve(n);

        for (size_t i = n; i-- > 0;) {
            while (!st.empty() && prices[st.back()] > prices[i]) {
                st.pop_back();
            }

            int discount = st.empty() ? 0 : prices[st.back()];
            res[i] = prices[i] - discount;

            st.push_back(i);
        }

        return res;
    }
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

void runComparativeTest(const string& test_name, const vector<int>& prices, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input: prices = " << format_vector(prices) << "\n";

    SolutionExtreme sol_ext;
    SolutionExtreme_2 sol_ext2;
    SolutionBaseline sol_base;

    vector<int> res_ext = sol_ext.finalPrices(prices);
    vector<int> res_ext2 = sol_ext2.finalPrices(prices);
    vector<int> res_base = sol_base.finalPrices(prices);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Extreme:   " << format_vector(res_ext) << "\n";
    cout << "Extremev2: " << format_vector(res_ext2) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    bool passed = (res_ext == expected && res_base == expected && res_ext2 == expected);
    cout << "Result:    " << (passed ? "[PASS]" : "[FAIL]") << "\n";
    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 1475: Final Prices With Discount ---\n\n";

    try {
        runComparativeTest("Example 1 (Mixed Discounts)", {8, 4, 6, 2, 3}, {4, 2, 4, 2, 3});

        runComparativeTest("Example 2 (Strictly Ascending - No Discounts)", {1, 2, 3, 4, 5}, {1, 2, 3, 4, 5});

        runComparativeTest("Example 3 (Mixed with Equals)", {10, 1, 1, 6}, {9, 0, 1, 6});

        runComparativeTest("Edge Case (All Identical)", {5, 5, 5, 5}, {0, 0, 0, 5});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }
    return 0;
}