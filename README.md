# ESP32 RFID-RC522 Test Sketch

This project is a test sketch for using the RFID-RC522 module with an ESP32-S (38-pin version). Maybe this repo will help someone... idk

## Hardware Configuration

### ESP32-S Dev Board Pinout

```text
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
```
  Refence: https://www.espboards.dev/esp32/nodemcu-32s/


### Pin Mapping

| RC522 Pin | ESP32 Pin | Description |
|-----------|-----------|-------------|
| SDA (SS)  | GPIO 21   | SPI Slave Select |
| SCK       | GPIO 18   | SPI Clock |
| MOSI      | GPIO 23   | SPI Master Out Slave In |
| MISO      | GPIO 19   | SPI Master In Slave Out |
| IRQ       | GPIO 16   | Interrupt Request |
| GND       | GND       | Ground |
| RST       | GPIO 22   | Reset |
| 3.3V      | 3V3       | Power |

## Software Dependencies

This sketch requires the following Arduino libraries:
- `MFRC522` by GithubCommunity
Download it from arduino ide. I don't recommend other IDEs

## Usage

1. Open `sketch_may11a.ino` in the Arduino IDE.
2. Select your ESP32 board (e.g., "NodeMCU-32S").
3. Install the `MFRC522` library.
4. Upload the sketch.
5. Open the Serial Monitor (115200 baud).
6. Scan an RFID tag to see its UID and type.
