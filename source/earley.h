#include "parser.h"

class Earley {
 private:

  struct Situation {
    Rule rule;
    int point_position;
    int count_of_read;

    Situation() = default;
    Situation(const Rule& rule, int point_position, int count_of_read);

    bool operator==(const Situation& other) const = default;

    struct Hash {
        size_t operator()(const Situation& situation) const;
    };
  };

  struct Layer {
    std::unordered_set<Situation, Situation::Hash> situations;
    std::map<char, std::vector<Situation>> situation_with_dot_before_noterm;
    std::vector<Situation> ordered_situation;
    size_t index_nocomplete;
    size_t index_nopredict;
    bool is_changed;

    Layer();
    
    void Add(const Situation& situation, const Grammar& grammar);
    void ResetIsChanged();
    bool Contain(const Situation& situation);
  };

  Grammar grammar_;
  std::vector<Layer> layers;

  void ExpentGrammar();

  void Scan(int layer_number, const std::string& word);

  void Predict(int layer_number, const std::string& word);

  void Complete(int layer_number, const std::string& word);

  bool Changes(int layer_number);

 public:
  Earley() = default;
  Earley(const Grammar& grammar);

  void ReadGrammar(Reader& reader);
  void Check(const std::string& word);
};