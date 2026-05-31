#ifndef MAIN_H
#define MAIN_H

#include <Arduino.h>

enum class Type {
    Float,
    Int,
    TempFloat,
    TempInt
};

enum class Error{
    None,
    MoveError,
    TypeError,
    UnkownCommandError,
};

struct ParamDef {
    char name;
    Type type;

    char* getTypeStr(Error &err){
        err = Error::None;
        switch (type)
        {
        case Type::Float: return "Float";
        case Type::Int: return "Int";
        case Type::TempFloat: return "TempFloat";
        case Type::TempInt: return "TempInt";
        
        default:
            err = Error::TypeError;
            return "Unknown";
            break;
        }
    }
};

struct Command {
    char* name;
    ParamDef* params;
    int paramsCount;
};

class main{
  public:
  main();
  void setup(int bdrate);
  void readSerial();
  void registerCommand(char* name, ParamDef* params, int paramsCount);
  void parseLine(char* line);
  void getArg(char* line, int &c_index, char c_end, char* &out);
  void createCommand(Command &command, char* name, ParamDef* params,
                    int paramCount);
  void findCommand(char* command, Command &out, Error* err = nullptr);
  float getFloat(char* line, int &index);
  int getInt(char* line, int &index);
  private:
  Command commands[128];
  int current_command = 0;
};

#endif
