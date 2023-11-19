#include <SoftwareSerial.h>

// Array to simulate memory with 127 addresses
int memory[127];

const byte rxPin = 2;
const byte txPin = 3;

SoftwareSerial PC_Serial =  SoftwareSerial(rxPin, txPin);

void setup() {
  // Serial.begin(9600);
  PC_Serial.begin(9600);
  
}

// Function to set a value at a specific memory address
void set_address(int addr, int value_) {
  memory[addr] = value_;
}

// Function to get the value at a specific memory address
int get_address(int addr, int value_) {
  return memory[addr];
}

void loop() {
  // Check if there is data available on the SoftwareSerial port
  if (PC_Serial.available() > 0) {
        // Read three rows of data from the SoftwareSerial port
        String row_1 = PC_Serial.readStringUntil('\n');
        String row_2 = PC_Serial.readStringUntil('\n');
        String row_3 = PC_Serial.readStringUntil('\n');

        // Convert the read strings to integers
        int row_1_int = row_1.toInt();
        int row_2_int = row_2.toInt();
        int row_3_int = row_3.toInt();

        // Check the command type
        if (row_1_int == 1) {
          // If the command is to get a value
          // Serial.println("Get");
          // Get the value from the specified memory address and send it back
          PC_Serial.println(get_address(row_2_int, row_3_int));
        } else if (row_1_int == 2) {
          // If the command is to set a value
          // Serial.println("Set");
          // Set the value at the specified memory address
          set_address(row_2_int, row_3_int);
        }
  }
}
