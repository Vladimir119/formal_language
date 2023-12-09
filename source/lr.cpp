#include "lr.h"

LR::Situation::Situation(const Rule& rule, int point_position, char latter_after): 
        rule(rule), 
        point_position(point_position),
        latter_after(latter_after) {}

bool LR::Situation::is_point_before_noterm(const Grammar& grammar) const {
    if (point_position == rule.right.size()) return false;
    return grammar.is_noterm(rule.right[point_position]);
}

char LR::Situation::latter_after_point() const {
    if (point_position == rule.right.size()) return ' ';
    return rule.right[point_position];
}

size_t LR::Situation::Hash::operator()(const Situation& situation) const {
    size_t constant_1 = 1'000;
    size_t constant_2 = 1'000'000;
    return std::hash<char>{}(situation.rule.left) +
    std::hash<std::string>{}(situation.rule.right) * constant_1 + 
    static_cast<size_t>(situation.latter_after) * constant_2;
};

LR::Situation LR::Situation::StartSituation(const Grammar& grammar) { return Situation(grammar.StartRule(), 0, kEndString); }


LR::TermFirst::TermFirst() : is_eps_generated(false) {}
LR::TermFirst::TermFirst(char noterm) : noterm(noterm), is_eps_generated(false) {}

bool LR::TermFirst::push(char latter) {
    if (first.contains(latter)) return false;
    first.insert(latter);
    return true;
}

bool LR::TermFirst::merge(const TermFirst& other) {
    bool is_change = false;
    for (auto latter : other.first) {
        is_change = (is_change || push(latter));
    }

    return is_change;
}

LR::VertexDfa::VertexDfa(int index, const std::vector<Situation>& situations) : index(index), situations(situations) {}

size_t LR::HashVectorSit::operator()(const std::vector<Situation>& situations) const {
    size_t result = 0;
    for (auto situation : situations) {
        result += Situation::Hash()(situation);
    }
    return result;
};

LR::Action::Action(ActionEnum type, int number) : type(type), number(number) {}


std::vector<LR::Situation> LR::Clouser(const std::vector<Situation>& items) {
    bool is_changed = false;
    std::vector<Situation> result = items;
    std::unordered_set<Situation, Situation::Hash> itmes_set;
    for (auto item : items) itmes_set.insert(item);

    int index_of_new = 0;

    for (int i = index_of_new; i < result.size(); ++i) {
        Situation item = result[i];
        if (!item.is_point_before_noterm(grammar_)) continue;
        char cur_noterm = item.latter_after_point();

        for (const auto& rule : grammar_.transitions.at(cur_noterm)) {
            std::string cur_contined; cur_contined.clear();
            for (size_t j = item.point_position + 1; j < item.rule.right.size(); ++j) {
                cur_contined += item.rule.right[j];
            } cur_contined += item.latter_after;
            
            for (auto latter : FirstForStr(cur_contined)) {
                Situation new_situation(rule, 0, latter);
                if (!itmes_set.contains(new_situation)) {
                    result.push_back(new_situation);
                    itmes_set.insert(new_situation);
                }
            }
        }
        index_of_new += 1;
    }

    return result;
}

std::vector<LR::Situation> LR::GoTo(const std::vector<Situation>& items, char latter) {
    std::vector<Situation> result;

    for (const auto& item : items) {
        if (item.point_position == item.rule.right.size() || item.latter_after_point() != latter) continue;
        Situation new_situation = item;
        new_situation.point_position += 1;
        result.push_back(new_situation);
    }

    return Clouser(result);
}

void LR::BuildTermFirst() {
    bool is_changed = false;
    do {
        is_changed = false;

        for (const Rule& rule : grammar_.rules) {
            char cur_noterm = rule.left;
            bool is_eps_generate = true;
            for (auto latter : rule.right) {
                if(!firsts.contains(cur_noterm)) 
                    firsts[cur_noterm] = TermFirst(cur_noterm);

                if (grammar_.is_latter(latter)) {
                    // latter - in alphabet
                    is_changed = (is_changed || firsts[cur_noterm].push(latter));
                    is_eps_generate = false;
                    break;
                }
                // latter - is noterm
                if (!firsts.contains(latter))
                    firsts[latter] = TermFirst(latter);
                
                is_changed = (is_changed || firsts[cur_noterm].merge(firsts[latter]));
                
                if (!firsts[latter].is_eps_generated) {
                    is_eps_generate = false;
                    break;
                }
            }
            firsts[cur_noterm].is_eps_generated = is_eps_generate;
            if (firsts[cur_noterm].is_eps_generated) firsts[cur_noterm].push(kEndString);
        }

    } while (is_changed);
}

std::unordered_set<char> LR::FirstForStr(const std::string& str) {
    std::unordered_set<char> result;
    for (auto latter : str) {
        if (grammar_.is_latter(latter)) {
            if (!result.contains(latter)) result.insert(latter);
            break;
        }
        if (grammar_.is_noterm(latter)) {
            
            for (auto el : firsts[latter].first) {
                if (!result.contains(el)) result.insert(el);
            }
            if (!firsts.at(latter).is_eps_generated) break;
        }
        if (latter == kEndString) {
            result.insert(kEndString);
            break;
        }
    }

    return result;
}

