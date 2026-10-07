
#include <cstdlib>
#include <iostream>
#include <stack>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::stack;
using std::string;
using std::vector;

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

class SolutionBaseline {
public:
    vector<int> exclusiveTime(int n, const vector<string>& logs) const {
        vector<int> res(n, 0);

        vector<size_t> st;
        st.reserve(logs.size() / 2);

        int prev_time = 0;

        for (const string& log : logs) {
            size_t pos1 = log.find(':');
            size_t pos2 = log.find(':', pos1 + 1);

            size_t id = static_cast<size_t>(std::atoi(log.c_str()));
            std::string_view type(log.data() + pos1 + 1, pos2 - pos1 - 1);
            int time = std::atoi(log.c_str() + pos2 + 1);

            if (type == "start") {
                if (!st.empty()) {
                    res[st.back()] += time - prev_time;
                }
                st.push_back(id);
                prev_time = time;
            } else {
                res[st.back()] += time - prev_time + 1;
                st.pop_back();
                prev_time = time + 1;
            }
        }

        return res;
    }
};

class SolutionOptimized {
public:
    vector<int> exclusiveTime(int n, const vector<string>& logs) const {
        vector<int> res(n, 0);

        vector<int> st;

        st.reserve(logs.size() / 2);

        int prev_time = 0;

        for (const string& log : logs) {
            int id = 0;
            int time = 0;
            bool is_start = false;
            size_t i = 0;

            while (log[i] != ':') {
                id = id * 10 + (log[i] - '0');
                i++;
            }
            i++;

            if (log[i] == 's') {
                is_start = true;
                i += 6;
            } else {
                is_start = false;
                i += 4;
            }

            size_t len = log.length();
            while (i < len) {
                time = time * 10 + (log[i] - '0');
                i++;
            }

            if (is_start) {
                if (!st.empty()) {
                    res[static_cast<size_t>(st.back())] += time - prev_time;
                }
                st.push_back(id);
                prev_time = time;
            } else {
                res[static_cast<size_t>(st.back())] += time - prev_time + 1;
                st.pop_back();

                prev_time = time + 1;
            }
        }

        return res;
    }
};

void runComparativeTest(const string& test_name, int n, const vector<string>& logs, const vector<int>& expected) {
    cout << "Test Case: " << test_name << "\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    vector<int> res_opt = sol_opt.exclusiveTime(n, logs);
    vector<int> res_base = sol_base.exclusiveTime(n, logs);

    cout << "Expected:  " << format_vector(expected) << "\n";
    cout << "Optimized: " << format_vector(res_opt) << "\n";
    cout << "Baseline:  " << format_vector(res_base) << "\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 636: Exclusive Time of Functions ---\n\n";

    try {
        runComparativeTest("Example 1 (Basic Nesting)", 2, {"0:start:0", "1:start:2", "1:end:5", "0:end:6"}, {3, 4});

        runComparativeTest("Example 2 (Recursive Calls)", 1, {"0:start:0", "0:start:2", "0:end:5", "0:start:6", "0:end:6", "0:end:7"}, {8});

        runComparativeTest("Example 3 (Deep Preemption)", 2, {"0:start:0", "0:start:2", "0:end:5", "1:start:6", "1:end:6", "0:end:7"}, {7, 1});

        runComparativeTest("Edge Case 1 (Sequential, no nesting)", 2, {"0:start:0", "0:end:0", "1:start:1", "1:end:1"}, {1, 1});

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}