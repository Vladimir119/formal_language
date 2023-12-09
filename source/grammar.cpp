#include "grammar.h"

Rule::Rule() {}
Rule::Rule(char left, const std::string& right): left(left), right(right) {}

bool Rule::operator<(const Rule& other) const {
    return (left < other.left) || (right < other.right);
}

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
    start_noterm(start_noterm) {
        ExpendGrammar();
        rules.push_back(StartRule());
        for (auto transition : transitions) {
            for (auto rule : transition.second) {
                rules.push_back(rule);
            }
        }
    }

std::vector<char> Grammar::AllLatter() const {
    std::vector<char> result;
    for (auto latter : alpabet) result.push_back(latter);
    for (auto noterm : noterminals) result.push_back(noterm);
    return result;  
}

std::map<Rule, int> Grammar::IndexesOfRule() const {
    std::map<Rule, int> result;
    for (int i = 0; i < rules.size(); ++i) {
        result[rules[i]] = i;
    }
    return result;
}

void Grammar::ExpendGrammar() {
    size_noterm += 1;
    size_grammar += 1;
    transitions[kStartSymbol].push_back(Rule(kStartSymbol, std::string(1, start_noterm)));
}