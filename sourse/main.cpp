#include "earley.h"

int main() {
    Earley earley;
    ConsoleReader reader;
    earley.ReadGrammar(reader);
    while(true) {
        std::string word = reader.getLine();
        earley.Check(word);
    }
    return 0;
}