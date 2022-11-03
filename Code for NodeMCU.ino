#include<Wire.h>
#include<LiquidCrystal_I2C.h>
#include <ESP8266WiFi.h>;
#include <WiFiClient.h>;
#include <ThingSpeak.h>;

const char* ssid = "POCO";   // Your Network SSID
const char* password = "c0553ct8";       // Your Network Password

//const char* ssid = "Megatorn";   // Your Network SSID
//const char* password = "979331102";       // Your Network Password

WiFiClient client;

unsigned long myChannelNumber = 1766517; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "1B77BZOAMITDYKLV"; //Your Write API Key


LiquidCrystal_I2C lcd(0x27,16,2);

int X;
int Y;
float TIME = 0;
float FREQUENCY = 0;
float WATER = 0;
float TOTAL = 0;
float LS = 0;
const int input = D5;




void setup()
{
Serial.begin(115200);

delay(10);
  // Connect to WiFi network
  WiFi.begin(ssid, password);
  ThingSpeak.begin(client);
 
lcd.begin();
lcd.backlight();
lcd.clear();
lcd.setCursor(0,0);
lcd.print("Water Flow Meter");
lcd.setCursor(0,1);
lcd.print("****************");
delay(2000);
pinMode(input,INPUT);
}


void loop()
{
X = pulseIn(input, HIGH);
Y = pulseIn(input, LOW);
TIME = X + Y;
FREQUENCY = 1000000/TIME;
WATER = FREQUENCY/7.5;
LS = WATER/60;
if(FREQUENCY >= 0)
{
if(isinf(FREQUENCY))
{
lcd.clear();
lcd.setCursor(0,0);
lcd.print("SPEED :0.00 L/M");
lcd.setCursor(0,1);
lcd.print("TOTAL:");
lcd.print( TOTAL);
lcd.print(" L");
ThingSpeak.writeField(myChannelNumber, 2,TOTAL, myWriteAPIKey);
Serial.println(TOTAL);

}
else
{
TOTAL = TOTAL + LS;
Serial.println(FREQUENCY);
lcd.clear();
lcd.setCursor(0,0);

//Print Speed in lcd
lcd.print("SPEED: ");
lcd.print(WATER);
lcd.print(" L/M");

//Print Speed in Serial monitor And server
Serial.print("Speed: ");
Serial.print(WATER);                 // Print on Serial Monitor
Serial.println(" L/M");
delay(1000);
ThingSpeak.writeField(myChannelNumber, 3,WATER, myWriteAPIKey); //Update in ThingSpeak
ThingSpeak.writeField(myChannelNumber, 2,TOTAL, myWriteAPIKey); //Update in ThingSpeak
delay(100);

lcd.setCursor(0,1);

//Print total water in lcd
lcd.print("TOTAL:");
lcd.print( TOTAL);
lcd.print(" L");

//Print total water in S.Monitor and server
Serial.print("Total: ");
Serial.print(TOTAL);                 // Print on Serial Monitor
Serial.println(" L");
delay(1000);

delay(100);

}
}
delay(1000);
}
