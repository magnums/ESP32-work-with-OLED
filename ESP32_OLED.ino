#include <WiFi.h>
#include <time.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET    -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const char* ssid = "PhayouneWIFI";
const char* password = "PhayounE2014";

const char* ntpServer1 = "pool.ntp.org";
const char* ntpServer2 = "time.nist.gov";
const long gmtOffset_sec = 25200; // UTC+7
const int daylightOffset_sec = 0; // No daylight saving time in UTC+7

// English day names
const char* en_dayNames[] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

// Thai day names
const char* th_dayNames[] = {"อาทิตย์", "จันทร์", "อังคาร", "พุธ", "พฤหัสบดี", "ศุกร์", "เสาร์"};

// Thai month names
const char* th_monthNames[] = {"มกราคม", "กุมภาพันธ์", "มีนาคม", "เมษายน", "พฤษภาคม", "มิถุนายน", "กรกฎาคม", "สิงหาคม", "กันยายน", "ตุลาคม", "พฤศจิกายน", "ธันวาคม"};


void printLocalTime() {
  
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("No time available (yet)");
    return;
  }

  // Calculate day index (0 for Sunday, 1 for Monday, ..., 6 for Saturday)
  int dayIndex = (timeinfo.tm_wday+1) % 7;

  // Print Thai date and time on Serial monitor
  Serial.print("วัน");
  Serial.print(th_dayNames[timeinfo.tm_wday]);
  Serial.print(", ");
  Serial.print(timeinfo.tm_mday);
  Serial.print(" ");
  Serial.print(th_monthNames[timeinfo.tm_mon]);
  Serial.print(" ");
  Serial.print(timeinfo.tm_year + 1900 + 543);
  Serial.print(" ");
  Serial.print(timeinfo.tm_hour);
  Serial.print(":");
  if (timeinfo.tm_min < 10) {
    Serial.print("0");
  }
  Serial.print(timeinfo.tm_min);
  Serial.print(":");
  if (timeinfo.tm_sec < 10) {
    Serial.print("0");
  }
  Serial.println(timeinfo.tm_sec);

  // Display English date and time on OLED
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  //display.print("Day: ");
  display.print(en_dayNames[dayIndex]); // Display day name
  display.setCursor(0, 25);
  //display.print("Date: ");
  display.print(timeinfo.tm_mday);
  display.print("/");
  display.print(timeinfo.tm_mon+1);
  display.print("/");
  display.print(timeinfo.tm_year + 1900+543);
  display.setCursor(0, 50);
  //display.print("Time: ");
  display.print(timeinfo.tm_hour);
  display.print(":");
  if (timeinfo.tm_min < 10) {
    display.print("0");
  }
  display.print(timeinfo.tm_min);
  display.print(":");
  if (timeinfo.tm_sec < 10) {
    display.print("0");
  }
  display.println(timeinfo.tm_sec);

  display.display();
}


void setup() {
  Serial.begin(115200);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  // Set NTP server
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);

  // Connect to WiFi
  Serial.printf("Connecting to %s ", ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println(" CONNECTED");
  // Print IP address
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  printLocalTime(); // Display current time on OLED
  delay(1000);      // Update every second
}
