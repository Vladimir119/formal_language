#include "parser.h"

std::pair<std::string, std::string> Parser::Split(const std::string& str, const std::string& separator) {
    size_t current_index_in_separator = 0;
    std::string first;
    std::string second;

    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == separator[current_index_in_separator]) {
            current_index_in_separator += 1;
        }
        if (current_index_in_separator == separator.size()) {
            for (int j = 0; j < i - separator.size() + 1; ++j) {
                first += str[j];
            }
            for (int j = 0; j < str.size() - i - 1; ++j) {
                second += str[j + i + 1];
            }

            return {first, second};
        }
    }

    return {" ", " "};
}

bool Parser::IsContextFree(const std::string& noterm) {
    if (noterm.size() != 1) {
        return false;
    }

    char noterm_char = noterm[0];

    return std::isupper(noterm_char);
}

Grammar Parser::Parse(Reader& reader) {
    int size_noterm = stoi(reader.getLine());
    int size_alphabet = stoi(reader.getLine());
    int size_grammar = stoi(reader.getLine());

    std::unordered_set<char> noterminals;
    std::string string_of_noterminals = reader.getLine();
    for (auto symbol : string_of_noterminals) {
        noterminals.insert(symbol);
    }

    if (noterminals.size() != size_noterm) {
        throw("ERROR! NO CORRECT NOTERMINAL SIZE!\n");
    }

    std::unordered_set<char> alphabet;
    std::string string_of_alphabet = reader.getLine();
    for (auto symbol : string_of_alphabet) {
        alphabet.insert(symbol);
    }

    if (alphabet.size() != size_alphabet) {
        throw("ERROR! NO CORRECT ALPHABET SIZE!\n");
    }

    std::map<char, std::vector<Rule>> rules;

    for (int i = 0; i < size_grammar; ++i) {
        std::string current_rule = reader.getLine();
        auto current_pair = Split(current_rule, kSeparator); 

        std::string noterm_str = current_pair.first;
        if (! IsContextFree(noterm_str)) {
            throw("no context free grammar!!!\n");
        }

        char noterm_char = noterm_str[0];

        rules[noterm_char].push_back(Rule(noterm_char, current_pair.second));
    }

    std::string start_noterm = reader.getLine();
    if (start_noterm.size() > 1) {
        throw("ERROR. NO CORRECT START NOTERMINAL!\n");
    }

    return Grammar(size_noterm, size_alphabet, size_grammar, noterminals, alphabet, rules, start_noterm[0]);
}