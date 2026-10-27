/* cpp_primitive_byte_recursion.cpp
 * C++ typed recursion class for the SLeeLa byte primitive.
 * Patterns: linear recursion, divide-and-conquer, structural tree recursion.
 * Build: c++ -std=c++17 -Wall -Wextra -Wpedantic cpp_primitive_byte_recursion.cpp -o cpp_primitive_byte_recursion
 * Reference prototype; adapt type alias and lifetime model to the VM.
 */
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <memory>
#include <vector>
class SLStanchionByteRecursor {
public:
    using Value = std::uint8_t;
    struct Node {
        Value value;
        std::unique_ptr<Node> left, right;
        explicit Node(Value v) : value(v) {}
    };
    static std::size_t countLinear(const std::vector<Value>& v, std::size_t i, Value target, std::size_t depth, std::size_t limit) {
        if (i >= v.size() || depth >= limit) return 0;
        return (v[i] == target ? 1u : 0u) + countLinear(v, i + 1, target, depth + 1, limit);
    }
    static std::size_t countDivide(const std::vector<Value>& v, std::size_t first, std::size_t last, Value target, std::size_t depth, std::size_t limit) {
        if (first >= last || depth >= limit) return 0;
        if (last - first == 1) return v[first] == target ? 1u : 0u;
        const std::size_t mid = first + (last - first) / 2;
        return countDivide(v, first, mid, target, depth + 1, limit)
             + countDivide(v, mid, last, target, depth + 1, limit);
    }
    static std::size_t countTree(const Node* n, Value target, std::size_t depth, std::size_t limit) {
        if (n == nullptr || depth >= limit) return 0;
        return (n->value == target ? 1u : 0u)
             + countTree(n->left.get(), target, depth + 1, limit)
             + countTree(n->right.get(), target, depth + 1, limit);
    }
};
int main() {
    using R = SLStanchionByteRecursor;
    const std::vector<R::Value> values{ std::uint8_t{7}, std::uint8_t{7}, static_cast<R::Value>(0) };
    constexpr std::size_t limit = 64;
    std::cout << "byte: linear=" << R::countLinear(values, 0, std::uint8_t{7}, 0, limit)
              << " divide=" << R::countDivide(values, 0, values.size(), std::uint8_t{7}, 0, limit) << '\n';
}
