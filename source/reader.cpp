#include "reader.h"

std::string ConsoleReader::getLine() {
 std::string line;
 std::cin >> line;
 return line;
}

FileReader::FileReader(std::string path) {
    f_.open(path);
}
std::string FileReader::getLine() {
    std::string line;
    f_ >> line;
    return line;
}