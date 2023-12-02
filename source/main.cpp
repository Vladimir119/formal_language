#include "earley.h"

int main() {
    Earley earley;
    ConsoleReader reader;
    earley.ReadGrammar(reader);

    int count_test = stoi(reader.getLine());

    for (int i = 0; i < count_test; ++i) {
        std::string word = reader.getLine();
        earley.Check(word);
    }
    return 0;
}