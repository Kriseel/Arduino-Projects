/*
  github.com/Kriseel

  Weather forecast display using data from Meteorologisk Institutt.
  Wifi library to connect to wifi.
  Json to parse the data
  HTTPClient and NetworkClientSecure to connect to the API endpoints (HTTPS)
*/


#include "values.h" // Personal
#include <WiFi.h> // WiFi setup
#include <HTTPClient.h> // Http request 
#include <NetworkClientSecure.h> // Network client
#include <ArduinoJson.h> // JSON parsing
#include <Wire.h> // I2C
#include <Adafruit_GFX.h> // Display GFXs
#include <Adafruit_SSD1306.h> // Display libraries
#include <Fonts/WeatherIcon.h> // Weather icon font sourced from https://github.com/kaldoran/Adafruit-Weather-Icon and slightly modified

#define SCREEN_WIDTH 128 // Display width in pixels
#define SCREEN_HEIGHT 64 // Display height in pixels
#define OLED_RESET -1 // -1 sharing arduino reset pin (or specified in board core like ESP32)
#define SCREEN_ADDRESS 0x3D // check datasheet for display to find the address

// The endpoint url for the API
const char *endpointURL = "https://api.met.no/weatherapi/locationforecast/2.0/compact?lat=" LAT "&lon=" LONG;

// Hellenic Academic and Research Institutions CA certificate, the root certificate for the MET API service
// Valid until 13 Feb 2045 10:55:37 GMT
const char *rootCACertificate = R"string_literal(
-----BEGIN CERTIFICATE-----
MIIFpDCCA4ygAwIBAgIQOcqTHO9D88aOk8f0ZIk4fjANBgkqhkiG9w0BAQsFADBs
MQswCQYDVQQGEwJHUjE3MDUGA1UECgwuSGVsbGVuaWMgQWNhZGVtaWMgYW5kIFJl
c2VhcmNoIEluc3RpdHV0aW9ucyBDQTEkMCIGA1UEAwwbSEFSSUNBIFRMUyBSU0Eg
Um9vdCBDQSAyMDIxMB4XDTIxMDIxOTEwNTUzOFoXDTQ1MDIxMzEwNTUzN1owbDEL
MAkGA1UEBhMCR1IxNzA1BgNVBAoMLkhlbGxlbmljIEFjYWRlbWljIGFuZCBSZXNl
YXJjaCBJbnN0aXR1dGlvbnMgQ0ExJDAiBgNVBAMMG0hBUklDQSBUTFMgUlNBIFJv
b3QgQ0EgMjAyMTCCAiIwDQYJKoZIhvcNAQEBBQADggIPADCCAgoCggIBAIvC569l
mwVnlskNJLnQDmT8zuIkGCyEf3dRywQRNrhe7Wlxp57kJQmXZ8FHws+RFjZiPTgE
4VGC/6zStGndLuwRo0Xua2s7TL+MjaQenRG56Tj5eg4MmOIjHdFOY9TnuEFE+2uv
a9of08WRiFukiZLRgeaMOVig1mlDqa2YUlhu2wr7a89o+uOkXjpFc5gH6l8Cct4M
pbOfrqkdtx2z/IpZ525yZa31MJQjB/OCFks1mJxTuy/K5FrZx40d/JiZ+yykgmvw
Kh+OC19xXFyuQnspiYHLA6OZyoieC0AJQTPb5lh6/a6ZcMBaD9YThnEvdmn8kN3b
LW7R8pv1GmuebxWMevBLKKAiOIAkbDakO/IwkfN4E8/BPzWr8R0RI7VDIp4BkrcY
AuUR0YLbFQDMYTfBKnya4dC6s1BG7oKsnTH4+yPiAwBIcKMJJnkVU2DzOFytOOqB
AGMUuTNe3QvboEUHGjMJ+E20pwKmafTCWQWIZYVWrkvL4N48fS0ayOn7H6NhStYq
E613TBoYm5EPWNgGVMWX+Ko/IIqmhaZ39qb8HOLubpQzKoNQhArlT4b4UEV4AIHr
W2jjJo3Me1xR9BQsQL4aYB16cmEdH2MtiKrOokWQCPxrvrNQKlr9qEgYRtaQQJKQ
CoReaDH46+0N0x3GfZkYVVYnZS6NRcUk7M7jAgMBAAGjQjBAMA8GA1UdEwEB/wQF
MAMBAf8wHQYDVR0OBBYEFApII6ZgpJIKM+qTW8VX6iVNvRLuMA4GA1UdDwEB/wQE
AwIBhjANBgkqhkiG9w0BAQsFAAOCAgEAPpBIqm5iFSVmewzVjIuJndftTgfvnNAU
X15QvWiWkKQUEapobQk1OUAJ2vQJLDSle1mESSmXdMgHHkdt8s4cUCbjnj1AUz/3
f5Z2EMVGpdAgS1D0NTsY9FVqQRtHBmg8uwkIYtlfVUKqrFOFrJVWNlar5AWMxaja
H6NpvVMPxP/cyuN+8kyIhkdGGvMA9YCRotxDQpSbIPDRzbLrLFPCU3hKTwSUQZqP
JzLB5UkZv/HywouoCjkxKLR9YjYsTewfM7Z+d21+UPCfDtcRj88YxeMn/ibvBZ3P
zzfF0HvaO7AWhAw6k9a+F9sPPg4ZeAnHqQJyIkv3N3a6dcSFA1pj1bF1BcK5vZSt
jBWZp5N99sXzqnTPBIWUmAD04vnKJGW/4GKvyMX6ssmeVkjaef2WdhW+o45WxLM0
/L5H9MG0qPzVMIho7suuyWPEdr6sOBjhXlzPrjoiUevRi7PzKzMHVIf6tLITe7pT
BGIBnfHAT+7hOtSLIBD6Alfm78ELt5BGnBkpjNxvoEppaZS3JGWg/6w/zgH7IS79
aPib8qXPMThcFarmlwDB31qlpzmq6YR/PFGoOtmUW4y/Twhx5duoXNTSpv4Ao8YW
xw/ogM4cKGR0GQjTQuPOAF1/sdwTsOEFy9EgqoZ0njnnkf3/W9b3raYvAwtt41dU
63ZTGI0RmLo=
-----END CERTIFICATE-----
)string_literal";

