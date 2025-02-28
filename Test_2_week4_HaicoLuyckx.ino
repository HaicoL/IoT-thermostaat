#define BLYNK_TEMPLATE_ID "user9"
#define BLYNK_TEMPLATE_NAME "user9@server.wyns.it"
#define BLYNK_PRINT Serial

#include <DHT.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

char auth[] = "8Wle5tD5B__VxYCnc7kVadJXSRUZWZEm";
char ssid[] = "Proximus-Home-089155";
char pass[] = "xrsmp5uk6uj2kss6";

#define DHTTYPE DHT11
#define DHTPIN 4
#define PIN_LED 23

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
WidgetLED led1(V4);

float wantedTemp;
bool ketelStatus;

BLYNK_WRITE(V3) {
  float currentTemp = dht.readTemperature();
  wantedTemp = param.asFloat();

  if (currentTemp < wantedTemp) {
    led1.on();
    ketelStatus = true;
  } else {
    led1.off();
    ketelStatus = false;
  }

  Blynk.virtualWrite(V2, wantedTemp);
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED, OUTPUT);
  delay(1000);

  Blynk.begin(auth, ssid, pass, "server.wyns.it", 8081);

  dht.begin();

  timer.setInterval(1000L, sendThermostatData);
}

void loop() {
  Blynk.run();
  timer.run();
}

void sendThermostatData() {
  float temp = dht.readTemperature();

  Serial.print("");
  Serial.println("-----------------");
  Serial.print("Ingestelde temp: ");
  Serial.print(wantedTemp);
  Serial.println("°C");

  if (isnan(temp)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  } else {
    Serial.print("Gemeten temperatuur: ");
    Serial.print(temp);
    Serial.println("°C");
  }

  if (temp < wantedTemp) {
    digitalWrite(PIN_LED, HIGH);
    Serial.println("Ketel AAN");
  } else {
    digitalWrite(PIN_LED, LOW);
    Serial.println("Ketel UIT");
  }  
  
  Serial.println("-----------------");
  Serial.print("");

  Blynk.virtualWrite(V1, temp);
}
