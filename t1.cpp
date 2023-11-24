#include <iostream>
#include <fstream>

int main() {
    // Open an output file stream
    std::ofstream outputFile("output.txt");
    
    // Output to console
    std::cout << "This is output to console" << std::endl;

    // Output to file
    outputFile << "This is output to file" << std::endl;

    // Close the file stream
    outputFile.close();

    return 0;
}
