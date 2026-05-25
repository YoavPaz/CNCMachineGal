#include "main.h"

main m;

ParamDef G1Params[] = {
  {'X', Type::Float},
  {'Y', Type::Float},
  {'Z', Type::Float},
  {'F', Type::TempInt}
};

void setup(){
  Serial.begin(115200);
  m.setup(115200);

  m.registerCommand("G1", G1Params, 4);
}

void loop(){
  m.readSerial();
}
