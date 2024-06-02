#include <iostream>
#include <filesystem>

namespace fs = std::filesystem;

int main() {
    // Replace "filename.txt" with the path to your file
    fs::path filePath = "/Users/shmimi/Desktop/webserv/assets/html/site-1/images/youtube.png";

    try {
        // Get the size of the file
        std::uintmax_t fileSize = fs::file_size(filePath);

        // Print the file size in bytes
        std::cout << "File size: " << fileSize << " bytes" << std::endl;
    } catch (const fs::filesystem_error& ex) {
        std::cerr << "Error accessing file: " << ex.what() << std::endl;
    }

    return 0;
}
