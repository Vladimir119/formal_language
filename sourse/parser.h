#include "reader.h"
#include "grammar.h"

class Parser{
  private:
    static std::pair<std::string, std::string> Split(const std::string& str, const std::string& separator);

    static bool IsContextFree(const std::string& noterm);

  public:
    static Grammar Parse(Reader& reader);
};