/*
ESP32-2432S028R, 2.8 inch, 240*320, LCD TFT Module with Touch 
ARDUINO IDE 2.2.1 with this librarys:
TFT_eSPI by Bodmer ver 2.5.34
#XPT2046_Touchscreen by Stoffregen ver 1.4
NTPClient by Weinberg 3.2.1

https://en.wikipedia.org/wiki/Boids
https://wiki.edu.vn/wiki29/2021/11/17/beflockung-verhalten-wikipedia/
write update_fish() with 3 laws:
separation: steer to avoid crowding local flockmates
alignment: steer towards the average heading of local flockmates
cohesion: steer to move towards the average position (center of mass) of local flockmates
*/

#define WITH_TIME
#include <TFT_eSPI.h>
#include <SPI.h>
#include <math.h>
#include <time.h>

#ifdef WITH_TIME
#include <WiFi.h>
#include <NTPClient.h>
const char* ssid = "xxxxxxxxxx";         // Replace with your
const char* password = "xxxxxxx";  // WiFi credentials
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
int wifiTimeOutCounter = 0;
#endif  //WITH_TIME

#define MAX_Y 240
#define MAX_X 320
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object
uint16_t palette[16] = { TFT_GREENYELLOW, TFT_NAVY, TFT_ORANGE, TFT_DARKCYAN, TFT_MAROON,
                         TFT_PURPLE, TFT_PINK, TFT_LIGHTGREY, TFT_YELLOW, TFT_BLUE,
                         TFT_GREEN, TFT_CYAN, TFT_RED, TFT_MAGENTA, TFT_BLUE, TFT_WHITE };

// simutation values
#define MAX_SPEED 2.0
#define NUM_FLOCKS 200
#define SEPARATION_RADIUS 10.0
#define ALIGNMENT_RADIUS 15.0
#define COHESION_RADIUS 20.0
#define COHESION_FACTOR 0.02
#define ALIGNMENT_FACTOR 0.01
#define SEPARATION_FACTOR 0.05

typedef struct {
  float pos_x, pos_y, velocity_x, velocity_y;
  uint16_t color;
} FLOCK;
FLOCK flocks[NUM_FLOCKS];

#ifdef WITH_TIME
// it's missing in the Standard-Libary
String getFormattedDate() {
#define LEAP_YEAR(Y) ((Y > 0) && !(Y % 4) && ((Y % 100) || !(Y % 400)))
  unsigned long rawTime = timeClient.getEpochTime() / 86400L;
  unsigned long days = 0, year = 1970;
  uint8_t month;
  static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  while ((days += (LEAP_YEAR(year) ? 366 : 365)) <= rawTime) year++;
  rawTime -= days - (LEAP_YEAR(year) ? 366 : 365);
  for (month = 0; month < 12; month++) {
    uint8_t monthLength;
    if (month == 1) monthLength = LEAP_YEAR(year) ? 29 : 28;
    else monthLength = monthDays[month];
    if (rawTime < monthLength) break;
    rawTime -= monthLength;
  }
  String monthStr = ++month < 10 ? "0" + String(month) : String(month);
  String dayStr = ++rawTime < 10 ? "0" + String(rawTime) : String(rawTime);
  return String(dayStr) + "-" + monthStr + "-" + year;
}
#endif  //WITH_TIME

void initFlock(FLOCK flocks[], int num_flocks) {
  for (int i = 0; i < num_flocks; i++) {
    flocks[i].pos_x = rand() % MAX_X;
    flocks[i].pos_y = rand() % MAX_Y;
    flocks[i].velocity_x = ((float)rand() / RAND_MAX) * 2.0 - 1.0;
    flocks[i].velocity_y = ((float)rand() / RAND_MAX) * 2.0 - 1.0;
    flocks[i].color = random(0x10);  // Returns color 0 - 0x0F (i.e. 0-15)
  }
}

// Calculate distance between two flocks
float calculateDistance(FLOCK a, FLOCK b) {
  return sqrt((a.pos_x - b.pos_x) * (a.pos_x - b.pos_x) + (a.pos_y - b.pos_y) * (a.pos_y - b.pos_y));
}

