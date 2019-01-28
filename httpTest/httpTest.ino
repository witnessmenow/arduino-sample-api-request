#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

#include <ArduinoJson.h>

//------- Replace the following! ------
char ssid[] = "SSID";       // your network SSID (name)
char password[] = "PASS";  // your network key

WiFiClientSecure client;

#define TEST_HOST "www.tindie.com"
#define TEST_HOST_FINGERPRINT "BC 73 A5 9C 6E EE 38 43 A6 37 FC 32 CF 08 16 DC CF F1 5A 66"


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);

  // Set WiFi to station mode and disconnect from an AP if it was Previously
  // connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);

  // Attempt to connect to Wifi network:
  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  IPAddress ip = WiFi.localIP();
  Serial.println(ip);

  client.setFingerprint(TEST_HOST_FINGERPRINT);
  makeHTTPRequest();
}

void makeHTTPRequest() {
  client.setTimeout(10000);
  if (!client.connect(TEST_HOST, 443))
  {
    Serial.println(F("Connection failed"));
    return;
  }

  // give the esp a breather
  yield();

  // Send HTTP request
  client.print(F("GET "));
  client.print("/api/v1/order/?format=json&limit=1&username=brianlough&api_key=refwv43tgfvdfbe4444&offset=4");
  client.println(F(" HTTP/1.1"));

  //Headers
  client.print(F("Host: "));
  client.println(TEST_HOST);

  client.println(F("Cache-Control: no-cache"));

  if (client.println() == 0)
  {
    Serial.println(F("Failed to send request"));
    return;
  }
  //delay(100);
  // Check HTTP status
  char status[32] = {0};
  client.readBytesUntil('\r', status, sizeof(status));
  if (strcmp(status, "HTTP/1.1 200 OK") != 0)
  {
    Serial.print(F("Unexpected response: "));
    Serial.println(status);
    return;
  }

  // Skip HTTP headers
  char endOfHeaders[] = "\r\n\r\n";
  if (!client.find(endOfHeaders))
  {
    Serial.println(F("Invalid response"));
    return;
  }

  while(client.available() && client.peek() != '{')
  {
    char c = 0;
    client.readBytes(&c, 1);
    Serial.print(c);
    Serial.println("BAD");
  }

  while(client.available()) {
    char c = 0;
    client.readBytes(&c, 1);
    Serial.print(c);
  }
}

void loop() {
  // put your main code here, to run repeatedly:

}
