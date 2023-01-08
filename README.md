# ARDUINO IDE 2.0.2 mit ESP32-2432S028R
ESP32 with 2.8inch 240*320 LCD TFT Module and Touch 

## General Information
https://www.aliexpress.com/item/1005004502250619.html <br>
https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/ <br>
Info Files "2.8inch_ESP32-2432S028R.zip" from download link: "http://www.jczn1688.com/zlxz" 
with passwort: "jczn1688" <br>

## How to install on Linux
`wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.0.2_Linux_64bit.AppImage` <br>
`mv arduino-ide_2.0.2_Linux_64bit.AppImage ~/Schreibtisch` <br>
`chmod u+x ~/Schreibtisch/arduino-ide_2.0.2_Linux_64bit.AppImage` <br>
Starten im Terminal im hintergrund mit <br>`~/Schreibtisch/arduino-ide_2.0.2_Linux_64bit.AppImage & disown` <br>
Settings in Arduino IDE:<br>
Menü -> File -> Preferences -> Language -> English <br>
Menü -> File -> Preferences -> Show verbose output during compile und upload -> check <br>
Menü -> File -> Preferences -> Compiler warnings -> Default <br>
Menü -> File -> Preferences -> Additional URLs -> https://<!--This is a comment-->dl.espressif.com/dl/<!--This is, too-->package_esp32_index.json <br>
Menü -> File -> Preferences -> OK <br>
Libary Manager -> TFT_eSPI -> Suchen und Installieren  TFT_eSPI by Bodmer <br>
Libary Manager -> TFT_eSPI -> Suchen und Installieren  LovyanGFX by lovyan03 <br>
Bords Manager -> ESP32 suchen und esp32 by Espressif Sytems instalieren <br>
Tools -> Board -> esp32 -> ESP32_DEV_Module auswählen <br>
Tools -> Port -> /dev/ttyACM0 auswählen -> OK <br>
-Upload Speed: “921600” (Win) <br>
-CPU Frequency : “240MHz (WiFi/BT)” <br>
-Flash Frequency : “80MHz” <br>
-Flash Mode : “QIO” or “DIO” <br>
-Flash Size : “4MB (32Mb)” <br>
-Partition Scheme : “Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)” <br>
-Core Degug Level : “Verbose” <br>
-PSRAM : “Disabled” <br>

## Prepear TFT_eSPI Libary
Copy this in ~/Arduino/libraries/TFT_eSPI/User_Setup.h <br>
```
#define ILI9341_2_DRIVER    
#define TFT_WIDTH  240
#define TFT_HEIGHT 320
#define TFT_BL   21           
#define TFT_BACKLIGHT_ON HIGH  
#define TFT_MOSI 13 
#define TFT_SCLK 14
#define TFT_CS   15  
#define TFT_DC   2  
#define TFT_RST  12  
#define TFT_BL   21 
#define TOUCH_CS 33  
#define LOAD_GLCD   
#define LOAD_FONT2  
#define LOAD_FONT4  
#define LOAD_FONT6  
#define LOAD_FONT7  
#define LOAD_FONT8  
#define LOAD_GFXFF  
#define SMOOTH_FONT
#define SPI_FREQUENCY  55000000 
#define SPI_READ_FREQUENCY  20000000
#define SPI_TOUCH_FREQUENCY  2500000
```
## Load first Example
```
wget https://macsbug.files.wordpress.com/2022/08/life_game_esp32_2432s028.zip_-1.pdf -O lifegame.zip && unzip  lifegame.zip -d ~/Arduino/ && rm  lifegame.zip && rm -rf ~/Arduino/__MACOSX/
```  
Menü -> File -> Open -> Life_Game_ESP32_2432S028 -> Life_Game_ESP32_2432S028.ino <br>
ZEILE 13 von lgfx_ESP32_2432S028.h: SPI2_HOST in HSPI_HOST ändern <br>
Compile und Upload -> BINGO <br>
