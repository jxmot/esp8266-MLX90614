/* ************************************************************************ */
/*
  This is a library example for the MLX90614 Temp Sensor
    was based on the Adafruit MLX90614 example. Many changes
    have been made.
*/
/* ************************************************************************ */
#include <Wire.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "ThingspeakClient.h"
#include "pins_arduino.h"

// header files located in this repository...
#include "configData.h"
#include "Adafruit_MLX90614.h"
// NOTE: Modifiy the example_thingspeak.h file and rename it to _thingspeak.h
#include "_thingspeak.h"

/* ************************************************************************ */
bool ConnectToAP(char *ssid, char *passw);
void logSensorData(float _amb, float _obj);

bool wifi_ready = false;

const int DEFAULT_SERIAL_BAUD = 115200;

// load the WiFi and serial baud config data...
configData *cfgdat = new configData();
// get an MLX object...
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

/* ************************************************************************ */
void setup() {
    // let's check for errors before continuing...
    String errMsg;
    if(!cfgdat->getError(errMsg)) 
    {
        // configuration was read and parsed...
        Serial.begin(cfgdat->getBAUD());
        // attempt an WiFi connection...
        wifi_ready = ConnectToAP(cfgdat->getSSID(), cfgdat->getPASS());
        // did it work?
        if(wifi_ready)
        {
            // sure did!
            Serial.println("Adafruit MLX90614 test w/ Thingspeak");
            pinMode(D7, OUTPUT);
            digitalWrite(D7, LOW);
            mlx.begin();  
        } else Serial.println("ERROR - setup() WiFi connection failed!");
    } else {
        // Reading the config file failed, did you upload it to the
        // SPIFFS?
        Serial.begin(DEFAULT_SERIAL_BAUD);
        Serial.println();
        Serial.println();
        Serial.println("ERROR - setup() - config failed!");
    }
}

void loop() {
static bool only_once = false;

    if(wifi_ready)
    {
        digitalWrite(D7, HIGH);
        Serial.print("Ambient = "); Serial.print(mlx.readAmbientTempF()); 
        Serial.print("*F\tObject = "); Serial.print(mlx.readObjectTempF()); 
        Serial.println("*F");
    
        logSensorData(mlx.readAmbientTempF(), mlx.readObjectTempF());
    
        delay(5000);
        Serial.println();
        digitalWrite(D7, LOW);
        delay(10000);
    } else {
        yield();
        if(!only_once)
        {
            only_once = true;
            Serial.println("ERROR - oops! something didn't work!!");
        }
    }
}

void logSensorData(float _amb, float _obj) {

    Serial.print("logSensorData() - connecting to ");
    Serial.println(THINGSPEAK_HOST);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    
    if (!client.connect(THINGSPEAK_HOST, HTTPPORT)) {
      Serial.println("logSensorData() - connection failed");
      return;
    }
    
    // We now create a URI for the request
    String url = "/update?api_key=";
    url += THINGSPEAK_API_WRITE_KEY;
    url += "&field1=";
    url += String(_amb);
    url += "&field2=";
    url += String(_obj);
    
    Serial.print("logSensorData() - Requesting URL: ");
    Serial.println(url);
    
    // This will send the request to the server
    // NOTE: Why does Thingspeak use GET and not POST???
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + THINGSPEAK_HOST + "\r\n" + 
                 "Connection: close\r\n\r\n");
    delay(10);

    while(!client.available()){
      delay(100);
      Serial.print(".");
    }
    Serial.println(".");

//#define PRINT_SERVER_RESPONSE
#ifdef PRINT_SERVER_RESPONSE
    // Read all the lines of the reply from server and print them to Serial
    while(client.available()){
      String line = client.readStringUntil('\r');
      Serial.print(line);
    }
    Serial.println();
#endif
    Serial.println("logSensorData() - closing connection");
    client.stop();
}

bool ConnectToAP(char *ssid, char *passw) {
uint8_t mac[6];
char macStr[18] = { 0 };

    Serial.println();
    Serial.println();
    Serial.println("Attempting to Connect to WiFi....");
    
    while (true) {
        delay(1000);
        Serial.print("Connecting to ");
        Serial.println(ssid);
        WiFi.mode(WIFI_STA);
        WiFi.begin(ssid, passw);
        for (int x = 0; x < 5; x++) {
            delay(1000);
            if (WiFi.status() == WL_CONNECTED) {
                Serial.print("WiFi connected in ");
                Serial.print(x);
                Serial.println(" seconds");
                Serial.print("IP address: ");
                Serial.println(WiFi.localIP().toString());
            
                WiFi.macAddress(mac);
                sprintf(macStr, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
                Serial.print("MAC address: ");
                Serial.println(macStr);
                Serial.println();
                Serial.println();
                return true;
            }
        }
        return false;
    }
}