String payload = "";

// Number of timeSeries entries we want to get (typically what we can fit on display)
const uint8_t timeSeriesLimit = 4;

// Struct for each weatherSlot for time, temp, etc. We can put these structs into an array to access for each time (0 would be the first one when fetching data (NOW))
struct WeatherSlot {
  String time;
  int8_t temp;
  String symbolCode;
};
WeatherSlot forecast[timeSeriesLimit];

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Setting up network client and http request to fetch data from API
String getData() {
  NetworkClientSecure *client = new NetworkClientSecure;
  if (client) {
    client->setCACert(rootCACertificate);
    {
      // Extra scoping block for HTTPClient so it is both created after and destroyed before NetworkClientSecure is
      HTTPClient https;

      Serial.println("[HTTPS] Begin...");
      if (https.begin(*client, endpointURL)) { // HTTPS
        // Set user agent
        https.setUserAgent(USER_AGENT);
        Serial.println("[HTTPS] GET...");

        // Start the connection and get https
        int httpCode = https.GET();

        // Check code for error
        if (httpCode > 0) {
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

          // Found and made connection -> get the payload body
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);
            
            // end https and delete client before returning
            https.end();
            delete client;
            
            // We successfully got the payload, return it
            return payload;
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
        }
        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
      }
      // End of extra scoping block
    }
    delete client;
  } else {
    Serial.println("Unable to create client");
  }

  return "ERROR";
}

