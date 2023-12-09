#include <string>
#include <map>
#include <unordered_set>
#include <vector>

static const char kStartSymbol = '@';
static const char kEmptyString = '#';
static const std::string kSeparator = "->";

struct Rule{
    char left;
    std::string right;

    Rule();
    Rule(char left, const std::string& right);
    bool operator==(const Rule& other) const = default;

    bool operator<(const Rule& other) const;
};

struct Grammar{
    int size_noterm;
    int size_alphabet;
    int size_grammar; 
    char start_noterm;
    std::map<char, std::vector<Rule>> transitions;
    std::vector<Rule> rules;

    std::unordered_set<char> noterminals;
    std::unordered_set<char> alpabet;

    bool is_latter(char latter) const { return alpabet.contains(latter); }
    bool is_noterm(char latter) const { return noterminals.contains(latter); }

    Rule StartRule() const { return Rule(kStartSymbol, std::string(1, start_noterm)); }
    std::vector<char> AllLatter() const;

    Grammar();
    Grammar(int size_noterm,
            int size_alphabet,
            int size_grammar,
            const std::unordered_set<char> noterminals,
            const std::unordered_set<char> alpabet,
            const std::map<char, std::vector<Rule>>& transitions,
            char start_noterm);

    std::map<Rule, int> IndexesOfRule() const;
    void ExpendGrammar();
};
