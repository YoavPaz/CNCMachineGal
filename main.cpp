#include "main.h"

main::main(){
  
}

void main::setup(int bdrate){
    //Serial.begin(bdrate);

    Serial.println("Started cnc rooter!");
}

void main::createCommand(Command &command, char* name, ParamDef* params, int paramCount, Action action){
    command.name = name;
    command.params = params;
    command.paramsCount = paramCount;
    command.action = action;
}

void main::registerCommand(char* name, ParamDef* params,
                            int paramsCount, Action action){
    Serial.print("Registering command ");
    Serial.println(name);
    Command cmd;
    main::createCommand(cmd, name, params, paramsCount, action);
    commands[current_command++] = cmd;

    /*for (int j = 0; j < current_command; j++){
        Serial.print("for Command: ");
        Serial.println(commands[j].name);
        for (int i = 0; i < commands[j].paramsCount; i++){
            ParamDef param = commands[j].params[i];
            Serial.print(param.name);
            Serial.print(":");
            Error typeE;
            char* typeStr = param.getTypeStr(typeE);
            if (typeE == Error::TypeError) Serial.println("Type error.");
            else Serial.println(typeStr);
        }
    }*/

    Serial.println("--------------------------");
}

void main::parseLine(char* line){
    int index = 0;
    char command[4];
    char* command_ptr = command;

    main::getArg(line, index, ' ', command_ptr);
    Serial.print("command: ");
    Serial.println(command);
    
    Command cmd;
    Error err;
    main::findCommand(command, cmd, &err);
    if (err == Error::UnkownCommandError){
        Serial.println("[Error] UnkownCommand!");
        return;
    }

    Serial.print("Found command name: ");
    Serial.println(cmd.name);

    ParamValue paramValues[5];
    int currnet_param_num = 0;
    for (int i = 0; i < cmd.paramsCount; i++){
        ParamDef param = cmd.params[i];
        Type type = param.type;

        int temp_index = index;
        
        switch (type){
            case Type::Int:
            {
                char temp[4];
                char* tempPtr = temp;

                main::getArg(line, temp_index, param.name, tempPtr);
                int intValue = main::getInt(line, temp_index);

                ParamValue paramValue(param.name, intValue);
                paramValues[currnet_param_num++] = paramValue;

                Serial.print("Got Int Value: ");
                Serial.println(intValue);
                break;
            }

            case Type::Float:
            {
                char temp[4];
                char* tempPtr = temp;

                main::getArg(line, temp_index, param.name, tempPtr);
                float floatValue = main::getFloat(line, temp_index);
                
                ParamValue paramValue(param.name, floatValue);
                paramValues[currnet_param_num++] = paramValue;

                Serial.print("Got Float Value: ");
                Serial.println(floatValue);
                break;
            }
        }
        
    }

    cmd.action(paramValues, currnet_param_num);
}

int main::getInt(char* line, int &index){
    int value = 0;

    while (line[index] == ' ')
        index++;

    while (line[index] >= '0' && line[index] <= '9')
    {
        value = value * 10 + (line[index] - '0');
        index++;
    }

    return value;
}

float main::getFloat(char* line, int &index){
    float value = 0.0f;
    bool negative = false;

    while (line[index] == ' ') index++;

    if (line[index] == '-')
    {
        negative = true;
        index++;
    }

    while (line[index] >= '0' && line[index] <= '9')
    {
        value = value * 10 + (line[index] - '0');
        index++;
    }

    if (line[index] == '.')
    {
        index++;

        float factor = 0.1f;

        while (line[index] >= '0' && line[index] <= '9')
        {
            value += (line[index] - '0') * factor;
            factor *= 0.1f;
            index++;
        }
    }

    if (negative)
        value = -value;

    return value;
}

void main::findCommand(char* command, Command &out, Error* err = nullptr)
{
    bool found_command = false;
    for (int i = 0; i < current_command; i++){
        Serial.print("Checking if ");
        Serial.print(commands[i].name);
        Serial.print(" is ");
        Serial.println(command);
        if (strcmp(commands[i].name, command) == 0){
            out = commands[i];
            found_command = true;
        }
    }
    if (err != nullptr) *err = Error::None;
    if (found_command || err == nullptr) return;
    *err = Error::UnkownCommandError;
}

void main::getArg(char* line, int &c_index, char c_end, char* &out)
{
    int b_index = 0;

    while (line[c_index] != '\0' &&
           line[c_index] != c_end)
        out[b_index++] = line[c_index++];

    out[b_index] = '\0';

    if (line[c_index] == c_end)
    {
        c_index++;
    }
}

void main::readSerial()
{
    static char buffer[128];
    static int currentBuffer = 0;
    static bool ignoreUntilNewline = false;

    while (Serial.available() > 0)
    {
        char c = Serial.read();

        if (c == '\r')
            continue;

        if (c == ';')
        {
            buffer[currentBuffer] = '\0';

            //Serial.print("CMD RECEIVEDDDDDDDDDDDDddD: ");
            Serial.println(buffer);
            

            currentBuffer = 0;
            ignoreUntilNewline = true;

            continue;
        }

        if (c == '\n')
        {
            buffer[currentBuffer] = '\0';

            Serial.print("CMD RECEIVED: ");
            Serial.println(buffer);
            main::parseLine(buffer);

            currentBuffer = 0;
            ignoreUntilNewline = false;

            continue;
        }

        if (ignoreUntilNewline)
            continue;

        if (currentBuffer < sizeof(buffer) - 1)
        {
            buffer[currentBuffer++] = c;
        }
    }
}
