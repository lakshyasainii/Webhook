#include <DHT.h>
#include <WiFiNINA.h>

char ssid[] = "Lakshya's iPhone"; // my Personal hotspot name
char pass[] = "987654321"; // my Personal hotspot passcode

char server[] = "api.thingspeak.com";
String apiKey = "Y4DFOLMEV5C95AYD"; // put your api key here

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WiFiClient client;
void setup() {
  Serial.begin(9600);// 9600 is bits per second
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print("Searching and connecting to wifi");
    WiFi.begin(ssid, pass);
    delay(20000); 
  }

  Serial.println("WiFi Connected");
  dht.begin();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Failed...");
    delay(5000);
    return;
  }

  String data = "api_key=" + apiKey;
  data += "&field1=" + String(temperature);
  data += "&field2=" + String(humidity);

  if (client.connect(server, 80)) {
    client.print("POST /update HTTP/1.1\r\n");
    client.print("Host: " + String(server) + "\r\n");
    client.print("Content-Type: application/x-www-form-urlencoded\r\n");
    client.print("Content-Length: " + String(data.length()) + "\r\n");
    client.print("\r\n");
    client.print(data);
    
    Serial.println("Data sent...");
  } else {
    Serial.println("Connection failed!");
  }

  while (client.connected()) 
  {
    if (client.available()) 
    {
      char word = client.read();
      Serial.print(word);
    }
  }
  client.stop();
  delay(10000); 
}