# Platformio ESP32S3 ESP32-2432S028R
ESP32 with 2.8inch 240*320 LCD TFT Module and Touch 

![Unbenannt](https://github.com/OttoMeister/ARDUINO_ESP32-2432S028R/assets/12480979/11216454-3f5b-4595-b2d6-9a5257635b39)

## General Information
`https://www.aliexpress.com/item/1005004502250619.html` <br>
`https://www.aliexpress.com/item/1005004961285750.html` <br>
`https://macsbug.wordpress.com/2022/08/17/esp32-2432s028/` <br>
`https://www.makerfabs.com/sunton-esp32-2-8-inch-tft-with-touch.html` <br>
<br>
Thanks to the efforts of these individuals and many others, programming on Suntown displays has become very easy. 
<br>
https://github.com/rzeldent/esp32-smartdisplay<br>
https://github.com/rzeldent/platformio-espressif32-sunton<br>
https://github.com/lvgl/lv_port_esp32<br>
https://github.com/platformio/platformio-core<br>
https://github.com/witnessmenow/ESP32-Cheap-Yellow-Display<br>

## Install PlatformIO on Linux (no IDE)
```
curl -fsSL -o get-platformio.py https://raw.githubusercontent.com/platformio/platformio-core-installer/master/get-platformio.py
python3 get-platformio.py
curl -fsSL https://raw.githubusercontent.com/platformio/platformio-core/develop/platformio/assets/system/99-platformio-udev.rules | sudo tee /etc/udev/rules.d/99-platformio-udev.rules
sudo service udev restart
export PATH=$PATH:$HOME/.local/bin
ln -s ~/.platformio/penv/bin/platformio ~/.local/bin/platformio
ln -s ~/.platformio/penv/bin/pio ~/.local/bin/pio
ln -s ~/.platformio/penv/bin/piodebuggdb ~/.local/bin/piodebuggdb
pio settings set enable_telemetry no
pio settings set check_platformio_interval 9999
```
## Compile with PlatformIO on Linux
```
git clone https://github.com/OttoMeister/ARDUINO_ESP32-2432S028R
cd ARDUINO_ESP32-2432S028R/
platformio run 
platformio run --upload-port  /dev/ttyUSB0 -t upload
platformio run --monitor-port /dev/ttyUSB0 -t monitor
```
## Install PlatformIO on Win11 (no IDE)
Windows Command Prompt <br>
Install Python:
```
winget install Python.Python.3.13
REM Restart Windows Command Prompt 
python -V
python.exe -m pip install --upgrade pip
```
Install Git:
```
winget install git.git
REM restart Windows Command Prompt 
git --version
```
Install PlatformIO:
```
pip install platformio
pio --version
pio settings set enable_telemetry no
pio settings set check_platformio_interval 9999
```
Install CP210x driver:
```
cd %USERPROFILE%\Desktop
curl -L --fail -o cp210x.zip https://www.silabs.com/documents/public/software/CP210x_Windows_Drivers.zip --silent
mkdir cp210x
tar -xf cp210x.zip -C cp210x
start /wait cp210x\CP210xVCPInstaller_x64.exe /S
```
Install CH340 (WCH) Chips
```
cd %USERPROFILE%\Desktop
curl -L --fail -o CH341SER.exe https://www.wch.cn/download/file?id=65  --silent
start /wait CH341SER.exe /S
```
## Compile with PlatformIO on Win11
```
cd %USERPROFILE%\Desktop
git clone https://github.com/OttoMeister/ARDUINO_ESP32-2432S028R
cd ARDUINO_ESP32-2432S028R/
platformio run 
platformio run --upload-port  COM1 -t upload
platformio run --monitor-port COM1 -t monitor
```



