/*
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN         9        
#define SS_PIN          10         

int ledGranted = 3;
int ledDenied = 4;
int buzzPort = 5;

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);  
  pinMode(ledGranted, OUTPUT);
  pinMode(ledDenied, OUTPUT);
  pinMode(buzzPort, OUTPUT);
  noTone(buzzPort);
  SPI.begin();      // Init SPI bus
  mfrc522.PCD_Init();   // Init MFRC522
  delay(4);       // Optional delay. Some board do need more time after init to be ready
  mfrc522.PCD_DumpVersionToSerial();  // Show details of PCD - MFRC522 Card Reader details
  Serial.print("Please scan your card Sir");
  Serial.println();
  //Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  String UID = "";

  for(byte i = 0; i < mfrc522.uid.size; ++i)
  {
    UID.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  UID.toUpperCase();
  Serial.print(UID);
  Serial.println();
  if(UID == "F9AC56E")
  {
    Serial.print("Acces Granted");
    Serial.println();
    digitalWrite(ledGranted, HIGH);
    tone(buzzPort, 700);
    delay(300);
    noTone(buzzPort);
    delay(3000);
    digitalWrite(ledGranted, LOW);
  }
  else
  {
    Serial.print("Acces Denied");
    Serial.println();
    digitalWrite(ledDenied, HIGH);
    tone(buzzPort, 300);
    delay(1000);
    noTone(buzzPort);
    delay(3000);
    digitalWrite(ledDenied, LOW);
  }
  // Dump debug info about the card; PICC_HaltA() is automatically called
  mfrc522.PICC_DumpToSerial(&(mfrc522.uid));
}
