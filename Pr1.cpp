#include <iostream>
#include <vector>
#include <queue>
#include <omp.h>
using namespace std;

class Node {
public:
    int data;
    Node* left;
    Node* right;

    Node(int val) : data(val), left(NULL), right(NULL) {}
};

// Parallel DFS using OpenMP
void parallelDFS(Node* node) {
    if (!node) return;

    cout << "DFS Visited node: " << node->data << endl;

    #pragma omp task
    parallelDFS(node->left);

    #pragma omp task
    parallelDFS(node->right);
}

// Parallel BFS (Level-by-Level) using OpenMP
void parallelBFS(Node* root) {
    if (!root) return;

    queue<Node*> currentLevel;
    currentLevel.push(root);

    while (!currentLevel.empty()) {
        int size = currentLevel.size();

        #pragma omp parallel for
        for (int i = 0; i < size; ++i) {
            Node* node;

            // Use critical section to safely pop from queue
            #pragma omp critical
            {
                if (!currentLevel.empty()) {
                    node = currentLevel.front();
                    currentLevel.pop();
                    cout << "BFS Visited node: " << node->data << endl;
                }
            }

            // Critical again to safely push children
            if (node) {
                #pragma omp critical
                {
                    if (node->left) currentLevel.push(node->left);
                    if (node->right) currentLevel.push(node->right);
                }
            }
        }
    }
}

int main() {
    Node* root = new Node(1);
    root->left = new Node(2);
    root->right = new Node(3);
    root->left->left = new Node(4);
    root->left->right = new Node(5);
    root->right->left = new Node(6);
    root->right->right = new Node(7);

    // Run both in parallel sections
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            cout << "\n--- Parallel BFS ---\n";
            parallelBFS(root);
        }

        #pragma omp section
        {
            cout << "\n--- Parallel DFS ---\n";
            #pragma omp parallel
            {
                #pragma omp single
                parallelDFS(root);
            }
        }
    }

    return 0;
}