void LR::BuildDfa() {
    std::unordered_map<std::vector<Situation>, int, HashVectorSit> map_items;
    std::queue<VertexDfa> item_queue;

    std::vector<Situation> start_items = 
        Clouser({Situation::StartSituation(grammar_)});
    all_vertex.push_back(VertexDfa(1, start_items));
    item_queue.push(all_vertex[0]);
    map_items[start_items] = 1;

    std::vector<char> all_latter = grammar_.AllLatter();

    while (!item_queue.empty()) {
        VertexDfa top = item_queue.front();
        item_queue.pop();

        for (auto latter : all_latter) {
            std::vector<Situation> go_to = GoTo(top.situations, latter);
            if (go_to.size() == 0) continue; ///????
            if (!map_items.contains(go_to)) {
                int cur_index = static_cast<int>(all_vertex.size()) + 1;
                all_vertex.push_back(VertexDfa(cur_index, go_to));
                map_items[go_to] = cur_index;
                item_queue.push(all_vertex[cur_index - 1]);
            }
            all_vertex[top.index - 1].transitions[latter] = map_items[go_to];
        }
    }
}

void LR::BuildLRTable() {
    std::map<Rule, int> indexes_rules = grammar_.IndexesOfRule();

    table.resize(all_vertex.size() + 1);

    for (auto vertex : all_vertex) {
        for (auto transition : vertex.transitions) {
            char cur_latter = transition.first;
            int next_vertex = transition.second;

            Action action;
            if (grammar_.is_latter(cur_latter)) action = Action(ActionEnum::SHIFT, next_vertex);
            else action = Action(ActionEnum::GOTO, next_vertex);

            if (!table[vertex.index].contains(cur_latter))
                table[vertex.index][cur_latter] = action;
        }
    } // goto and shift

    for (auto vertex : all_vertex) {
        for (auto item : vertex.situations) {
            if (item.point_position != item.rule.right.size()) continue;
            int index_of_rule = indexes_rules[item.rule];
            
            if (table[vertex.index].contains(item.latter_after)) {
                std::cout << "NO RL(1) Grammar!!!\n";
                break;
            }
            table[vertex.index][item.latter_after] = Action(ActionEnum::REDUCE, index_of_rule);
        }
    } // reduse
}

void LR::Shift(int vertex_to, char latter) {
    Cell first_new_cell = {Latter, -1, latter};
    Cell second_new_cell = {Index, vertex_to, ' '};
    lr_stack.push(first_new_cell);
    lr_stack.push(second_new_cell);
}

bool LR::Reduce(int number_of_rule) {
    Rule cur_rule = grammar_.rules[number_of_rule];
    std::string reduce_str = cur_rule.right;


    if (reduce_str.size() != 0) {
        if (lr_stack.size() < 2 * reduce_str.size()) return false;

        std::string cur_stack_str;
        for (int i = 0; i < reduce_str.size() * 2; ++i) {
            Cell top = lr_stack.top();
            lr_stack.pop();

            if (top.state == Index) continue;

            cur_stack_str += top.latter;
        }
        std::reverse(cur_stack_str.begin(), cur_stack_str.end());
        if (cur_stack_str != reduce_str) return false;
    }

    if (lr_stack.empty()) {
        std::cout << "EPTY STACK!!!\n";
        return false;
    } // ??
    Cell index_for_goto = lr_stack.top();

    lr_stack.push({Latter, -1, cur_rule.left});
    
    Action action = table[index_for_goto.index].at(cur_rule.left);
    if (action.type != ActionEnum::GOTO) {
        std::cout << "Expected GOTO\n";
        return false;
    } // ?
    lr_stack.push({Index, action.number, ' '});

    return true;
}


LR::LR(const Grammar& grammar) : grammar_(grammar) {
    BuildTermFirst();
    BuildDfa();
    BuildLRTable();
}

bool LR::Parse(const std::string& word) {
    std::string copy_word = word;
    copy_word += kEndString;
    int cur_index = 0;
    lr_stack.push({CellState::Index, 1, ' '});

    while (cur_index != copy_word.size() && !lr_stack.empty()) {
        char cur_token = copy_word[cur_index];
        int cur_vertex = lr_stack.top().index;
        
        Action action;
        try {
            action = table[cur_vertex].at(cur_token);
        }
        catch(...) {
            return false;
        }

        if (action.type == ActionEnum::SHIFT) {
            Shift(action.number, cur_token);
            cur_index += 1;
        } else if (action.type == ActionEnum::REDUCE) {
            if (action.number == 1 && cur_token == kEndString) {
                // reduce (1) by $
                return Reduce(action.number);
            }
            if (!Reduce(action.number)) {
                std::cout << "WRONG WORD\n";
                return false;
            }
        }

    }

    return false;
}