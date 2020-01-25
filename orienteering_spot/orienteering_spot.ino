/*
	OpenSource Orienteering Control Point
 Putting together a few components, you can build a cheap control point.
	Here we have: 
 - a standard Arduino 2009 / UNO or similar 
 - a RTC module to keep track of date and time between reset
 - a speaker to advise users that the reading / writing of the tag is done successfully
 - a RFID antenna (the old-but-gold and cheap MFRC522)
 This control point is able to:
 - read the rfid of the user
 - write on the memory of his tag info related to date, time and the ID of the station itself.
	
	So the player, at the arrival, will have only to put his tag on the base station to check times and stations visited.

 CC BY-SA 
*/

//RTC
#include <Wire.h>
#include "RTClib.h"
//RFID
#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9

//#define DEBUG

int notes[] = {294, 262};

MFRC522 mfrc522(SS_PIN, RST_PIN);        // Create MFRC522 instance.

RTC_DS1307 RTC;
/*
byte STAZIONE = 1;
byte block = 61;
*/

byte STAZIONE = 2;
byte block = 62;

void setup () {
  Serial.begin(9600);

  SPI.begin();                // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card

  Wire.begin();
  RTC.begin();

  //if RTC is new or with a discharger battery ,proceed with adjustmenf
  //RTC.adjust(DateTime(__DATE__, __TIME__));
}

void loop () {
  DateTime now = RTC.now();

#ifdef DEBUG
  Serial.print(now.hour());
  Serial.print(" ");
  Serial.println(now.minute());
#endif

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
  byte status;

  unsigned int year  = now.year();

  buffer[0] = STAZIONE;
  buffer[1] = now.day();
  buffer[2] = now.month();
  buffer[3] = 14; //year
  buffer[4] = now.minute();
  buffer[5] = now.hour();
  for (int i = 6; i < 16; i++) {
    buffer[i] = 255;
  }

  //Serial.println("Authenticating using key A...");
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
#ifdef DEBUG
    Serial.print("PCD_Authenticate() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
#endif
    return;
  }

  status = mfrc522.MIFARE_Write(block, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
#ifdef DEBUG
    Serial.print("MIFARE_Write() failed: ");
    Serial.println(mfrc522.GetStatusCodeName(status));
#endif
    return;
  }
  else {
#ifdef DEBUG
    Serial.println("MIFARE_Write() success: ");
#endif 
tone(8, 3000,1000);

    delay(1000);
    noTone(8);
    delay(5000);
  }

  // Halt PICC
  mfrc522.PICC_HaltA();

  // Stop encryption on PCD
  mfrc522.PCD_StopCrypto1();
}
