#include <iostream>
#include <vector>
#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <chrono> // For measuring execution time
#include <cstdint>

// Time complexity: O(log N)
void heapify(int *array, int idx, int size) {
    int largest_val = array[idx];
    int largest_idx = idx;

    // left child
    if (2 * idx < size) {
        if (largest_val < array[2 * idx]) {
            largest_val = array[2 * idx];
            largest_idx = 2 * idx;
        }
    }
    // right child
    if (2 * idx + 1 < size) {
        if (largest_val < array[2 * idx + 1]) {
            largest_val = array[2 * idx + 1];
            largest_idx = 2 * idx + 1;
        }
    }

    if (idx != largest_idx) {
        // swap idx and largest_idx
        int tmp = array[idx];
        array[idx] = array[largest_idx];
        array[largest_idx] = tmp;
        // recursively call heapify()
        heapify(array, largest_idx, size);
    }
}

// Time complexity: O(N log N)
void build_max_heap(int *array, int size) {
    int i = size / 2;
    while (i >= 0) {
        heapify(array, i--, size);
    }
}

// Time complexity: O(1)
int remove_max(int *array, int size)
{
    if (size <= 0)
        return 0;

    int tmp = array[0];
    array[0] = array[size - 1];
    array[size - 1] = tmp;
    return array[size - 1];
}

// Time complexity: O(N log N)
void heap_sort(int *array, int size) {
    // O(N log N)
    build_max_heap(array, size);
    // O(N log N)
    for (int i = 0; i < size; i++) {
        // O(1)
        array[size - i - 1] = remove_max(array, size - i);
        // O(log N)
        heapify(array, 0, size - i - 1);
    }
}

// Helper function to print an array
void print_array(const int *array, int size) {
    for (int i = 0; i < size; i++) {
        std::cout << array[i] << " ";
    }
    std::cout << std::endl;
}

// Function to verify the array is sorted and has no added or removed elements
void verify_sort_and_elements(const std::vector<int>& original, const int *sorted_array, int size) {
    // Check sorted order
    for (int i = 1; i < size; i++) {
        assert(sorted_array[i - 1] <= sorted_array[i]);
    }

    // Check that no elements are added or removed
    std::vector<int> sorted_copy(sorted_array, sorted_array + size);
    std::sort(sorted_copy.begin(), sorted_copy.end());

    std::vector<int> original_copy = original;
    std::sort(original_copy.begin(), original_copy.end());

    assert(sorted_copy == original_copy);
}

void test_heap_sort() {
    const int small_test_size = 10;
    const int large_test_size = 10000;
    const int threshold_to_print = 20;
    const int num_runs = 100;

    // Seed random number generator
    std::srand(static_cast<unsigned int>(std::time(nullptr)));

    // Small random test
    std::cout << "Small Random Test:\n";
    std::vector<int> small_test(small_test_size);
    for (int& num : small_test) {
        num = std::rand() % 100; // Random numbers between 0 and 99
    }
    if (small_test_size <= threshold_to_print) {
        std::cout << "Before Sorting:\n";
        print_array(small_test.data(), small_test_size);
    }

    std::vector<int> small_test_copy = small_test;
    heap_sort(small_test.data(), small_test_size);

    if (small_test_size <= threshold_to_print) {
        std::cout << "After Sorting:\n";
        print_array(small_test.data(), small_test_size);
    }

    verify_sort_and_elements(small_test_copy, small_test.data(), small_test_size);

    // Large random test
    std::cout << "\nLarge Random Test:\n";
    std::vector<int> large_test(large_test_size);

    double total_time = 0.0;

    for (int run = 0; run < num_runs; run++) {
        // Generate a new random array for each run
        for (int& num : large_test) {
            num = std::rand();
        }

        std::vector<int> large_test_copy = large_test;

        // Measure sorting time
        auto start_time = std::chrono::high_resolution_clock::now();
        heap_sort(large_test.data(), large_test_size);
        auto end_time = std::chrono::high_resolution_clock::now();

        std::chrono::duration<double> elapsed_time = end_time - start_time;
        total_time += elapsed_time.count();

        // Verify correctness for each run
        verify_sort_and_elements(large_test_copy, large_test.data(), large_test_size);
    }

    double average_time = total_time / num_runs;

    std::cout << "Average time to sort large array over " << num_runs << " runs: " 
              << average_time << " seconds\n";
}

int main() {
    test_heap_sort();
    std::cout << "All tests passed.\n";
    return 0;
}

