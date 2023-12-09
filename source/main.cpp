#include "lr.h"

int main() {

    ConsoleReader reader;
    Parser parser;
    Grammar grammar = parser.Parse(reader);

    LR lr(grammar);

    int count_word; std::cin >> count_word;
    for(int i = 0; i < count_word; ++i) {
        std::string str;
        std::cin >> str;

        if (lr.Parse(str)) {
            std::cout << "OK\n";
        } else std::cout << "NO\n";
    }

    return 0;
}