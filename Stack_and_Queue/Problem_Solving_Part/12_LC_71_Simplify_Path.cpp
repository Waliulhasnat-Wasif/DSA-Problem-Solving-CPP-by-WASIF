#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

// Specific using declarations to maintain namespace hygiene
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::getline;
using std::string;
using std::stringstream;
using std::vector;

// ==========================================
// Approach 1: Baseline Architecture (StringStream Split)
// Very readable, but incurs standard library string splitting overhead.
// Time Complexity: O(N) | Space Complexity: O(N)
// ==========================================
class SolutionBaseline {
public:
    string simplifyPath(const string& path) const {
        vector<string> st;
        stringstream ss(path);
        string token;

        // Split the path based on '/'
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

        // Reconstruct the canonical path
        string res = "";
        for (const string& dir : st) {
            res += '/';
            res += dir;
        }

        return res.empty() ? "/" : res;
    }
};

// ==========================================
// Approach 2: Optimized Architecture (Manual Parsing & Vector Stack)
// FAANG-level optimization avoiding stringstream allocations.
// Features std::move for zero-copy ownership transfer.
// Time Complexity: Strict O(N) | Space Complexity: Strict O(N)
// ==========================================
class SolutionOptimized {
public:
    string simplifyPath(const string& path) const {
        vector<string> st;

        // Type safe size retrieval to prevent unsigned underflow traps
        int n = static_cast<int>(path.length());

        // Manual parsing for maximum performance
        for (int i = 0; i < n; ++i) {
            if (path[i] == '/') {
                continue;
            }

            string temp;
            while (i < n && path[i] != '/') {
                temp += path[i];
                ++i;
            }

            if (temp == ".") {
                continue;  // Ignore current directory symbol
            } else if (temp == "..") {
                if (!st.empty()) {
                    st.pop_back();  // Go up one directory
                }
            } else {
                // HIGHLIGHT: std::move steals memory, preventing costly deep copies
                st.push_back(std::move(temp));
            }
        }

        if (st.empty()) {
            return "/";
        }

        string res;
        // HIGHLIGHT: Pre-allocate memory to prevent dynamic reallocation
        res.reserve(n);

        for (const string& dir : st) {
            res += '/';
            res += dir;
        }

        return res;
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
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

// ==========================================
// Main Function (Clean & Safe Entry Point)
// ==========================================
int main() {
    cout << "--- Testing LeetCode 71: Simplify Path ---\n\n";

    try {
        // 1. Example 1: Trailing slash removal
        runComparativeTest("Example 1 (Trailing Slash)", "/home/", "/home");

        // 2. Example 2: Multiple consecutive slashes
        runComparativeTest("Example 2 (Multiple Slashes)", "/home//foo/", "/home/foo");

        // 3. Example 3: Parent directory processing
        runComparativeTest("Example 3 (Parent Directory)", "/home/user/Documents/../Pictures", "/home/user/Pictures");

        // 4. Example 4: Root level boundary check
        runComparativeTest("Example 4 (Root Boundary)", "/../", "/");

        // 5. Example 5: Multi-dot directories (valid names)
        runComparativeTest("Example 5 (Valid multi-dots)", "/.../a/../b/c/../d/./", "/.../b/d");

        // 6. Edge Case: Deep navigation resolving to root
        runComparativeTest("Edge Case (Deep resolution to root)", "/a/./b/../../c/", "/c");

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}