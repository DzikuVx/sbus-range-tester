# sbus-range-tester
ESP32 and GPS based device for testing the range of SBUS receivers

## Pin assignment

| Part      | On device     |   On ESP32    |
| ------    | ------        | --------      |
| GPS       | TX            | 13            |
| GPS       | RX            | 15            |
| Button L  | -             | 4             |
| Button R  | -             | 0             |
| OLED      | RST           | 16            |
| OLED      | SDA           | 22              |
| OLED      | SCL           | 21              |
| Receiver  | SBUS          | 14            |

Button pins are pulled up, buttons should conect to GND

## Useful info

* Requires [TinyGPS++](http://arduiniana.org/libraries/tinygpsplus/) library installed
* Button R long press resets the distance
* Button L long press resets Failsafe counter

## BOM

* [ESP32 TTGO LoRa32 v2](http://bit.ly/2BjG47d) or other [ESP32 OLED](http://bit.ly/2oonWzy). Other ESP32 board might require changing pin assignments
* 2x [Tactile buttons](http://bit.ly/2Kzawy3)
* [BN-180](http://bit.ly/2VqHuSF) or ther UBLOX GPS module
* [1S LiPo battery](https://bit.ly/3lfjgZ6)