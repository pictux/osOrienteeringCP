/*
  OpenSource Orienteering Base Station
  Putting together a few components, you can build a cheap base station (that is the same as the control point!).
  Here we have:
  - a standard Arduino 2009 / UNO or similar
  - a RTC module to keep track of date and time between reset
  - a speaker to advise users that the reading / writing of the tag is done successfully
  - a RFID antenna (the old-but-gold and cheap MFRC522)
  This control point is able to:
  - read the rfid of the user
  - eventually reset the memory of his tag for the next challenge

  So the player, at the arrival, put his tag on the base station, and it will retrieve times and stations visited.

  CC BY-SA
*/

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522 instance.

void setup() {
  Serial.begin(9600); // Initialize serial communications with the PC
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522 card
  Serial.println("Scan PICC to see UID and type...");
}

void loop() {
  byte status;

  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }
  // Now a card is selected. The UID and SAK is in mfrc522.uid.

#ifdef DEBUG
  //tag dump
  // Dump UID
  Serial.print("Card UID:");
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  }
  Serial.println();
#endif

  // Dump PICC type
  byte piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.print("PICC type: ");
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
  if (        piccType != MFRC522::PICC_TYPE_MIFARE_MINI
              &&        piccType != MFRC522::PICC_TYPE_MIFARE_1K
              &&        piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    //Serial.println("This sample only works with MIFARE Classic cards.");
    return;
  }

  byte buffer[16];

  byte size = sizeof(buffer);
  // change this: valueBlockA , for read anather block
  // cambiate valueBlockA per leggere un altro blocco
  status = mfrc522.MIFARE_Read(62, buffer, &size);


  for (byte index = 0; index < 16; index++) {
    Serial.print(buffer[index] < 0x10 ? " 0" : " ");
    Serial.print(buffer[index], HEX);
    if ((index % 4) == 3) {
      Serial.print(" ");
    }
  }

  // Dump debug info about the card. PICC_HaltA() is automatically called.
  //mfrc522.PICC_DumpToSerial(&(mfrc522.uid));

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}
