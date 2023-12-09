#include "parser.h"

#include <queue>
#include <unordered_map>
#include <stack>

const char kEndString = '$';

class LR {
  private:

    struct Situation {
        Rule rule;
        int point_position;
        char latter_after;

        Situation() = default;
        Situation(const Rule& rule, int point_position, char latter_after);

        bool operator==(const Situation& other) const = default;

        bool is_point_before_noterm(const Grammar& grammar) const;

        char latter_after_point() const;

        struct Hash {
            size_t operator()(const Situation& situation) const;
        };

        static Situation StartSituation(const Grammar& grammar);
    };
   
    struct TermFirst {
        char noterm;
        std::unordered_set<char> first;
        bool is_eps_generated;

        TermFirst();
        TermFirst(char noterm);

        bool push(char latter);

        bool merge(const TermFirst& other);
    };
    
    struct VertexDfa{
        int index;
        std::vector<Situation> situations;
        std::map<char, int> transitions;

        VertexDfa(int index, const std::vector<Situation>& situations);
    };

    struct HashVectorSit {
        size_t operator()(const std::vector<Situation>& situations) const;
    };

    enum ActionEnum {
        SHIFT,
        REDUCE,
        GOTO,
    };

    struct Action {
        ActionEnum type;
        int number;

        Action() = default;
        Action(ActionEnum type, int number);
    };

    enum CellState{
        Index,
        Latter
    };

    struct Cell{
        CellState state;
        
        int index;
        char latter;
    };

    Grammar grammar_;
    std::map<char, TermFirst> firsts;

    std::vector<VertexDfa> all_vertex;
    std::vector<std::map<char, Action>> table;
    
    std::stack<Cell> lr_stack;

    std::vector<Situation> Clouser(const std::vector<Situation>& items);
    std::vector<Situation> GoTo(const std::vector<Situation>& items, char latter);

    void BuildTermFirst();

    std::unordered_set<char> FirstForStr(const std::string& str);

    void BuildDfa();

    void BuildLRTable();
    
    void Shift(int vertex_to, char latter);

    bool Reduce(int number_of_rule);

 public:
    LR(const Grammar& grammar);

    bool Parse(const std::string& word);
};
