// test touch setup
// Landscape - The screen's aspect ratio has a width greater than the height.
// X=0 (width) and Y=0 (hight) is on the upper left.

#define LGFX_USE_V1
#include <LovyanGFX.hpp>
#include "lgfx_ESP32_2432S028.h"
#define MAX_X 319
#define MAX_Y 239
uint16_t trX = 0, trY = 0, tX = 0, tY = 0, tZ = 0;
uint32_t chipId = 0;
static LGFX lcd;  // Create an instance of LGFX.
//static LGFX_Sprite sprite (&lcd; // Create an instance of LGFX_Sprite when using a sprite.

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Booting...\n");
  lcd.init();
  lcd.setRotation(1);
  lcd.fillScreen(TFT_BLACK);
  lcd.drawLine(0, 0, MAX_X, 0, 0x561F);
  lcd.drawLine(0, 0, 0, MAX_Y, 0x561F);
  lcd.drawLine(MAX_X, MAX_Y, 0, MAX_Y, 0x561F);
  lcd.drawLine(MAX_X, MAX_Y, MAX_X, 0, 0x561F);
  lcd.setCursor(0, 100);
  for (int i = 0; i < 17; i = i + 8) chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  lcd.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  lcd.printf("This chip has %d cores\n", ESP.getChipCores());
  lcd.print("Chip ID: ");
  lcd.println(chipId);
  delay(100);
}

void loop() {
  tZ = lcd.getTouch(&tX, &tY);
  if (tX < 1 || tX > MAX_X * 2) tX = 0; //Why I have to do that?
  if (tX > MAX_X && tX < MAX_Y * 2) tX = MAX_Y;
  if (tY < 1 || tY > MAX_Y * 2) tY = 0;
  if (tY > MAX_Y && tY < MAX_Y * 2) tY = MAX_Y;
  lcd.getTouchRaw(&trX, &trY);
  Serial.printf("tX:%d,tY:%d,trX:%d,trY:%d\n", tX, tY, trX, trY);
  lcd.setCursor(0, 0);
  lcd.printf("tX=%d  \ntY=%d  \ntZ=%d  \ntrX=%d  \ntrY=%d  ", tX, tY, tZ, trX, trY);
  delay(100);
}
