#include "main.h"
#include "functions.h"

main m;

ParamDef G1Params[] = {
  {'X', Type::Float},
  {'Y', Type::Float},
  {'Z', Type::Float},
  {'F', Type::TempInt}
};

ParamDef G0Params[] = {
  {'X', Type::Float},
  {'Y', Type::Float},
  {'Z', Type::Float},
  {'F', Type::TempInt}
};

void setup(){
  Serial.begin(115200);
  m.setup(115200);

  m.registerCommand("G1", G1Params, 4, functions::G1Move);
  m.registerCommand("G0", G0Params, 4, functions::G0Move);
}

void loop(){
  m.readSerial();
}


/*String buffer = "";

void setup() {
    Serial.begin(115200);

    delay(2000);

    Serial.println("READY");
}

void loop() {
    while (Serial.available() > 0) {

        char c = Serial.read();

        // ignore carriage return
        if (c == '\r')
            continue;

        // newline = complete message
        if (c == '\n') {

            Serial.print("Message got: ");
            Serial.println(buffer);

            buffer = "";
        }
        else {
            buffer += c;
        }
    }
}
*/
