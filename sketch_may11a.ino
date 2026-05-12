/*
This is just a test sketch
Which must use module: RFID-RC522
I use esp32S which has 38 pins, so pin numbers are according to my board
*/

#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 22
#define SDA_PIN 21
#define MOSI_PIN 23
#define MISO_PIN 19
#define SCK_PIN 18
#define IRQ_PIN 16

/*
- RST_PIN (GPIO22): Reset pin for the RC522 module. Used to reset the module.
- SDA_PIN (GPIO21): Slave Select (SS) pin for SPI communication. Used to select the RC522 module when communicating over SPI.
- MOSI_PIN (GPIO23): Master Out Slave In (MOSI) pin for SPI communication. Used to send data from the ESP32 to the RC522 module.
- MISO_PIN (GPIO19): Master In Slave Out (MISO) pin for SPI communication. Used to receive data from the RC522 module to the ESP32.   
- SCK_PIN (GPIO18): Serial Clock (SCK) pin for SPI communication. Used to provide the clock signal for SPI communication between the ESP32 and the RC522 module.
- IRQ_PIN (GPIO16): Interrupt Request (IRQ) pin. Used to signal the ESP32 when the RC522 module has data to send or an event occurs, allowing for efficient handling of RFID events without constant polling.
*/

/*
Text art of esp32s dev board with those pins marked above

                 +-------------------------------+
  3V3       o----|                               |----o GND
  EN        o----|                               |----o MOSI/23
  SVP/36    o----|                               |----o RST/22
  SVN/39    o----|                               |----o TX0 / GPIO1
  GPIO34    o----|                               |----o RX0 / GPIO3
  GPIO35    o----|                               |----o SDA/21
  GPIO32    o----|                               |----o GND
  GPIO33    o----|                               |----o MISO/19
  GPIO25    o----|                               |----o SCK/18
  GPIO26    o----|                               |----o GPIO5
  GPIO27    o----|                               |----o GPIO17
  GPIO14    o----|                               |----o IRQ/16
  GPIO12    o----|                               |----o GPIO4
  GND       o----|                               |----o GPIO0
  GPIO13    o----|                               |----o GPIO2
  GPIO9     o----|                               |----o GPIO15
  GPIO10    o----|                               |----o SD1 / GPIO8
  GPIO11    o----|                               |----o SD0 / GPIO7
  5V        o----|                               |----o CLK / GPIO6
                 +-------------------------------+

  RC522 pins: SS(GPIO21), SCK(GPIO18), MOSI(GPIO23), MISO(GPIO19), RST(GPIO22), IRQ(GPIO16)
  Power: 3V3 and GND

  Source: https://www.espboards.dev/esp32/nodemcu-32s/
*/

MFRC522 rfid(SDA_PIN, RST_PIN);

volatile bool irqFired = false;

void IRAM_ATTR handleIRQ() {
    irqFired = true;
}

static void printHex(const byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        if (buffer[i] < 0x10) Serial.print('0');
        Serial.print(buffer[i], HEX);
        if (i + 1 < bufferSize) Serial.print(':');
    }
}

static void printDec(const byte *buffer, byte bufferSize) {
    for (byte i = 0; i < bufferSize; i++) {
        Serial.print(buffer[i], DEC);
        if (i + 1 < bufferSize) Serial.print(' ');
    }
}

void trackIncomingSignals() {
    byte gain = rfid.PCD_ReadRegister(MFRC522::RFCfgReg);
    byte status1 = rfid.PCD_ReadRegister(MFRC522::Status1Reg);
    byte status2 = rfid.PCD_ReadRegister(MFRC522::Status2Reg);

    Serial.print("RF Gain:   0x"); Serial.println(gain, HEX);
    Serial.print("Status1:   0x"); Serial.println(status1, HEX);
    Serial.print("Status2:   0x"); Serial.println(status2, HEX);
}
void setup() {
    Serial.begin(115200);
    delay(200);

    Serial.println();
    Serial.println(F("RC522 RFID test starting..."));

    SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, SDA_PIN);

    rfid.PCD_Init();

    rfid.PCD_WriteRegister(MFRC522::ComIEnReg, 0x20);
    rfid.PCD_WriteRegister(MFRC522::ComIEnReg, 0x04);

    pinMode(IRQ_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(IRQ_PIN), handleIRQ, FALLING);

    delay(50);
    Serial.print(F("MFRC522 Firmware Version: 0x"));
    Serial.println(rfid.PCD_ReadRegister(MFRC522::VersionReg), HEX);
    Serial.println(F("Scan an RFID tag/card to see its UID."));
}

void loop() {
    if (!rfid.PICC_IsNewCardPresent()) {
        delay(20);
        return;
    }

    if (!rfid.PICC_ReadCardSerial()) {
        delay(20);
        return;
    }

    Serial.println();
    Serial.print(F("UID (HEX): "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    Serial.print(F("UID (DEC): "));
    printDec(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();

    MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    Serial.print(F("PICC type: "));
    Serial.println(rfid.PICC_GetTypeName(piccType));

    trackIncomingSignals();

    rfid.PICC_HaltA();
    rfid.PCD_StopCrypto1();

    delay(500);
}