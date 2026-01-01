#include <ESP8266WiFi.h>
#include <PubSubClient.h>

const char* ssid = "Mohamed Fathy";
const char* password = "341978341978";
const char* mqtt_server = "192.168.100.25";

WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi() {
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  char message[length + 1];
  
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0'; 

  Serial.print(message);
  Serial.print('\n');
}

void reconnect() {
  while (!client.connected()) {
    if (client.connect("ESP01_Client")) {
      client.subscribe("/home/stm32");
    } 
    
    else {
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}