/*
2.8 inch 240*320 LCD TFT Module With Touch 
TFT_eSPI by Bodmer and ARDUINO IDE 2.0.2 with ESP32-2432S028

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

#ifdef WITH_TIME
#include <WiFi.h>
#include <NTPClient.h>
const char* ssid = "xxxxxxxxxx";         // Replace with your
const char* password = "xxxxxxxxx";  // WiFi credentials
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);
int wifiTimeOutCounter = 0;
#endif  //WITH_TIME

uint32_t chipId = 0;

#define MAX_Y 240
#define MAX_X 320
TFT_eSPI tft = TFT_eSPI();
TFT_eSprite spr = TFT_eSprite(&tft);  // Declare Sprite object "spr" with pointer to "tft" object
uint16_t palette[16] = { TFT_GREENYELLOW, TFT_NAVY, TFT_ORANGE, TFT_DARKCYAN, TFT_MAROON,
                         TFT_PURPLE, TFT_PINK, TFT_LIGHTGREY, TFT_YELLOW, TFT_BLUE,
                         TFT_GREEN, TFT_CYAN, TFT_RED, TFT_MAGENTA, TFT_BLUE, TFT_WHITE };

// simutation values
#define MAX_FISH 100
#define NEIGHBOR_DISTANCE 10
#define RANDOM_FACTOR 1.5
#define SEPARATION_FACTOR 1
#define ALIGNMENT_FACTOR .5
#define COHESION_FACTOR .9
#define OLDSPEED_FACTOR .1
#define ENDSPEED_FACTOR 2

struct fish {
  float_t x;  // pos
  float_t y;
  float_t dx;  //direction
  float_t dy;
  uint16_t color;
};
struct fish f[MAX_FISH];

#ifdef WITH_TIME
// it's missing in the Standard-Libary
String getFormattedDate() {
#define LEAP_YEAR(Y) ((Y > 0) && !(Y % 4) && ((Y % 100) || !(Y % 400)))
  unsigned long rawTime = timeClient.getEpochTime() / 86400L;
  unsigned long days = 0, year = 1970;
  uint8_t month;
  static const uint8_t monthDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
  while ((days += (LEAP_YEAR(year) ? 366 : 365)) <= rawTime)
    year++;
  rawTime -= days - (LEAP_YEAR(year) ? 366 : 365);
  days = 0;
  for (month = 0; month < 12; month++) {
    uint8_t monthLength;
    if (month == 1) {
      monthLength = LEAP_YEAR(year) ? 29 : 28;
    } else {
      monthLength = monthDays[month];
    }
    if (rawTime < monthLength) break;
    rawTime -= monthLength;
  }
  String monthStr = ++month < 10 ? "0" + String(month) : String(month);
  String dayStr = ++rawTime < 10 ? "0" + String(rawTime) : String(rawTime);
  //return String(year) + "-" + monthStr + "-" + dayStr; //for the retarded
  return String(dayStr) + "-" + monthStr + "-" + year;
}
#endif  //WITH_TIME

// Function to calculate distance from direction and speed vector
inline float_t calcDistance(float_t dx, float_t dy) {
  return (sqrt(dx * dx + dy * dy));  // Calculate distance
}

// Function to calculate angle in degress from direction and speed vector
inline float_t calcAngle(float_t dx, float_t dy) {
  return (atan2(dy, dx) * 180.0 / M_PI);  // Calculate angle in degrees
}

// Function to calculate direction and speed vector X from angle in degrees and distance
inline float_t calcVectorX(float_t angle, float_t distance) {
  float_t rad = angle * M_PI / 180.0;  // Convert angle to radians
  return (cos(rad) * distance);        // Calculate x component of vector
}

// Function to calculate direction and speed vector Y from angle in degrees and distance
inline float_t calcVectorY(float_t angle, float_t distance) {
  float_t rad = angle * M_PI / 180.0;  // Convert angle to radians
  return (sin(rad) * distance);        // Calculate y component of vector
}
inline float_t rand_float() {
  // Random numbers between -1 and +1 as float
  return (random(-32768, 32767) / 32767.0);
}

void setup(void) {
  Serial.begin(115200);
  while (!Serial) delay(10);
  Serial.println("Booting...");

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

  tft.init();
  spr.setColorDepth(4);
  spr.createSprite(MAX_X, MAX_Y);
  tft.setRotation(1);

  // create random pos
  uint16_t i = MAX_FISH;
  while (i--) {
    f[i].color = random(0x10);  // Returns color 0 - 0x0F (i.e. 0-15)
    f[i].x = random(MAX_X);
    f[i].y = random(MAX_Y);
    f[i].dx = 0;
    f[i].dy = 0;
  };
}


void loop() {
  spr.fillSprite(0);  // Fill the whole sprite with color 0 (Sprite is in memory so not visible yet)

  // for every fish
  for (int i = 0; i < MAX_FISH; i++) {
    float_t separation_dx = 0;
    float_t separation_dy = 0;
    float_t alignment_dx = 0;
    float_t alignment_dy = 0;
    float_t cohesion_dx = 0;
    float_t cohesion_dy = 0;
    uint16_t num_neighbors = 0;

    for (uint16_t j = 0; j < MAX_FISH; j++) {
      if (i == j) continue;  // keine Berechnung für den eigenen Fisch
      float_t distx = f[j].x - f[i].x;
      float_t disty = f[j].y - f[i].y;
      if (distx < 2 && distx > -2 && disty < 2 && disty > -2) {
        distx = 10 * rand_float();
        disty = 10 * rand_float();
      }
      float_t dist = calcDistance(distx, disty);

      if (dist >= 0 && dist < NEIGHBOR_DISTANCE) {
        // Update the seperation vector
        separation_dx -= (distx / dist);
        separation_dy -= (disty / dist);
      }
      if (dist > 0 && dist < NEIGHBOR_DISTANCE) {
        // Update the alignment vector
        alignment_dx += f[j].dx;
        alignment_dy += f[j].dy;
      }
      if (dist > 0 && dist < NEIGHBOR_DISTANCE) {
        // Update the cohesion vector
        cohesion_dx += f[j].x;
        cohesion_dy += f[j].y;
        num_neighbors++;
      }
    }
    if (num_neighbors > 0) {
      cohesion_dx -= f[i].x;
      cohesion_dy -= f[i].y;
    }

    // update direction and speed vector
    float_t oldAngle = calcAngle(f[i].dx, f[i].dy);
    f[i].dx = calcVectorX(oldAngle, OLDSPEED_FACTOR);
    f[i].dy = calcVectorY(oldAngle, OLDSPEED_FACTOR);
    float_t separationAngle = calcAngle(separation_dx, separation_dy);
    f[i].dx += calcVectorX(separationAngle, SEPARATION_FACTOR);
    f[i].dy += calcVectorY(separationAngle, SEPARATION_FACTOR);
    float_t alignmentAngle = calcAngle(alignment_dx, alignment_dy);
    f[i].dx += calcVectorX(alignmentAngle, ALIGNMENT_FACTOR);
    f[i].dy += calcVectorY(alignmentAngle, ALIGNMENT_FACTOR);
    float_t cohesionAngle = calcAngle(cohesion_dx, cohesion_dy);
    f[i].dx += calcVectorX(cohesionAngle, COHESION_FACTOR);
    f[i].dy += calcVectorY(cohesionAngle, COHESION_FACTOR);
    f[i].dx += (RANDOM_FACTOR * rand_float());  // Random x and y coordinate
    f[i].dy += (RANDOM_FACTOR * rand_float());  // Random x and y coordinate
    oldAngle = calcAngle(f[i].dx, f[i].dy);
    f[i].dx = calcVectorX(oldAngle, ENDSPEED_FACTOR);
    f[i].dy = calcVectorY(oldAngle, ENDSPEED_FACTOR);

    // Update the position of the fish based on its direction and speed
    f[i].x += f[i].dx;
    f[i].y += f[i].dy;

    // ceck boundary same side
    if (f[i].x <= 0) f[i].x = MAX_X - 5;
    if (f[i].x >= MAX_X - 1) f[i].x = 5;
    if (f[i].y <= 0) f[i].y = MAX_Y - 5;
    if (f[i].y >= MAX_Y - 1) f[i].y = 5;

    /* // ceck boundary other side
    if (f[i].x <= 0) f[i].x = 5;
    if (f[i].x >= MAX_X - 1) f[i].x = MAX_X - 5;
    if (f[i].y <= 0) f[i].y = 5;
    if (f[i].y >= MAX_Y - 1) f[i].y = MAX_X - 5;  */

    // print fish
    for (int16_t dx = -2; dx <= 2; dx++)
      for (int16_t dy = -2; dy <= 2; dy++)
        spr.drawPixel(f[i].x + dx, f[i].y + dy, f[i].color);
  }
#ifdef WITH_TIME
  spr.setTextColor(3);
  timeClient.update();
  String strtmp = timeClient.getFormattedTime() + "  " + getFormattedDate();
  if (timeClient.isTimeSet()) {
    spr.drawString(timeClient.getFormattedTime(), 70, 80, 6);
    spr.drawString(getFormattedDate(), 40, 130, 6);
  }
#endif  //WITH_TIME

  spr.pushSprite(0, 0);
}
