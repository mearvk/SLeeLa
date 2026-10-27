// Linear recursion over a linked list.
 // Build: c++ -std=c++17 -Wall -Wextra -Wpedantic linked_list_length.cpp -o linked_list_length
#include <cstddef>
#include <iostream>
#include <memory>

struct Node {
    int value;
    std::unique_ptr<Node> next;
    explicit Node(int v) : value(v), next(nullptr) {}
};

std::size_t length(const Node* node) {
    return node == nullptr ? 0U : 1U + length(node->next.get());
}

int main() {
    Node a(10), b(20), c(30);
    a.next = std::make_unique<Node>(b.value);
    a.next->next = std::make_unique<Node>(c.value);
    std::cout << "list length = " << length(&a) << '\n';
}