// Update flock behavior
void updateFlockBehavior(FLOCK flocks[], int num_flocks) {
  for (int i = 0; i < num_flocks; i++) {
    float separationX = 0.0, separationY = 0.0;
    float alignmentX = 0.0, alignmentY = 0.0;
    float cohesionX = 0.0, cohesionY = 0.0;
    int separationCount = 0, alignmentCount = 0, cohesionCount = 0;

    for (int j = 0; j < num_flocks; j++) {
      if (i != j) {
        float distance = calculateDistance(flocks[i], flocks[j]);
        if (distance < SEPARATION_RADIUS) {
          separationX -= (flocks[j].pos_x - flocks[i].pos_x);
          separationY -= (flocks[j].pos_y - flocks[i].pos_y);
          separationCount++;
        }
        if (distance < ALIGNMENT_RADIUS) {
          alignmentX += flocks[j].velocity_x;
          alignmentY += flocks[j].velocity_y;
          alignmentCount++;
        }
        if (distance < COHESION_RADIUS) {
          cohesionX += flocks[j].pos_x;
          cohesionY += flocks[j].pos_y;
          cohesionCount++;
        }
      }
    }
    if (separationCount > 0) {
      separationX /= separationCount;
      separationY /= separationCount;
    }
    if (alignmentCount > 0) {
      alignmentX /= alignmentCount;
      alignmentY /= alignmentCount;
    }
    if (cohesionCount > 0) {
      cohesionX /= cohesionCount;
      cohesionY /= cohesionCount;
      cohesionX = (cohesionX - flocks[i].pos_x) / 100.0;
      cohesionY = (cohesionY - flocks[i].pos_y) / 100.0;
    }
    float totalForceX = separationX + alignmentX + cohesionX;
    float totalForceY = separationY + alignmentY + cohesionY;

    // Calculate speed
    float speed = sqrt(totalForceX * totalForceX + totalForceY * totalForceY);

    // Normalize velocity only if necessary
    if (speed > MAX_SPEED) {
      flocks[i].velocity_x = (totalForceX / speed) * MAX_SPEED;
      flocks[i].velocity_y = (totalForceY / speed) * MAX_SPEED;
    } else {
      flocks[i].velocity_x = totalForceX;
      flocks[i].velocity_y = totalForceY;
    }

    // Update position
    flocks[i].pos_x += flocks[i].velocity_x;
    flocks[i].pos_y += flocks[i].velocity_y;
    // Boundaries
    if (flocks[i].pos_x < -30) flocks[i].pos_x = MAX_X + 20;
    if (flocks[i].pos_x > MAX_X + 20) flocks[i].pos_x = -30;
    if (flocks[i].pos_y < -30) flocks[i].pos_y = MAX_Y + 20;
    if (flocks[i].pos_y > MAX_Y + 20) flocks[i].pos_y = -30;
  }
}

// Print fish
void printFish(FLOCK flocks[], int num_flocks) {
  for (int i = 0; i < num_flocks; i++)
    for (int16_t dx = -1; dx <= 1; dx++)
      for (int16_t dy = -1; dy <= 1; dy++)
        spr.drawPixel(flocks[i].pos_x + dx, flocks[i].pos_y + dy, flocks[i].color);
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Booting...");
  // prind debugg infos an serial
  uint32_t chipId = 0;
  for (int i = 0; i < 17; i = i + 8) chipId |= ((ESP.getEfuseMac() >> (40 - i)) & 0xff) << i;
  Serial.printf("ESP32 Chip model = %s Rev %d\n", ESP.getChipModel(), ESP.getChipRevision());
  Serial.printf("This chip has %d cores\n", ESP.getChipCores());
  Serial.print("Chip ID: ");
  Serial.println(chipId);

#ifdef WITH_TIME
  Serial.print("Connecting to ");
  Serial.print(ssid);
  Serial.print(" ");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifiTimeOutCounter++;
    if (wifiTimeOutCounter >= 60) ESP.restart();
  }
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  // Print the MAC address
  uint8_t mac[6];
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  for (int i = 0; i < 6; i++) {
    Serial.print(mac[i], HEX);
    if (i < 5) Serial.print(":");
  }
  Serial.println("");
  // Convert MAC address to unique identifier (UID)
  uint64_t uid = ((uint64_t)mac[0] << 40) | ((uint64_t)mac[1] << 32) | ((uint64_t)mac[2] << 24) | ((uint64_t)mac[3] << 16) | ((uint64_t)mac[4] << 8) | ((uint64_t)mac[5]);
  Serial.print("MAC unique identifier: ");
  Serial.println(uid);
  timeClient.begin();
  timeClient.setTimeOffset(-3600 * 5);
#endif  //WITH_TIME

  // ttf init
  tft.init();
  spr.setColorDepth(4);
  spr.createSprite(MAX_X, MAX_Y);
  tft.setRotation(1);

  // init flock
  srand(time(NULL));
  initFlock(flocks, NUM_FLOCKS);
}

void loop() {
  // Fill the sprite with color 0
  spr.fillSprite(0);

  updateFlockBehavior(flocks, NUM_FLOCKS);
  printFish(flocks, NUM_FLOCKS);

#ifdef WITH_TIME
  spr.setTextColor(3);
  timeClient.update();
  String strtmp = timeClient.getFormattedTime() + "  " + getFormattedDate();
  if (timeClient.isTimeSet()) {
    spr.drawString(timeClient.getFormattedTime(), 70, 80, 6);
    spr.drawString(getFormattedDate(), 40, 130, 6);
  }
#endif  //WITH_TIME

  // print sprite
  spr.pushSprite(0, 0);
}
