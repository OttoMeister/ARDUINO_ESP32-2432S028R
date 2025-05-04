# Platformio ESP32S3 ESP32-2432S028R
ESP32 with 2.8inch 240*320 LCD TFT Module and Touch 

![Unbenannt](https://github.com/OttoMeister/ARDUINO_ESP32-2432S028R/assets/12480979/11216454-3f5b-4595-b2d6-9a5257635b39)
(ESP32_2432S028R_Display5.ino)
## General Information
`https://www.aliexpress.com/item/1005004502250619.html `<br>
`https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/` <br>

## How to install ARDUINO IDE 2.0.3 with ESP32 support on Linux
erase all old traces, then download, install and start it
```
rm -rf ~/Schreibtisch/arduino-ide* ~/.arduinoIDE ~/.arduino15 ~/.config/arduino* ~/.config/Arduino* ~/Arduino*
wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.0.3_Linux_64bit.AppImage
mv arduino-ide_2.0.3_Linux_64bit.AppImage ~/Schreibtisch
chmod u+x ~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage
~/Schreibtisch/arduino-ide_2.0.3_Linux_64bit.AppImage & disown
```

## Install Arduino IDE 2.3.0 on Windows 
Download from here:
```
https://downloads.arduino.cc/arduino-ide/arduino-ide_2.3.0_Windows_64bit.exe
```
Start the installer, klick yes to everything.

Settings in Arduino IDE:<br>
Menü -> File -> Preferences -> Language -> English <br>
Menü -> File -> Preferences -> Show verbose output during compile und upload -> check <br>
Menü -> File -> Preferences -> Compiler warnings -> Default <br>
Menü -> File -> Preferences -> Additional URLs -> `https://dl.espressif.com/dl/package_esp32_index.json` <br>
Menü -> File -> Preferences -> OK <br>
Libary Manager -> TFT_eSPI -> search and install: TFT_eSPI by Bodmer ver 2.5.34 <br>
Libary Manager -> TFT_eSPI -> search and install: NTPClient by F.Weinberg ver 3.2.1 <br>
Libary Manager -> TFT_eSPI -> search and install: HTTPClient by A.McEwen ver 2.2.0 <br>
Libary Manager -> TFT_eSPI -> search and install: ArduinoJson.h by B.Blanchon ver 7.0.3 <br>
Bords Manager -> ESP32 search and install esp32 by Espressif Sytems instalieren <br>
Select Board and Port -> ESP32 Dev Module
-CPU Frequency : “240MHz (WiFi/BT)” <br>
-Core Degug Level : “Verbose” <br>
-Flash Frequency : “80MHz” <br>
-Flash Mode : “QIO” or “DIO” <br>
-Flash Size : “4MB (32Mb)” <br>
-Partition Scheme : “Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)” <br>
-PSRAM : “Disabled” <br>
-Upload Speed: “921600”  <br>

## Prepear LovyanGFX Libary
Copy lgfx_ESP32_2432S028.h to your local dirctrory and include `#include "lgfx_ESP32_2432S028.h"`
to your source code. Right now, there is only one example using this libary

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
//Touch Screen: ?????????ß
#define XPT2046_IRQ 36
#define XPT2046_MOSI 32
#define XPT2046_MISO 39
#define XPT2046_CLK 25
#define XPT2046_CS 33
#define SPI_TOUCH_FREQUENCY  2500000
```
## Load first Example under Linux
```
wget https://macsbug.files.wordpress.com/2022/08/life_game_esp32_2432s028.zip_-1.pdf -O tmp.zip 
unzip -o tmp.zip Life_Game_ESP32_2432S028/lgfx_ESP32_2432S028.h Life_Game_ESP32_2432S028/Life_Game_ESP32_2432S028.ino -d ~/Arduino/ 
rm tmp.zip
find ~/Arduino/* -type f -iname 'lgfx_ESP32_2432S028.h' -exec sed -i 's/SPI2_HOST/HSPI_HOST/g' '{}' \;
find ~/Arduino/Life_Game*
```  
Menü -> File -> Open -> Life_Game_ESP32_2432S028 -> Life_Game_ESP32_2432S028.ino <br>
Compile und Upload -> BINGO <br>

## To do

