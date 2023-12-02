#include "earley.h"

Earley::Situation::Situation(const Rule& rule, int point_position, int count_of_read) :
    rule(rule),
    point_position(point_position),
    count_of_read(count_of_read) {}

size_t Earley::Situation::Hash::operator()(const Situation& situation) const {
    size_t constant_1 = 1'000;
    size_t constant_2 = 1'000'000;
    return std::hash<char>{}(situation.rule.left) +
            std::hash<std::string>{}(situation.rule.right) * constant_1 + 
            situation.count_of_read * constant_2;
}

Earley::Layer::Layer(): index_nocomplete(0), index_nopredict(0), is_changed(false) {}

void Earley::Layer::Add(const Situation& situation, const Grammar& grammar) {
    if (situations.contains(situation)) {
        return;
    }
    situations.insert(situation);
    ordered_situation.push_back(situation);

    is_changed = true;

    char noterm = situation.rule.right[situation.point_position];
    if (!grammar.noterminals.contains(noterm)) {
        return;
    }

    situation_with_dot_before_noterm[noterm].push_back(situation);
}


void Earley::Layer::ResetIsChanged() { is_changed = false; }

bool Earley::Layer::Contain(const Situation& situation) { return situations.contains(situation); }

void Earley::ExpentGrammar() {
    grammar_.size_noterm += 1;
    grammar_.size_grammar += 1;
    grammar_.transitions[kStartSymbol].push_back(Rule(kStartSymbol, std::string(1, grammar_.start_noterm)));
}


void Earley::Scan(int layer_number, const std::string& word) {
    for (auto situation : layers[layer_number].situations) {
        int dot_pos = situation.point_position;
        if (dot_pos == situation.rule.right.size() || layer_number == layers.size() - 1) {
            continue;
        }
        
        char cur_latter = situation.rule.right[dot_pos];
        if (grammar_.alpabet.contains(cur_latter)) {
            if (cur_latter != word[layer_number]) {
                continue;
            }
            
            Situation new_situation = situation;
            new_situation.point_position += 1;
            layers[layer_number + 1].Add(new_situation, grammar_);
        }
    }
}

void Earley::Predict(int layer_number, const std::string& word) {
    Layer& layer = layers[layer_number];
    std::vector<Situation>& situations = layer.ordered_situation;

    for (size_t i = layer.index_nopredict; i < situations.size(); ++i) {
        int dot_pos = situations[i].point_position;
        if (dot_pos == situations[i].rule.right.size()) {
            continue;
        }
        int curr_noterm = situations[i].rule.right[dot_pos];
        if(grammar_.noterminals.contains(curr_noterm)) {
            for (auto rule : grammar_.transitions[curr_noterm]) {
                Situation new_situation(Rule(curr_noterm, rule.right), 0, layer_number);
                layer.Add(new_situation, grammar_);
            }
        }
    }
    layer.index_nopredict = situations.size();
}

void Earley::Complete(int layer_number, const std::string& word) {
    Layer& layer = layers[layer_number];
    std::vector<Situation>& situations = layer.ordered_situation;

    for (size_t i = layer.index_nocomplete; i < situations.size(); ++i) {
        int dot_pos = situations[i].point_position;
        if (dot_pos != situations[i].rule.right.size()) {
            continue;
        }

        char noterm = situations[i].rule.left;
        int l = situations[i].count_of_read;
            for (auto old_situation : layers[l].situation_with_dot_before_noterm[noterm]) {
            Situation new_situation = old_situation;
            new_situation.point_position += 1;
            layer.Add(new_situation, grammar_);
        }
    }
    layer.index_nocomplete = situations.size();
}

bool Earley::Changes(int layer_number) {
    return layers[layer_number].is_changed;
}

Earley::Earley(const Grammar& grammar): grammar_(grammar) { ExpentGrammar(); };

void Earley::ReadGrammar(Reader& reader) {
    grammar_ = Parser::Parse(reader);
    ExpentGrammar();
}

void Earley::Check(const std::string& word) {
    int size = static_cast<int>(word.size());
    layers = std::vector<Layer>(size + 1);

    Situation start_situation(Rule(kStartSymbol, std::string(1, grammar_.start_noterm)), 0, 0);
    layers[0].Add(start_situation, grammar_);

    for (int r = 0; r <= size; ++r) {
        while(Changes(r)) {
            layers[r].ResetIsChanged();
            Predict(r, word);
            Complete(r, word);
        }
        Scan(r, word);
    }

    Situation finish_situation(Rule(kStartSymbol, std::string(1, grammar_.start_noterm)), 1, 0);
    if (layers[size].Contain(finish_situation)) {
        std::cout << "OK\n";
    } else {
        std::cout << "NO\n";
    }
}