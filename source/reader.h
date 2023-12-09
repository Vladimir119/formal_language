#include <iostream>
#include <string>
#include <fstream>

class Reader{
  public:
    virtual std::string getLine() = 0;
};

class ConsoleReader : public Reader{
  public:
    virtual std::string getLine() override;
};

class FileReader : public Reader{
  private:
    std::fstream f_;
  public:
    FileReader(std::string path);
    virtual std::string getLine() override;
};