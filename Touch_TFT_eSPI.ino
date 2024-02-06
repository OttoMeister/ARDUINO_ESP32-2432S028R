// test touch

#include <SPI.h>
#include <TFT_eSPI.h>
TFT_eSPI tft = TFT_eSPI();

uint16_t trX = 0, trY = 0, tX = 0, tY = 0;
uint32_t chipId = 0;

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Booting...\n");

  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.drawLine(0, 0, 319, 0, 0x561F);
  tft.drawLine(0, 239, 319, 239, 0x561F);
  tft.drawLine(0, 0, 0, 239, 0x561F);
  tft.drawLine(319, 0, 319, 239, 0x561F);
  tft.setCursor(0, 100);
  for (int i = 0; i < 17; i = i + 8) chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  tft.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  tft.printf("This chip has %d cores\n", ESP.getChipCores());
  tft.print("Chip ID: ");
  tft.println(chipId);
  delay(100);  
}

void loop() {
  tft.getTouch(&tX, &tY);
  tft.getTouchRaw(&trX, &trY);

  Serial.printf("tX:%d,tY:%d,trX:%d,trY:%d\n", tX, tY, trX, trY);
  tft.setCursor(0, 0);
  tft.printf("tX=%d  \ntY=%d  \ntrX=%d  \ntrY=%d  \n", tX, tY, trX, trY);
  delay(100);  
}