uint8_t parseData(String payload) {
  JsonDocument doc;

  DeserializationError error = deserializeJson(doc, payload);

  // Check for parsing error
  if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.f_str());
    return 1;
  }

  // Fetch the values into structs
  for (uint8_t i = 0; i < timeSeriesLimit; i++) {
    forecast[i].time = doc["properties"]["timeseries"][i]["time"].as<String>();
    forecast[i].temp = doc["properties"]["timeseries"][i]["data"]["instant"]["details"]["air_temperature"];
    forecast[i].symbolCode = doc["properties"]["timeseries"][i]["data"]["next_1_hours"]["summary"]["symbol_code"].as<String>();
  }

  // Print to serial so we can check without display
  for (uint8_t i = 0; i < timeSeriesLimit; i++) {
    Serial.println();
    Serial.println(forecast[i].time);
    Serial.println(forecast[i].temp);
    Serial.println(forecast[i].symbolCode);
  }

  return 0;
}

String trimDateTime(String time) {
  int hour = time.substring(11, 13).toInt() + 2; // plus one for UTC + 2

  hour = hour % 24;

  if (hour < 10) {
    return "0" + String(hour) + ":00";
  } else {
    return String(hour) + ":00";
  }
}

String getSymbolFromCode(String symbolCode) {
  // Converting the symbol code into a symbol using a specific character based on custom font
  // Hard to follow and read, some values even though ends up in same result on a different line in order to have shorter lines
  // Sleet is same symbol as for rain. All showers variants is just the same as their main non-showers variant
  
  // Symbol overview
  // https://github.com/metno/weathericons/tree/main/weather

  if(symbolCode == "clearsky_day" || symbolCode == "clearsky_polartwilight") return "B";
  if(symbolCode == "clearsky_night") return "C";

  if(symbolCode == "cloudy") return "N";

  if(symbolCode == "fair_day" || symbolCode == "fair_polartwilight") return "(";
  if(symbolCode == "fair_night") return ")";

  if(symbolCode == "fog") return "L";

  if(symbolCode == "partlycloudy_day" || symbolCode == "partlycloudy_polartwilight") return "H";
  if(symbolCode == "partlycloudy_night") return "I";

  // Heavy variants - rain and sleet together, then thunder versions, then snow
  if(symbolCode == "heavyrain" || symbolCode == "heavyrainshowers_day" || symbolCode == "heavyrainshowers_polartwilight" || symbolCode == "heavyrainshowers_night") return "X";
  if(symbolCode == "heavysleet" || symbolCode == "heavysleetshowers_day" || symbolCode == "heavysleetshowers_polartwilight" || symbolCode == "heavysleetshowers_night") return "X";

  if(symbolCode == "heavyrainandthunder" || symbolCode == "heavyrainshowersandthunder_day" || symbolCode == "heavyrainshowersandthunder_polartwilight" || symbolCode == "heavyrainshowersandthunder_night") return "*";
  if(symbolCode == "heavysleetandthunder" || symbolCode == "heavysleetshowersandthunder_day" || symbolCode == "heavysleetshowersandthunder_polartwilight" || symbolCode == "heavysleetshowersandthunder_night") return "*";

  if(symbolCode == "heavysnow" || symbolCode == "heavysnowshowers_day" || symbolCode == "heavysnowshowers_night" || symbolCode == "heavysnowshowers_polartwilight") return "W";

  if(symbolCode == "heavysnowandthunder" || symbolCode == "heavysnowshowersandthunder_day" || symbolCode == "heavysnowshowersandthunder_night" || symbolCode == "heavysnowshowersandthunder_polartwilight") return "+";

  // Light variants
  if(symbolCode == "lightrain" || symbolCode == "lightrainshowers_day" || symbolCode == "lightrainshowers_polartwilight" || symbolCode == "lightrainshowers_night") return "Q";
  if(symbolCode == "lightsleet" || symbolCode == "lightsleetshowers_day" || symbolCode == "lightsleetshowers_polartwilight" || symbolCode == "lightsleetshowers_night") return "Q";

  if(symbolCode == "lightrainandthunder" || symbolCode == "lightrainshowersandthunder_day" || symbolCode == "lightrainshowersandthunder_polartwilight" || symbolCode == "lightrainshowersandthunder_night") return ",";
  if(symbolCode == "lightsleetandthunder" || symbolCode == "lightsleetshowersandthunder_day" || symbolCode == "lightsleetshowersandthunder_polartwilight" || symbolCode == "lightsleetshowersandthunder_night") return ",";

  if(symbolCode == "lightsnow" || symbolCode == "lightsnowshowers_day" || symbolCode == "lightsnowshowers_night" || symbolCode == "lightsnowshowers_polartwilight") return "U";

  if(symbolCode == "lightsnowandthunder" || symbolCode == "lightsnowshowersandthunder_day" || symbolCode == "lightsnowshowersandthunder_night" || symbolCode == "lightsnowshowersandthunder_polartwilight") return "-";

  // Normal variants
  if(symbolCode == "rain" || symbolCode == "rainshowers_day" || symbolCode == "rainshowers_polartwilight" || symbolCode == "rainshowers_night") return "R";
  if(symbolCode == "sleet" || symbolCode == "sleetshowers_day" || symbolCode == "sleetshowers_polartwilight" || symbolCode == "sleetshowers_night") return "R";

  if(symbolCode == "rainandthunder" || symbolCode == "rainshowersandthunder_day" || symbolCode == "rainshowersandthunder_polartwilight" || symbolCode == "rainshowersandthunder_night") return ".";
  if(symbolCode == "sleetandthunder" || symbolCode == "sleetshowersandthunder_day" || symbolCode == "sleetshowersandthunder_polartwilight" || symbolCode == "sleetshowersandthunder_night") return ".";

  if(symbolCode == "snow" || symbolCode == "snowshowers_day" || symbolCode == "snowshowers_night" || symbolCode == "snowshowers_polartwilight") return "V";

  if(symbolCode == "snowandthunder" || symbolCode == "snowshowersandthunder_day" || symbolCode == "snowshowersandthunder_night" || symbolCode == "snowshowersandthunder_polartwilight") return "/";
  
  // Default N (cloudy) if not found
  return "N";
}

