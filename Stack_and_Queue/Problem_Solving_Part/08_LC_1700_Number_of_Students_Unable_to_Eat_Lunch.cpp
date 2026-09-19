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

// ==========================================
// Approach 1: Baseline Architecture (Queue Simulation)
// Direct implementation of the problem statement.
// Time Complexity: O(N^2) worst case | Space: O(N)
// ==========================================
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

        // Type Safety Fix: Use size_t for indices and size counters
        size_t sandwich_idx = 0;
        size_t consecutive_rejections = 0;

        // Loop runs until queue is empty or we hit a deadlock
        while (!q.empty() && consecutive_rejections < q.size()) {
            if (q.front() == sandwiches[sandwich_idx]) {
                // Match found: Student takes the sandwich
                q.pop();
                sandwich_idx++;
                consecutive_rejections = 0;  // Reset rejections
            } else {
                // No match: Student goes to the back of the line
                q.push(q.front());
                q.pop();
                consecutive_rejections++;
            }
        }

        // Safely cast size_t back to int for the LeetCode return signature
        return static_cast<int>(q.size());
    }
};

// ==========================================
// Approach 2: Optimized Architecture (Mathematical State Tracking)
// Order doesn't matter. Only the counts matter until a deadlock is reached.
// Time Complexity: Strict O(N) | Space: Strict O(1) auxiliary
// ==========================================
class SolutionOptimized {
public:
    int countStudents(const vector<int>& students, const vector<int>& sandwiches) const {
        if (students.size() != sandwiches.size()) {
            throw invalid_argument("Error: Students and Sandwiches arrays must be equal length.");
        }

        // State Trackers: count[0] for circular (0), count[1] for square (1)
        int preference_counts[2] = {0, 0};

        for (int student : students) {
            preference_counts[student]++;
        }

        // Process the stack of sandwiches
        for (int sandwich : sandwiches) {
            if (preference_counts[sandwich] > 0) {
                // A student will eventually rotate and take it
                preference_counts[sandwich]--;
            } else {
                // Deadlock: No remaining student wants the top sandwich
                break;
            }
        }

        // Remaining students who couldn't eat
        return preference_counts[0] + preference_counts[1];
    }
};

// ==========================================
// Test Execution Engine (Separation of Concerns & Edge Cases)
// ==========================================
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
        // 1. Example 1: Everyone eats
        runComparativeTest("Example 1 (Perfect resolution)", {1, 1, 0, 0}, {0, 1, 0, 1}, 0);

        // 2. Example 2: Deadlock occurs in the middle
        runComparativeTest("Example 2 (Partial deadlock)", {1, 1, 1, 0, 0, 1}, {1, 0, 0, 0, 1, 1}, 3);

        // 3. Edge Case: Immediate Deadlock
        runComparativeTest("Edge Case 1 (Immediate Deadlock)", {0, 0, 0}, {1, 1, 1}, 3);

        // 4. Edge Case: All identical and matched
        runComparativeTest("Edge Case 2 (All matched without rotation)", {1, 1, 1}, {1, 1, 1}, 0);

    } catch (const exception& e) {
        cerr << "[Exception Caught] " << e.what() << endl;
    }

    return 0;
}