#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>

// Assembly instruction opcodes
// mov  ; hex 45  ; move addresse into a diffrent address
// add  ; hex 64  ; add a address into a diffrent address
// sub  ; hex 32  ; 
// jmp  ; hex 55  ; 
// cmp  ; hex 80  ; 

// Conditional jump opcodes
// ja   ; hex 15  ;  >  ; jump if greater
// jae  ; hex 43  ;  >= ; jump if greater or equal
// jb   ; hex 67  ;  <  ; jump if less
// jbe  ; hex 88  ;  <= ; jump if less or equal
// je   ; hex 44  ;  == ; jump if equal

// dis  ; hex 24  ; display on screen

File sd_card;

int row = 0;

int cmp_flag[2] = {0, 0}; // Flags for comparison results

const int chipSelect = 10;

const byte rxPin = 2;
const byte txPin = 3;

SoftwareSerial PC_Serial =  SoftwareSerial(rxPin, txPin);

// Function to read data from RAM
int get_address(int addr) {
  PC_Serial.println(1);    // Command for RAM read
  PC_Serial.println(addr); // Address to read
  PC_Serial.println(0);    // Placeholder value

  // while (PC_Serial.available() == 0) {}
  
  return PC_Serial.readStringUntil('\n').toInt();
}

// Function to write data to RAM
void set_address(int addr, int valu) {
  PC_Serial.println(2);  // RAM set
  PC_Serial.println(addr); // address 44
  PC_Serial.println(valu);  // set value
}

// Function to write data to GPU memory
void set_address_GPU(int addr, int valu) {
  PC_Serial.println(3);     // Command for GPU memory write
  PC_Serial.println(addr);  // Address to write
  PC_Serial.println(valu);  // Value to set
}

// Function to read code from SD card and write it to RAM
void code_from_sd_to_ram() {
  int address_counter = 0;
  
  SD.begin();
  
  sd_card = SD.open("index.txt", FILE_READ);
  
  if (sd_card) {
    while (sd_card.available()) {
      // Read a line of code from the SD card
      String code = sd_card.readStringUntil('\n');
      int myInt = code.toInt();

      // Print the code to the Serial Monitor
      Serial.println(myInt);
      delay(400);

      // Write the code to the specified address in RAM
      set_address(address_counter, myInt);

      address_counter++;
      
    }
    
    sd_card.close();
  }
}


// Assembly instructions

void add_() {
  Serial.println("add");

  // Get addresses and values
  int addr1 = get_address(row + 1);
  int addr2 = get_address(row + 2);

  int valu1 = get_address(addr1);
  int valu2 = get_address(addr2);

  // Perform addition and update the value in RAM
  set_address(addr1, valu2 + valu1);
}

void sub_() {
  Serial.println("sub");

  // Get addresses and values
  int addr1 = get_address(row + 1);
  int addr2 = get_address(row + 2);

  int valu1 = get_address(addr1);
  int valu2 = get_address(addr2);

  // Perform subtraction and update the value in RAM
  set_address(addr1, valu1 - valu2);
}

void jmp_() {
  Serial.println("jmp");

  // Jump to the specified address
  int addr1 = get_address(row + 1);
  row = addr1 - 1;
}

void cmp_() {
  Serial.println("cmp");
  
  // Get addresses and values for comparison
  int addr1 = get_address(row + 1);
  int addr2 = get_address(row + 2);

  int valu1 = get_address(addr1);
  int valu2 = get_address(addr2);

  // Update comparison flags
  cmp_flag[0] = valu1;
  cmp_flag[1] = valu2;
}

void je_() {
  Serial.println("je");

  // Jump if equal
  if (cmp_flag[0] == cmp_flag[1]) {
    int addr1 = get_address(row + 1);
    row = addr1 - 1;
  } else {
    row += 2;
  }
}

void ja_() {
  Serial.println("ja");

  // Jump if greater
  if (cmp_flag[0] > cmp_flag[1]) {
    int addr1 = get_address(row + 1);
    row = addr1 - 1;
  } else {
    row += 2;
  }
}

void jae_() {
  Serial.println("jae");

  // Jump if greater or equal
  if (cmp_flag[0] >= cmp_flag[1]) {
    int addr1 = get_address(row + 1);
    row = addr1 - 1;
  } else {
    row += 2;
  }
}

void jb_() {
  Serial.println("jb");

  // Jump if less
  if (cmp_flag[0] < cmp_flag[1]) {
    int addr1 = get_address(row + 1);
    row = addr1 - 1;
  } else {
    row += 2;
  }
}

void jbe_() {
  Serial.println("jbe");

  // Jump if less or equal
  if (cmp_flag[0] <= cmp_flag[1]) {
    int addr1 = get_address(row + 1);
    row = addr1 - 1;
  } else {
    row += 2;
  }
}

void dis_() {
  Serial.println("dis");
  int addr1 = get_address(row + 1);
  int addr2 = get_address(row + 2);

  int valu1 = get_address(addr1);
  int valu2 = get_address(addr2);

  // Write data to GPU memory for display
  set_address_GPU(valu1, valu2);
}

void mov_() {
  Serial.println("mov");

  // Move value from one address to another
  int addr1 = get_address(row + 1);
  int addr2 = get_address(row + 2);

  int valu = get_address(addr2);

  // Set the value at the destination address
  set_address(addr1, valu);
}

// Function to read and execute assembly code
void read_assembly() {
  int r = 0;
  while (r < 30) { // change to 127 if ram is 127
    // Print the content of RAM for debugging
    Serial.print(get_address(r));
    delay(200);
    r++;
  }
  Serial.println(" ");
  
  // Read and execute assembly code
  while (row < 50) { // change to 127 if ram is 127
    int val = get_address(row);
    
    if (val == 45) { // mov
      mov_();
      row += 2;
    } else if (val == 64) { // add
      add_();
      row += 2;
    } else if (val == 32) { // sub
      sub_();
      row += 2;
    } else if (val == 55) { // jmp
      jmp_();
    } else if (val == 80) { // cmp
      cmp_();
    } else if (val == 44) { // je
      je_();
    } else if (val == 15) { // ja
      ja_();
    } else if (val == 43) { // jae
      jae_();
    } else if (val == 67) { // jb
      jb_();
    } else if (val == 88) { // jbe
      jbe_();
    } else if (val == 24) { // dis
      dis_();
    }
    
    row++;

    delay(200);
  }

  
  r = 0;
  while (r < 30) { // change to 127 if ram is 127
    Serial.print(get_address(r));
    delay(200);
    r++;
  }
  Serial.println(" ");
}

void setup() {
  Serial.begin(9600);
  PC_Serial.begin(9600);

  code_from_sd_to_ram();
  delay(1000);
  read_assembly();
  
}

void loop() {
  
}
