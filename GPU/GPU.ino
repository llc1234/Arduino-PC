#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

int memory[32];

const byte rxPin = 2;
const byte txPin = 3;

SoftwareSerial PC_Serial =  SoftwareSerial(rxPin, txPin);

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup() {
  Serial.begin(9600);
  PC_Serial.begin(9600);

  lcd.init(); // starting lcd
  lcd.backlight();

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("0123456789");
  lcd.setCursor(0, 1);
  lcd.print("0123456789");

  for (int i = 0; i < 32; i++) {
    memory[i] = -254;
  }
  
}

void set_address(int addr, int value_) {
  memory[addr] = value_;
}

int get_address(int addr, int value_) {
  return memory[addr];
}

void updateScreen() {
  lcd.clear();
  lcd.setCursor(0, 0);
  
  for (int i = 0; i < 32; i++) {
    if (i == 16) {
      lcd.setCursor(0, 1);
    }

    if (memory[i] != -254) {
      lcd.print(memory[i]);
    }
  }
}

void loop() {
  if (PC_Serial.available() > 0) {
        String row_1 = PC_Serial.readStringUntil('\n');
        String row_2 = PC_Serial.readStringUntil('\n');
        String row_3 = PC_Serial.readStringUntil('\n');

        int row_1_int = row_1.toInt();
        int row_2_int = row_2.toInt();
        int row_3_int = row_3.toInt();

        Serial.print(row_1_int);
        Serial.print(" ");
        Serial.print(row_2_int);
        Serial.print(" ");
        Serial.println(row_3_int);

        if (row_1_int == 3) {
          set_address(row_2_int, row_3_int);
          updateScreen();
          
        }
  }
}
