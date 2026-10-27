// Tree recursion: depth-first traversal of a binary tree.
 // Build: c++ -std=c++17 -Wall -Wextra -Wpedantic binary_tree_dfs.cpp -o binary_tree_dfs
#include <iostream>
#include <memory>

struct Node {
    int value;
    std::unique_ptr<Node> left, right;
    explicit Node(int v) : value(v) {}
};

void preorder(const Node* node) {
    if (!node) return;
    std::cout << node->value << ' ';
    preorder(node->left.get());
    preorder(node->right.get());
}

int main() {
    Node root(8);
    root.left = std::make_unique<Node>(3);
    root.right = std::make_unique<Node>(10);
    root.left->left = std::make_unique<Node>(1);
    root.left->right = std::make_unique<Node>(6);
    std::cout << "preorder DFS: ";
    preorder(&root);
    std::cout << '\n';
}
