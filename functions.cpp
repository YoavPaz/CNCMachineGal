#include "functions.h"

void functions::G1Move(ParamValue* params, int count){
    Serial.println("Moving G1 Values:");
    for (int i = 0; i < count; i++){
        ParamValue paramValue = params[i];
        Serial.print(paramValue.name);
        Serial.print(": ");
        if (paramValue.type == Type::Float)
            Serial.print(paramValue.value.FloatValue);
        if (paramValue.type == Type::Int)
            Serial.print(paramValue.value.FloatValue);
        Serial.println();
    }
}

void functions::G0Move(ParamValue* params, int count){
    Serial.println("Moving G0 Values:");
    for (int i = 0; i < count; i++){
        ParamValue paramValue = params[i];
        Serial.print(paramValue.name);
        Serial.print(": ");
        if (paramValue.type == Type::Float)
            Serial.print(paramValue.value.FloatValue);
        if (paramValue.type == Type::Int)
            Serial.print(paramValue.value.FloatValue);
        Serial.println();
    }
}
