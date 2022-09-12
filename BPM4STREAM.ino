#include <Wire.h>
#include "MAX30105.h"
#include <ESP8266WiFi.h>
#include "heartRate.h"
// MAX30102 lib: https://github.com/sparkfun/SparkFun_MAX3010x_Sensor_Library

MAX30105 particleSensor;
WiFiServer server(80);

const byte RATE_SIZE = 100; //Increase this for more averaging. 4 is good.
byte rates[RATE_SIZE]; //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0; //Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

void setup()
{
    Serial.begin(115200);
    Serial.println("Initializing...");
    WiFi.mode(WIFI_STA);
    WiFi.begin("", ""); // <--- Please setup this ! SSEID + PSW

    while (WiFi.status() != WL_CONNECTED) {delay(500);}
    server.begin();
    Serial.println(WiFi.localIP()); // the local IP of the ESP8266 server where you connect your OBS "browser"

  // Initialize sensor
    if (!particleSensor.begin(Wire, I2C_SPEED_FAST)) //Use default I2C port, 400kHz speed
    {
        Serial.println("MAX30105 was not found. Please check wiring/power. ");
        while (1);
    }
    Serial.println("Place your index finger on the sensor with steady pressure.");

    byte ledBrightness = 0x1F; //Options: 0=Off to 255=50mA <0x1F>
    byte sampleAverage = 4; //Options: 1, 2, <4>, 8, 16, 32
    byte ledMode = 2; //Options: 1 = Red only, 2 = Red + IR, <3> = Red + IR + Green
    int sampleRate = 400; //Options: 50, 100, 200, <400>, 800, 1000, 1600, 3200
    int pulseWidth = 411; //Options: 69, 118, 215, <411>
    int adcRange = 4096; //Options: 2048, <4096>, 8192, 16384
    particleSensor.setup(ledBrightness, sampleAverage, ledMode, sampleRate, pulseWidth, adcRange); //Configure sensor with these settings
    particleSensor.setPulseAmplitudeRed(0x0A); //Turn Red LED to low to indicate sensor is running
    particleSensor.setPulseAmplitudeGreen(0); //Turn off Green LED
}

void loop()
{
    long irValue = particleSensor.getIR();

    if (checkForBeat(irValue) == true)
    {
        //We sensed a beat!
        long delta = millis() - lastBeat;
        lastBeat = millis();

        beatsPerMinute = 60 / (delta / 1000.0);

        if (beatsPerMinute < 255 && beatsPerMinute > 20)
        {
            rates[rateSpot++] = (byte)beatsPerMinute; //Store this reading in the array
            rateSpot %= RATE_SIZE; //Wrap variable
        }
    }

    Serial.print("BPM: ");
    Serial.print(beatsPerMinute);
    Serial.println();

    WiFiClient client = server.available();
    if (!client) {
        return;
    }
    client.print(F("<!DOCTYPE html><html lang='en'><head> <meta charset='UTF-8'> <meta http-equiv='X-UA-Compatible' content='IE=edge'> <meta name='viewport' content='width=device-width, initial-scale=1.0'> <title>beats</title> <style>body{background: #111723;text-align: center;"));
    client.print(F("color:#09F;"));
    client.print(F("font-size: 7vw;"));
    client.print(F("font-family: 'Segoe UI', Tahoma, Geneva, Verdana, sans-serif;"));
    client.print(F("}#txtbpm{display:block;}#graph{height: 75%;width: 75%;margin:0 auto;display:block;}</style></head><body><span id='txtbpm'>"));
    client.print((int) beatsPerMinute); // The famous BPM
    client.print(F(" BPM</span><canvas id='graph' width='1280' height='720'></canvas> <script>var graph; var data={values:["));
    for (byte x = 0 ; x < RATE_SIZE ; x++) { // create array for JS
        client.print(F("{Y: "));
        client.print(rates[x]);
        client.print(F("},"));
    }
    client.print(F("]};var actpos="));
    client.print(rateSpot); // actual position in the array
    client.print(F(";"));
    client.print(F("var datatemp = data.values;data.values = data.values.splice(actpos+1,(100-actpos)).concat(datatemp.splice(0,actpos));")); // reorder array
    client.print(F("function getMaxY(){var max=0; for(var i=0; i < data.values.length; i ++){if(data.values[i].Y > max){max=data.values[i].Y;}}max +=10 - max % 10; return max;}function getXPixel(val){return (graph.width / data.values.length) * val + 1.5;}function getYPixel(val){return graph.height - ((graph.height / getMaxY()) * val);}window.addEventListener('DOMContentLoaded', ()=>{graph=document.getElementById('graph'); var c=graph.getContext('2d');c.lineWidth=10;
    client.print(F("c.strokeStyle='#039'; "));
    client.print(F("c.beginPath(); c.moveTo(getXPixel(0), getYPixel(data.values[0].Y));"));
    client.print(F("for(var i=0; i < data.values.length; i ++){c.lineTo(getXPixel(i), getYPixel(data.values[i].Y));}c.stroke();});setTimeout(() => {document.location = '/'},500) </script></body></html>"));
}
