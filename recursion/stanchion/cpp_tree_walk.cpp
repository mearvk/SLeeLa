// Structural recursion: three standard binary-tree traversal orders.
// Build: c++ -std=c++17 -Wall -Wextra -Wpedantic cpp_tree_walk.cpp -o cpp_tree_walk
#include <iostream>
#include <memory>

struct Node {
    int value;
    std::unique_ptr<Node> left;
    std::unique_ptr<Node> right;
    explicit Node(int v) : value(v) {}
};

void preorder(const Node* n) {
    if (!n) return;
    std::cout << n->value << ' ';
    preorder(n->left.get());
    preorder(n->right.get());
}
void inorder(const Node* n) {
    if (!n) return;
    inorder(n->left.get());
    std::cout << n->value << ' ';
    inorder(n->right.get());
}
void postorder(const Node* n) {
    if (!n) return;
    postorder(n->left.get());
    postorder(n->right.get());
    std::cout << n->value << ' ';
}

int main() {
    Node root(8);
    root.left = std::make_unique<Node>(3);
    root.right = std::make_unique<Node>(10);
    root.left->left = std::make_unique<Node>(1);
    root.left->right = std::make_unique<Node>(6);
    std::cout << "preorder: "; preorder(&root);
    std::cout << "\ninorder: "; inorder(&root);
    std::cout << "\npostorder: "; postorder(&root);
    std::cout << '\n';
}
