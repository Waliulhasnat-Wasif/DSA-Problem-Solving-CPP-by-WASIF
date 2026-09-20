#include <iostream>
#include <queue>
#include <stdexcept>
#include <string>
#include <vector>

using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::invalid_argument;
using std::queue;
using std::string;
using std::vector;

class SolutionSimulation {
public:
    int countStudents(const vector<int>& students, const vector<int>& sandwiches) const {
        if (students.size() != sandwiches.size()) {
            throw invalid_argument("Error: Students and Sandwiches arrays must be equal length.");
        }

        queue<int> q;
        for (int student : students) {
            q.push(student);
        }

        size_t sandwich_idx = 0;
        size_t consecutive_rejections = 0;

        while (!q.empty() && consecutive_rejections < q.size()) {
            if (q.front() == sandwiches[sandwich_idx]) {
                q.pop();
                sandwich_idx++;
                consecutive_rejections = 0;
            } else {
                q.push(q.front());
                q.pop();
                consecutive_rejections++;
            }
        }

        return static_cast<int>(q.size());
    }
};

class SolutionOptimized {
public:
    int countStudents(const vector<int>& students, const vector<int>& sandwiches) const {
        if (students.size() != sandwiches.size()) {
            throw invalid_argument("Error: Students and Sandwiches arrays must be equal length.");
        }

        int preference_counts[2] = {0, 0};

        for (int student : students) {
            preference_counts[student]++;
        }

        for (int sandwich : sandwiches) {
            if (preference_counts[sandwich] > 0) {
                preference_counts[sandwich]--;
            } else {
                break;
            }
        }

        return preference_counts[0] + preference_counts[1];
    }
};

void runComparativeTest(const string& test_name, const vector<int>& students, const vector<int>& sandwiches, int expected) {
    cout << "Test Case: " << test_name << "\n";

    SolutionOptimized sol_opt;
    SolutionSimulation sol_base;

    int res_opt = sol_opt.countStudents(students, sandwiches);
    int res_base = sol_base.countStudents(students, sandwiches);

    cout << "Expected: " << expected << " | Optimized: " << res_opt << " | Baseline: " << res_base << " "
         << (res_opt == expected && res_base == expected ? "[PASS]" : "[FAIL]") << "\n";

    cout << string(80, '-') << "\n";
}

int main() {
    cout << "--- Testing LC 1700: Number of Students Unable to Eat ---\n\n";

    try {
        runComparativeTest("Example 1 (Perfect resolution)", {1, 1, 0, 0}, {0, 1, 0, 1}, 0);

        runComparativeTest("Example 2 (Partial deadlock)", {1, 1, 1, 0, 0, 1}, {1, 0, 0, 0, 1, 1}, 3);

        runComparativeTest("Edge Case 1 (Immediate Deadlock)", {0, 0, 0}, {1, 1, 1}, 3);

        runComparativeTest("Edge Case 2 (All matched without rotation)", {1, 1, 1}, {1, 1, 1}, 0);

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}