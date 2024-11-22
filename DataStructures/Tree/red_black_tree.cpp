#include <iostream>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <chrono> // For measuring execution time

// Enum to represent the color of a node
enum class Color {
    RED,
    BLACK
};

// Structure for a node in the red-black tree
struct Node {
    int data;                 // Value of the node
    Color color;              // Color of the node (RED or BLACK)
    Node *left, *right, *parent; // Pointers to children and parent

    Node(int data, Color color, Node *nil)
        : data(data), color(color), left(nil), right(nil), parent(nil) {}
};

// Structure for the red-black tree
class RBTree {
private:
    Node *root;
    Node *nil; // Sentinel nil node used for leaves

    // Recursive helper to validate black height consistency
    int validate_black_height(Node *node) const {
        if (node == nil) return 1; // Base case: nil nodes have black height 1

        int left_height = validate_black_height(node->left);
        int right_height = validate_black_height(node->right);

        // Check if the black heights of left and right subtrees match
        assert(left_height == right_height);

        // Increment black height if the current node is black
        return left_height + (node->color == Color::BLACK ? 1 : 0);
    }

    // Recursive helper to validate red-black property
    void validate_red_black_property(Node *node) const {
        if (node == nil) return;

        // If a node is red, both its children must be black
        if (node->color == Color::RED) {
            assert(node->left->color == Color::BLACK);
            assert(node->right->color == Color::BLACK);
        }

        // Recursively check children
        validate_red_black_property(node->left);
        validate_red_black_property(node->right);
    }

    // In-order traversal for debugging or validation
    void inorder_traversal(Node *node) const {
        if (node == nil) return;
        inorder_traversal(node->left);
        std::cout << node->data << " ";
        inorder_traversal(node->right);
    }

public:
    RBTree() {
        nil = new Node(0, Color::BLACK, nullptr); // Sentinel node is always black
        root = nil;
    }

    ~RBTree() {
        destroy_tree(root);
        delete nil;
    }

    // Insert a value into the tree (placeholder for implementation)
    void insert(int data) {
        // Algorithm to be implemented
    }

    // Delete a value from the tree (placeholder for implementation)
    void remove(int data) {
        // Algorithm to be implemented
    }

    // Search for a value in the tree (returns true if found)
    bool search(int data) const {
        Node *current = root;
        while (current != nil) {
            if (data == current->data) return true;
            current = (data < current->data) ? current->left : current->right;
        }
        return false;
    }

    // Destroy the tree (recursive helper)
    void destroy_tree(Node *node) {
        if (node == nil) return;
        destroy_tree(node->left);
        destroy_tree(node->right);
        delete node;
    }

    // Validate all red-black tree properties
    void validate_rb_properties() const {
        // Property 1: Root is always black
        assert(root == nil || root->color == Color::BLACK);

        // Property 2: Every leaf (nil) is black (implicitly true in this implementation)

        // Property 3: Red nodes cannot have red children
        validate_red_black_property(root);

        // Property 4: Every path from a node to its descendant nils must have the same number of black nodes
        validate_black_height(root);
    }

    // In-order traversal (for debugging)
    void inorder_traversal() const {
        inorder_traversal(root);
        std::cout << std::endl;
    }
};

// Test framework for the red-black tree
void test_rb_tree() {
    RBTree tree;

    const int num_runs = 100;
    const int large_test_size = 10000;

    // Small test
    std::cout << "Small Random Test:\n";
    std::vector<int> small_test = {10, 20, 15, 5, 25, 30};
    for (int num : small_test) {
        tree.insert(num);
    }
    tree.validate_rb_properties();
    for (int num : small_test) {
        assert(tree.search(num));
    }
    tree.inorder_traversal();

    // Large random test
    std::cout << "\nLarge Random Test:\n";
    std::vector<int> large_test(large_test_size);
    for (int &num : large_test) {
        num = std::rand();
    }

    double total_time = 0.0;

    for (int run = 0; run < num_runs; run++) {
        RBTree test_tree;

        // Measure insertion time
        auto start_time = std::chrono::high_resolution_clock::now();
        for (int num : large_test) {
            test_tree.insert(num);
        }
        auto end_time = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> elapsed_time = end_time - start_time;

        total_time += elapsed_time.count();

        // Validate tree properties after all insertions
        test_tree.validate_rb_properties();

        // Verify that all elements are in the tree
        for (int num : large_test) {
            assert(test_tree.search(num));
        }
    }

    double average_time = total_time / num_runs;
    std::cout << "Average time to insert large array over " << num_runs << " runs: " 
              << average_time << " seconds\n";
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(nullptr))); // Seed RNG
    test_rb_tree();
    std::cout << "All tests passed.\n";
    return 0;
}
