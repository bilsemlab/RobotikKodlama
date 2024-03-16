#include <IRremote.hpp>

#define IR_RECEIVE_PIN 2

#define IR_BUTTON_1 69
#define IR_BUTTON_2 70
#define IR_BUTTON_3 71
#define IR_BUTTON_OK 28
#define RED 9
#define BLUE 10
#define GREEN 11

void setup() {
  pinMode(RED,OUTPUT);
  pinMode(GREEN,OUTPUT);
  pinMode(BLUE,OUTPUT);
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
}

void loop() {
  if (IrReceiver.decode()) {
    IrReceiver.resume();
    int command = IrReceiver.decodedIRData.command;
    Serial.println(command);
    switch (command) {
      case IR_BUTTON_1: {
          Serial.println("Kirmizi yandi");
          digitalWrite(RED,HIGH);
          digitalWrite(BLUE,LOW);
          digitalWrite(GREEN,LOW);
          break;
        }
      case IR_BUTTON_2: {
          Serial.println("Mavi yandi");
          digitalWrite(RED,LOW);
          digitalWrite(BLUE,HIGH);
          digitalWrite(GREEN,LOW);
          break;
        }
      case IR_BUTTON_3: {
          Serial.println("Yesil yandi");
          digitalWrite(RED,LOW);
          digitalWrite(BLUE,LOW);
          digitalWrite(GREEN,HIGH);
          break;
        }
      case IR_BUTTON_OK: {
          Serial.println("Pressed on button play/pause");
          digitalWrite(RED,LOW);
          digitalWrite(BLUE,LOW);
          digitalWrite(GREEN,LOW);
          break;
        }
      default: {
          Serial.println("Button not recognized");
        }
    }
  }
}
