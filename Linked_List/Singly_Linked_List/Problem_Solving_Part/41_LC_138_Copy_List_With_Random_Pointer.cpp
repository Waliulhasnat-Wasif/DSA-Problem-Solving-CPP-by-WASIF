#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using std::cout;
using std::pair;
using std::string;
using std::unordered_map;
using std::vector;

class Node {
public:
    int val;
    Node* next;
    Node* random;

    Node(int _val) {
        val = _val;
        next = nullptr;
        random = nullptr;
    }
};

class Solution {
public:
    Node* copyRandomListHashMap(Node* head) {
        if (head == nullptr) {
            return head;
        }

        unordered_map<Node*, Node*> nodeMap;
        Node* curr = head;

        while (curr != nullptr) {
            nodeMap[curr] = new Node(curr->val);
            curr = curr->next;
        }

        curr = head;

        while (curr != nullptr) {
            nodeMap[curr]->next = nodeMap[curr->next];
            nodeMap[curr]->random = nodeMap[curr->random];
            curr = curr->next;
        }

        return nodeMap[head];
    }

    Node* copyRandomList(Node* head) {
        if (head == nullptr) {
            return head;
        }

        Node* curr = head;
        while (curr != nullptr) {
            Node* clonedNode = new Node(curr->val);
            clonedNode->next = curr->next;
            curr->next = clonedNode;
            curr = clonedNode->next;
        }

        curr = head;
        while (curr != nullptr) {
            if (curr->random != nullptr) {
                curr->next->random = curr->random->next;
            }
            curr = curr->next->next;
        }

        curr = head;
        Node dummy(-1);
        Node* cloneTail = &dummy;

        while (curr != nullptr) {
            Node* clonedNode = curr->next;
            cloneTail->next = clonedNode;
            cloneTail = cloneTail->next;

            curr->next = clonedNode->next;
            curr = curr->next;
        }

        cloneTail->next = nullptr;

        return dummy.next;
    }
};

Node* buildListWithRandomPointers(const vector<pair<int, int>>& nodeData) {
    if (nodeData.empty()) return nullptr;

    vector<Node*> nodes;
    for (const auto& data : nodeData) {
        nodes.push_back(new Node(data.first));
    }

    for (size_t i = 0; i < nodes.size(); i++) {
        if (i < nodes.size() - 1) {
            nodes[i]->next = nodes[i + 1];
        }
        int randomIndex = nodeData[i].second;
        if (randomIndex != -1) {
            nodes[i]->random = nodes[randomIndex];
        }
    }

    return nodes[0];
}

void printList(Node* head) {
    if (head == nullptr) {
        cout << "[]\n";
        return;
    }

    unordered_map<Node*, int> nodeToIndex;
    Node* curr = head;
    int index = 0;
    while (curr != nullptr) {
        nodeToIndex[curr] = index++;
        curr = curr->next;
    }

    cout << "[";
    curr = head;
    while (curr != nullptr) {
        cout << "[" << curr->val << ",";
        if (curr->random == nullptr) {
            cout << "null";
        } else {
            cout << nodeToIndex[curr->random];
        }
        cout << "]";

        if (curr->next != nullptr) cout << ",";
        curr = curr->next;
    }
    cout << "]\n";
}

void deleteList(Node* head) {
    Node* curr = head;
    while (curr != nullptr) {
        Node* temp = curr;
        curr = curr->next;
        delete temp;
    }
}

bool verifyDeepCopy(Node* original, Node* copy) {
    Node* currOrig = original;
    Node* currCopy = copy;

    while (currOrig != nullptr && currCopy != nullptr) {
        if (currOrig->val != currCopy->val) return false;

        if (currOrig == currCopy) return false;

        if (currOrig->random != nullptr) {
            if (currCopy->random == nullptr) return false;
            if (currOrig->random->val != currCopy->random->val) return false;

            if (currOrig->random == currCopy->random) return false;
        } else {
            if (currCopy->random != nullptr) return false;
        }

        currOrig = currOrig->next;
        currCopy = currCopy->next;
    }

    return currOrig == nullptr && currCopy == nullptr;
}

void runTestCase(const vector<pair<int, int>>& input, const string& testName) {
    Solution sol;
    cout << "Test Case: " << testName << "\n";

    Node* originalHead = buildListWithRandomPointers(input);
    cout << "Original List : ";
    printList(originalHead);

    Node* copiedHead = sol.copyRandomList(originalHead);
    cout << "Copied List   : ";
    printList(copiedHead);

    if (verifyDeepCopy(originalHead, copiedHead)) {
        cout << "Status        : [PASS - Valid Deep Copy]\n";
    } else {
        cout << "Status        : [FAIL - Memory Cross-linked or Invalid Values]\n";
    }

    cout << string(60, '-') << "\n";

    deleteList(originalHead);
    deleteList(copiedHead);
}

int main() {
    cout << "--- Testing LeetCode 138: Copy List with Random Pointer ---\n\n";

    runTestCase({{7, -1}, {13, 0}, {11, 4}, {10, 2}, {1, 0}}, "Example 1 (Complex Forward/Backward dependencies)");
    runTestCase({{1, 1}, {2, 1}}, "Example 2 (Self and neighboring pointers)");
    runTestCase({{3, -1}, {3, 0}, {3, -1}}, "Example 3 (Multiple nulls and duplicate values)");
    runTestCase({}, "Edge Case 1 (Empty list)");

    return 0;
}