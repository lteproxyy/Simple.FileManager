#include <iostream>
#include <fstream>
#include <filesystem>
#include <vector>
#include <string>

// 6c746570726f7879

namespace fs = std::filesystem;

class FileManager {
public:
    static std::vector<std::string> list(const std::string& path) {
        std::vector<std::string> entries;
        try {
            for (const auto& entry : fs::directory_iterator(path)) {
                entries.push_back(entry.path().string());
            }
        } catch (const std::exception& e) {
            std::cerr << "Error listing directory: " << e.what() << std::endl;
        }
        return entries;
    }

    static std::string read(const std::string& filepath) {
        std::ifstream file(filepath, std::ios::in | std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file for reading: " << filepath << std::endl;
            return "";
        }
        std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
        return contents;
    }

    static bool write(const std::string& filepath, const std::string& data) {
        std::ofstream file(filepath, std::ios::out | std::ios::binary);
        if (!file) {
            std::cerr << "Error opening file for writing: " << filepath << std::endl;
            return false;
        }
        file << data;
        return true;
    }

    static bool copy(const std::string& src, const std::string& dest) {
        try {
            fs::copy_file(src, dest, fs::copy_options::overwrite_existing);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error copying file: " << e.what() << std::endl;
            return false;
        }
    }

    static bool move(const std::string& src, const std::string& dest) {
        try {
            fs::rename(src, dest);
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error moving file: " << e.what() << std::endl;
            return false;
        }
    }

    static bool remove(const std::string& path) {
        try {
            if (fs::is_directory(path)) {
                fs::remove_all(path);
            } else {
                fs::remove(path);
            }
            return true;
        } catch (const std::exception& e) {
            std::cerr << "Error removing: " << e.what() << std::endl;
            return false;
        }
    }

    static bool mkdir(const std::string& path) {
        try {
            return fs::create_directory(path);
        } catch (const std::exception& e) {
            std::cerr << "Error creating directory: " << e.what() << std::endl;
            return false;
        }
    }
};

int main() {
    std::string dir = ".";
    std::cout << "Listing files in: " << dir << std::endl;
    auto files = FileManager::list(dir);
    for (const auto& f : files) {
        std::cout << f << std::endl;
    }

    std::string testFile = "test.txt";
    FileManager::write(testFile, "Hello, FileManager!");
    std::cout << "File contents: " << FileManager::read(testFile) << std::endl;

    FileManager::copy(testFile, "copy_test.txt");
    FileManager::move("copy_test.txt", "moved_test.txt");
    FileManager::mkdir("test_dir");
    FileManager::remove(testFile);
    FileManager::remove("moved_test.txt");
    FileManager::remove("test_dir");

    return 0;
}
