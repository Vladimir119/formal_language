#include "grammar.h"

Rule::Rule() {}
Rule::Rule(char left, const std::string& right): left(left), right(right) {}

Grammar::Grammar() :
    size_noterm(0),
    size_alphabet(0),
    size_grammar(0),
    start_noterm(kStartSymbol) {}

Grammar::Grammar(int size_noterm,
                int size_alphabet,
                int size_grammar,
                const std::unordered_set<char> noterminals,
                const std::unordered_set<char> alpabet,
                const std::map<char, std::vector<Rule>>& transitions,
                char start_noterm) :
    size_noterm(size_noterm),
    size_alphabet(size_alphabet),
    size_grammar(size_grammar),
    noterminals(noterminals),
    alpabet(alpabet),
    transitions(transitions),
    start_noterm(start_noterm) {}
