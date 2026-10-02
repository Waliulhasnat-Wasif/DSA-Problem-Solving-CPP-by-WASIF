#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::getline;
using std::string;
using std::stringstream;
using std::vector;

class SolutionBaseline {
public:
    string simplifyPath(const string& path) const {
        vector<string> st;
        stringstream ss(path);
        string token;

        while (getline(ss, token, '/')) {
            if (token.empty() || token == ".") {
                continue;
            }
            if (token == "..") {
                if (!st.empty()) {
                    st.pop_back();
                }
            } else {
                st.push_back(token);
            }
        }

        string res = "";
        for (const string& dir : st) {
            res += '/';
            res += dir;
        }

        return res.empty() ? "/" : res;
    }
};

class SolutionOptimized {
public:
    string simplifyPath(const string& path) const {
        vector<std::string_view> st;

        size_t n = path.length();
        std::string_view path_view(path);

        size_t i = 0;
        while (i < n) {
            while (i < n && path[i] == '/') {
                ++i;
            }

            if (i >= n) {
                break;
            }

            size_t start = i;
            while (i < n && path[i] != '/') {
                ++i;
            }

            std::string_view comp = path_view.substr(start, i - start);

            if (comp == "..") {
                if (!st.empty()) {
                    st.pop_back();
                }
            } else if (comp != ".") {
                st.emplace_back(comp);
            }
        }

        if (st.empty()) {
            return "/";
        }

        string res;
        res.reserve(n);

        for (const std::string_view& dir : st) {
            res += '/';
            res += dir;
        }

        return res;
    }
};

void runComparativeTest(const string& test_name, const string& path, const string& expected) {
    cout << "Test Case: " << test_name << "\n";
    cout << "Input Path: \"" << path << "\"\n";

    SolutionOptimized sol_opt;
    SolutionBaseline sol_base;

    string res_opt = sol_opt.simplifyPath(path);
    string res_base = sol_base.simplifyPath(path);

    cout << "Expected:   \"" << expected << "\"\n";
    cout << "Optimized:  \"" << res_opt << "\"\n";
    cout << "Baseline:   \"" << res_base << "\"\n";

    cout << "Result: " << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LeetCode 71: Simplify Path ---\n\n";

    try {
        runComparativeTest("Example 1 (Trailing Slash)", "/home/", "/home");

        runComparativeTest("Example 2 (Multiple Slashes)", "/home//foo/", "/home/foo");

        runComparativeTest("Example 3 (Parent Directory)", "/home/user/Documents/../Pictures", "/home/user/Pictures");

        runComparativeTest("Example 4 (Root Boundary)", "/../", "/");

        runComparativeTest("Example 5 (Valid multi-dots)", "/.../a/../b/c/../d/./", "/.../b/d");

        runComparativeTest("Edge Case (Deep resolution to root)", "/a/./b/../../c/", "/c");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}