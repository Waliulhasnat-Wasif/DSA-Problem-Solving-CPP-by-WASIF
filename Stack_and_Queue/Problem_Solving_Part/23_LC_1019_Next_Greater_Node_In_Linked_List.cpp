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

struct ListNode {
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    explicit ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

class SolutionBaseline {
public:
    vector<int> nextLargerNodes(ListNode* head) const {
        vector<int> values;
        for (ListNode* curr = head; curr != nullptr; curr = curr->next) {
            values.push_back(curr->val);
        }

        size_t n = values.size();
        vector<int> res(n, 0);

        for (size_t i = 0; i < n; ++i) {
            for (size_t j = i + 1; j < n; ++j) {
                if (values[j] > values[i]) {
                    res[i] = values[j];
                    break;
                }
            }
        }
        return res;
    }
};

class SolutionOptimized {
public:
    vector<int> nextLargerNodes(ListNode* head) const {
        vector<int> res;
        res.reserve(10000);

        vector<pair<size_t, int>> st;
        st.reserve(10000);

        size_t current_idx = 0;

        for (ListNode* curr = head; curr != nullptr; curr = curr->next) {
            res.push_back(0);

            while (!st.empty() && st.back().second < curr->val) {
                res[st.back().first] = curr->val;
                st.pop_back();
            }

            st.emplace_back(current_idx, curr->val);
            ++current_idx;
        }

        return res;
    }
};

ListNode* buildList(const vector<int>& vals) {
    if (vals.empty()) {
        return nullptr;
    }
    ListNode* head = new ListNode(vals[0]);
    ListNode* curr = head;
    for (size_t i = 1; i < vals.size(); ++i) {
        curr->next = new ListNode(vals[i]);
        curr = curr->next;
    }
    return head;
}

void freeList(ListNode* head) {
    while (head != nullptr) {
        ListNode* temp = head;
        head = head->next;
        delete temp;
    }
}

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

void runTest(const string& test_name, const vector<int>& list_vals, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input List: " << format_vector(list_vals) << "\n";

    ListNode* head = buildList(list_vals);

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    vector<int> res_opt = sol_opt.nextLargerNodes(head);
    vector<int> res_base = sol_base.nextLargerNodes(head);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Optimized: " << format_vector(res_opt) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    cout << "Result:    " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";

    freeList(head);
}

int main() {
    cout << "--- Testing LC 1019: Next Greater Node (Interview Mode) ---\n\n";

    try {
        runTest("Example 1 (Standard List)", {2, 1, 5}, {5, 5, 0});

        runTest("Example 2 (Longer Sequence)", {2, 7, 4, 3, 5}, {7, 0, 5, 5, 0});

        runTest("Edge Case 1 (Strictly Descending)", {9, 8, 7, 6}, {0, 0, 0, 0});

        runTest("Edge Case 2 (Identical Elements)", {3, 3, 3, 3}, {0, 0, 0, 0});

    } catch (const std::exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}