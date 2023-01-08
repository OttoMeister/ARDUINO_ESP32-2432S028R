# ARDUINO IDE 2.0.2 mit ESP32-2432S028R
ESP32 with 2.8inch 240*320 LCD TFT Module and Touch 

## General Information
https://www.aliexpress.com/item/1005004502250619.html
https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/
Info Files "2.8inch_ESP32-2432S028R.zip" from download link:http://www.jczn1688.com/zlxz with passwort: "jczn1688"

## How to install on Linux



wget https://downloads.arduino.cc/arduino-ide/arduino-ide_2.0.2_Linux_64bit.AppImage

mv arduino-ide_2.0.2_Linux_64bit.AppImage ~/Schreibtisch

chmod u+x ~/Schreibtisch/arduino-ide_2.0.2_Linux_64bit.AppImage

-Starten im Terminal im hintergrund mit "~/Schreibtisch/arduino-ide_2.0.2_Linux_64bit.AppImage & disown"
#Einstellungen
Menü -> File -> Preferences -> Language -> English
Menü -> File -> Preferences -> Show verbose output during compile und upload -> check
Menü -> File -> Preferences -> Compiler warnings -> Default
Menü -> File -> Preferences -> Additional URLs -> https://dl.espressif.com/dl/package_esp32_index.json
Menü -> File -> Preferences -> OK
Libary Manager -> TFT_eSPI -> Suchen und Installieren  TFT_eSPI by Bodmer
Libary Manager -> TFT_eSPI -> Suchen und Installieren  LovyanGFX by lovyan03
Bords Manager -> ESP32 suchen und esp32 by Espressif Sytems instalieren
Tools -> Board -> esp32 -> ESP32_DEV_Module auswählen
Tools -> Port -> /dev/ttyACM0 auswählen -> OK
_ Upload Speed ​​: “460800” (Mac), “921600” (Win)
_ CPU Frequency : “240MHz (WiFi/BT)”
_ Flash Frequency : “80MHz”
_ Flash Mode : “QIO” or “DIO”
_ Flash Size : “4MB (32Mb)”
_ Partition Scheme : “Default 4MB with spiffs (1.2MB APP/1.5MB SPIFFS)”
_ Core Degug Level : “Verbose”
_ PSRAM : “Disabled”
#Beispiel laden
wget https://macsbug.files.wordpress.com/2022/08/life_game_esp32_2432s028.zip_-1.pdf -O lifegame.zip && unzip  lifegame.zip -d ~/Arduino/ && rm  lifegame.zip && rm -rf ~/Arduino/__MACOSX/
Menü -> File -> Open -> Life_Game_ESP32_2432S028 -> Life_Game_ESP32_2432S028.ino
ZEILE 13 von lgfx_ESP32_2432S028.h: SPI2_HOST in HSPI_HOST ändern
Compile und Upload -> BINGO