void displayData() {
  // Make sure display is clear before redrawing and default font
  display.setFont();
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);

  const int rowHeight = 32;
  const int columnWidth = 60;
  const int lineSpacing = 10;

  // Draw data to on display
  for (uint8_t i = 0; i < timeSeriesLimit; i++) {
    // Set default font for the normal text
    display.setFont();

    int col = i % 2;
    int row = i / 2;

    int x = col * columnWidth;
    int y = (row == 1) ? (row * rowHeight + 4) : (row * rowHeight);

    display.setCursor(x, y);
    display.print(trimDateTime(forecast[i].time));

    display.setCursor(x, y + lineSpacing);
    display.print(forecast[i].temp); display.write(247); display.print(F("C"));

    display.setCursor(x + 33, y + (lineSpacing * 2));
    // Custom weather icon font (https://github.com/kaldoran/Adafruit-Weather-Icon with some modifications)
    display.setFont(&Weathericon);
    display.print(getSymbolFromCode(forecast[i].symbolCode));
  }

  display.display();
}

// General Arduino IDE setup to be run first when the program starts
void setup() {
  Serial.begin(115200);

  // Connect to WIFI
  Serial.print("Connecting to: ");
  Serial.println(WIFI_SSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println(".");
  }

  Serial.println("Wifi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("\nSetting up Adafruit display");
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed, looping forever, please reset"));
    for(;;);
  }

  // Clear display
  display.clearDisplay();

}

// Main loop, fetching data, parsing and putting what we care about on display (once every hour +/- 10 min)
void loop() {
  
  // Get request
  payload = getData();

  // Parse the JSON
  if (payload != "ERROR") {
    if (parseData(payload) == 0) {
      // Display
      displayData();
    }
  }
  
  // Wait 1 hour before asking again
  delay(60UL * 60UL * 1000UL);
}