#include <gtest/gtest.h>

#include <chrono>

// Declare the function here
void generateHistograms();

// TestFamilyName, NameOfTest
TEST(GenerateHistograms, Test1) {
    // Timer
    auto start = std::chrono::high_resolution_clock::now();

    // Input
    std::istringstream input("World");
    std::streambuf *cinbuf = std::cin.rdbuf(input.rdbuf());

    // Output
    std::ostringstream output;
    std::streambuf *coutbuf = std::cout.rdbuf(output.rdbuf());

    // Run function with inputs
    generateHistograms();

    // Insert the input and get the output
    std::cin.rdbuf(cinbuf);
    std::cout.rdbuf(coutbuf);

    // Stop the clock.
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    // Validade the test.
    std::cout << "Test1 took " << duration.count() << " seconds.\n";
    EXPECT_EQ(output.str(), "Hello, World!\n");
}