#include <ESP8266WiFi.h>

#define ssid "GN"
#define password "giveme500"

const char* host = "127.0.0.1";
boolean checkConnect = false;

WiFiClient client;

void setup() {
  Serial.begin(115200);
  Serial.println();
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  //client.connect(host, 5000);
  Serial.println("Status : "+client.connect(host, 5000));
  
  if (checkConnect) {
    Serial.print("Connected to: ");
    Serial.println(host);
    String data = "No Message";
    client.print("Test Project True");
    if(client.available()) {
      data = client.readStringUntil('\n');
    }
    Serial.print("Host message: ");
    Serial.println(data);
    client.stop();
  } else {
    client.stop();
  }
}

void loop() {
}
