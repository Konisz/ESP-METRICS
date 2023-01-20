#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <WiFiClient.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "credentials.h" // define Wi-Fi SSID and Password 

#define ONE_WIRE_BUS D5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

ESP8266WebServer server(8022);

String device_hostname = "ESP-NODE1";
float readTemperature;
unsigned long delayTime;

String GenerateMetrics() {
  String message = "";

  message += "esp_node1 ";
  message += readTemperature;
  message += "\n";

  return message;
}

void HandleRoot() {
  server.send(200, "text/plain", GenerateMetrics() );
}

void HandleNotFound() {
  String message = "Error\n\n";
  server.send(404, "text/html", message);
}

void setup() {
  WiFi.begin(WIFI_SSID,WIFI_PASS);
  WiFi.hostname(device_hostname.c_str());

  server.on("/", HandleRoot);
  server.on("/metrics", HandleRoot);
  server.onNotFound( HandleNotFound );

  server.begin();
}

void loop() {
  sensors.requestTemperatures(); 
  readTemperature = sensors.getTempCByIndex(0);
  server.handleClient();
  delay(delayTime);
}