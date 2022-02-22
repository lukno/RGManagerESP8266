#include <ArduinoMqttClient.h>
#include <ESP8266WiFi.h>
#include "Adafruit_VL53L0X.h"
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = "DESKTOP";        // your network SSID (name)
char pass[] = "6e!91A88";    // your network password (use for WPA, or use as key for WEP)

WiFiClient wifiClient;
MqttClient mqttClient(wifiClient);
Adafruit_VL53L0X lox = Adafruit_VL53L0X();

VL53L0X_RangingMeasurementData_t measure;

const char broker[] = "test.mosquitto.org";
int        port     = 1883;
const char topic[]  = "university/race/time";

void setup() {
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to WPA SSID: ");
  Serial.println(ssid);
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    // failed, retry
    Serial.print(".");
    delay(5000);
  }

  Serial.println("You're connected to the network");
  Serial.println();

  Serial.print("Attempting to connect to the MQTT broker: ");
  Serial.println(broker);

  if (!mqttClient.connect(broker, port)) {
    Serial.print("MQTT connection failed! Error code = ");
    Serial.println(mqttClient.connectError());

    while (1);
  }

  Serial.println("You're connected to the MQTT broker!");
  Serial.println();
  
  Serial.println("Adafruit VL53L0X test");
  if (!lox.begin())
  {
  Serial.println(F("Failed to boot VL53L0X"));
  while(1);
  }
}

void loop() {
 
  // call poll() regularly to allow the library to send MQTT keep alive which
  // avoids being disconnected by the broker
  mqttClient.poll();

  lox.rangingTest(&measure, false); // pass in 'true' to get debug data printout!

if(measure.RangeMilliMeter <200)
{
  
  Serial.print("Distance (mm): "); Serial.println(measure.RangeMilliMeter);
  String odl = String(measure.RangeMilliMeter);
  
  Serial.print("Sending message to topic: ");
  Serial.println(topic);
  
  mqttClient.beginMessage(topic);
  mqttClient.print(odl);
  mqttClient.endMessage();

  digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
  delay(300);                       // wait for a second
  digitalWrite(LED_BUILTIN, HIGH);  

  delay(1500);
}
      
}
