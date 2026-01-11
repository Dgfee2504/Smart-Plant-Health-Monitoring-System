/*THIS CODE IS AFTER THE FLASK SERVER DETERMINES THE INFO AND NOW DECIDES WHICH COLOUR NEED TO BE PROJECTED*/
#include <WiFi.h>
#include <HTTPClient.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "SNUC";
const char* password = "snu12345";

// Flask server endpoints
const char* serverStatusURL = "http://10.31.25.157:5000/status";
const char* serverDHTURL = "http://10.31.25.157:5000/dht";

// RGB LED Pins
#define RED_PIN   16
#define GREEN_PIN 17
#define BLUE_PIN  18

// DHT11 settings
#define DHTPIN 4         
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

unsigned long previousMillis = 0;
const long interval = 10000;  

void setRGBColor(int red, int green, int blue) {
  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}

void setup() {
  Serial.begin(115200);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  setRGBColor(0, 0, 0);  // Start with LED off

  dht.begin();

  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected! IP address: ");
  Serial.println(WiFi.localIP());
}

void sendDHTData() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  // Check if readings are valid
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverDHTURL);
    http.addHeader("Content-Type", "application/json");

    String postData = "{\"temperature\": " + String(temperature, 1) + ", \"humidity\": " + String(humidity, 1) + "}";
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
      Serial.println("DHT11 data sent: " + postData);
    } else {
      Serial.println("Error sending DHT11 data: " + String(httpResponseCode));
    }

    http.end();
  } else {
    Serial.println("WiFi not connected for DHT POST");
  }
}

void fetchAndUpdateStatus() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(serverStatusURL);
    int httpCode = http.GET();

    if (httpCode > 0) {
      String payload = http.getString();
      Serial.println("Plant Status: " + payload);

      if (payload == "Healthy plant (Green)" || payload == "Healthy plant (Dominant Green)") {
        setRGBColor(255, 0, 0);  // green
      } 
      else if (payload == "Undernourished plant (Yellow)" || payload == "Undernourished plant (Dominant Yellow)") {
        setRGBColor(255, 255, 0);  // yellow
      } 
      else if (payload == "Dead plant (Brown)" || payload == "Dead plant (Dominant Brown)") {
        setRGBColor(0, 255, 0);  //red 
      } 
      else {
        Serial.println("Unknown status received.");
        setRGBColor(255, 255, 255);
        delay(300);
        setRGBColor(0, 0, 0);
      }

    } else {
      Serial.println("Error in HTTP request: " + String(httpCode));
      setRGBColor(255, 165, 0);  // Orange for request error
    }

    http.end();
  } else {
    Serial.println("WiFi not connected");
    setRGBColor(255, 0, 0);  // Red for no WiFi
  }
}

void loop() {
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    sendDHTData();         // Send DHT data to Flask
    fetchAndUpdateStatus(); // Fetch and update plant status
  }
}