

/*
Erik och Mathildas "väder station".
det är många fler saker som om tiden tillåtit skulle ha gjorts så som fixa bättre variable namn, göra fler funktioner (mycket onödig upprepad kod) och sökfunktion i stället för hårdkodade städer etc.
med det sagt så är vi väldigt nöjda med slutresultatet och att vi över huvud taget fick det att fungera så bra som det gjorde. 
kommentarer i koden är gjorda på saker vi anser relevant alltså inte på sådant som WiFi och client kod då det varken är originellt eller vad vi anser vara relevant. 
hoppas att det inte gör något att det är skrivet på engelska då det känns bättre.
*/
#include <LiquidCrystal_I2C.h>
#include <LCD.h>
#include <LedControl.h>
#include <SoftwareSerial.h>
#include "WiFiEsp.h"
#include <ArduinoJson.h>
#include <SPI.h>
#include <Wire.h>

const int port = 80;
const char serverJson[] = "api.openweathermap.org";
const char ssid[] =  "AndroidAP"; 
const char pass[] = "nevc6096";
const String email = "eri_byd@hotmail.com";
const String xid = "dicegame24";
const String hostname = "primat.se";
const String APIKey = "0c6c77820236b1a47e8ac996c35c21c8";
String data = "P1,4,2,1";

// Create module object on GPIO pin
// wifi 6 (RX) and 7 (TX)
SoftwareSerial mySerial1(10, 11);
  WiFiEspClient clientJson;
  
int switchNumber = 0;
int x = 0;
int i = 0;
int y = 0;
String change;

int DIN = 22;
int CS = 24;
int CLK = 26;
LedControl lc=LedControl(DIN,CLK,CS,0);

LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE); 

// Declare and initialise variable for radio status
int status = WL_IDLE_STATUS;
WiFiEspServer server(80);
//RingBuffer buf(8);
WiFiEspClient client;

void setup()
{


  
    lcd.begin(16,2);          //how many rows and collums
  lcd.backlight();            //backlights on
  lcd.setCursor(0,0);          //set starting point
  lc.shutdown(0,false);       //The MAX72XX is in power-saving mode on startup
  lc.setIntensity(0,15);      // Set the brightness to maximum value
  lc.clearDisplay(0);         // and clear the display
   
    // Initialize serial for debugging
    Serial.begin(115200);
    // Initialize serial
   mySerial1.begin(9600);
    wifiInit();
}

void loop()
{
  
client = server.available();
if(client) {  


    Serial.println(F("New client"));
    boolean currentLineIsBlank = true;
    while (client.connected()) {
          if (client.available()) {
            
        char c = client.read();


        if(i<6)// had issue before with i==5 here so i stuck with the i<6, and it is to get the 6th char saved in the "c" that will later have the value of the choice we have made on the server
        {

          if(i==5 ) // might not need this if statment but "if its not broken dont fix it"
          {
            

            if(c=='1' || c=='2' || c=='3' || c=='4' || c=='5' || c=='6' || c=='7')// this is to prevent a use of values from the "favocon" not sure of the name but its the server response i get before the one i want  
            {
              change = c; // change char c to a string
              switchNumber = change.toInt();// change string to int
                       Serial.print(c); 
              Serial.print(switchNumber); 
               }
            }
          }
          i++; // counter incrementer for the if statement just used
      
         


        if (c == '\n' && currentLineIsBlank) 
        {
          Serial.println(F("Sending response"));
          i=0;// when server is done it will also reset the counter to 0
                  Serial.println(switchNumber);

          // Send a standard HTTP response header
          client.print(
            "HTTP/1.1 200 OK\r\n"
            "Content-Type: text/html\r\n"
            "Connection: close\r\n"
            "\r\n");

String html ="<!DOCTYPE html> <html> <head> <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\"> <link rel= \"stylesheet\" href=\"https://zztorp.github.io/WeatherDataHtmlCss/main.css\"> </head> <body> <h1 align=\"center\">Mathilda & Eriks IoT-Projekt</h1> <h1 align=\"center\">Weather Data</h1> <div class=\"row\"> <div class=\"column\"> <div class=\"container\"> <img src=\"https://zztorp.github.io/WeatherDataHtmlCss/Stockholm.jpg\"  style=\"width:100%\"> <button class=\"btn\" onclick=\"window.location.href='/2'\"> Stockholm</button> </div> <div class=\"column\"> <div class=\"container\"> <img src=\"https://zztorp.github.io/WeatherDataHtmlCss/Tokyo.jpg\" style=\"width:100%\" alt=\"centered image\"/> <button class=\"btn\" onclick=\"window.location.href='/1'\">Tokyo</button> </div> <div class=\"column\"> <div class=\"container\"> <img src=\"https://zztorp.github.io/WeatherDataHtmlCss/Paris.jpg\" alt=\"Paris\" style=\"width:100%\"> <button class=\"btn\" onclick=\"window.location.href='/5'\">Paris</button> </div> <div class=\"column\"> <div class=\"container\"> <img src=\"https://zztorp.github.io/WeatherDataHtmlCss/Amsterdam.jpg\" alt=\"Amsterdam\" style=\"width:100%\"> <button class=\"btn\" onclick=\"window.location.href='/3'\">Amsterdam</button> </div> <div class=\"column\"> <div class=\"container\"> <img src=\"https://zztorp.github.io/WeatherDataHtmlCss/London.jpg\" alt=\"London\" style=\"width:100%\"> <button class=\"btn\" onclick=\"window.location.href='/6'\">London</button> </div> <div class=\"column\"> <div class=\"container\"> <img src=\"https://zztorp.github.io/WeatherDataHtmlCss/Barcelona.jpg\" alt=\"Barcelona\" style=\"width:100%\"> <button class=\"btn\" onclick=\"window.location.href='/4'\">Barcelona</button> </div> <div class=\"column\"> <div class=\"container\"> <img src=\"https://zztorp.github.io/WeatherDataHtmlCss/Sydney.jpg\" alt=\"Sydney\" style=\"width:100%\"> <button class=\"btn\" onclick=\"window.location.href='/7'\">Sydney</button> </div> </div> </body> </html>";
client.print(html);
            
         
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }

      }
    }
    delay(3000);
        Serial.println(switchNumber);

    client.stop();

    Serial.println(F("Client disconnected"));
        Serial.println(switchNumber);

   if(!switchNumber == 0){jSon();}// when the server is done we hopefully will have a new value for switchnumber which will then start the Json function
  }
}


void jSon()
{

client.flush();// here is also a "if its not broken dont fix it" kinda thing
client.stop();// i dont know if its needed its more of a failsafe to clear both clients that i used when it was not working and then they have stayed
clientJson.flush();// with more time i would have sorted out it they are needed or not
clientJson.stop();
  String city;
 

switch(switchNumber) // here we use the variable with the value of the response from the server to change what city to use in the API call
{
  case 1:
  city = "tokyo";
  break;
  case 2:
  city = "Stockholm";
  break;
  case 3:
  city = "Amsterdam";
  break;
  case 4:
  city = "Barcelona";
  break;
  case 5:
  city = "Paris";
  break;
  case 6:
  city = "London";
  break;
  case 7:
  city =  "Sydney";
  break;
 
  }
  String uri = "/data/2.5/weather?q=" + city + "&APPID=" + APIKey; // here we craft the whole key for the call to openweather
  
delay(1000);
    // if you get a connection, report back via serial
    if (clientJson.connect(serverJson, port))
    {
        Serial.println(F("Connected to server"));
        // Make a HTTP request
        clientJson.println("GET " + uri + " HTTP/1.1");
        clientJson.println("Host: " + hostname);
        clientJson.println("Connection: close");
        clientJson.println();
    }

    while (clientJson.available() == 0)
    {
        delay(5);
        Serial.print(F("..."));
    }

      
   char endOfHeaders[] = "\r\n\r\n"; // this is used to remove the header so we are left with only the Json text
      if (!clientJson.find(endOfHeaders)) {
   Serial.println(F("Invalid response"));
   Serial.print(F("innan buff"));

   return;
  }
     
StaticJsonBuffer <2000> jsonBuffer; // had no need to use a dynamic buffer when we change to an Omega so we left it as a static with more then enough space
JsonObject& root = jsonBuffer.parseObject(clientJson);// creating the json object used to extract the values from the API call

    if(!root.success()) {Serial.println(F("parseObject() failed"));}

JsonObject& weather_0 = root["weather"][0]; // faced problem when extracting the "main" and "description" from jsonobject root, so we made another jsonobject based on the first object that would only have the values inside "weather" 

String weather = weather_0 ["main"];// extracting all variables needed for the program
String CITY = root["name"];
String description = weather_0["description"];
String temp =  root["main"]["temp"];
float celsiusTemp  = temp.toFloat(); // temp comes in as a string so we change it to a float
celsiusTemp = celsiusTemp - 273.15; // temp is in Kelvin so we put it in a formula to change it immediately to celsius

  Serial.print(weather);
  Serial.print(CITY);
  Serial.print(description);

     if(weather == "Clouds"){cloud(CITY,weather,description,celsiusTemp);}// here we have 5 functions for different weather and one else statement for all the other weathers 
else if(weather == "Clear"){sun(CITY,weather,description,celsiusTemp);}
else if(weather == "Thunderstorm"){thunderstorm(CITY,weather,description,celsiusTemp);}
else if (weather == "Rain"){rain(CITY,weather,description,celsiusTemp);}
else if (weather == "Snow"){snow(CITY,weather,description,celsiusTemp);}
else
{
  
int lenght = description.length();
    lcd.clear();// clears the LCD screen
    lcd.setCursor(0,0); // sets the start point to top left
    lcd.print("City: ");
    lcd.print(CITY);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(celsiusTemp);
     delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Weather: ");
    lcd.print(description);
     delay(2000);
for (int positionCounter = 16; positionCounter < lenght + 9; positionCounter++) { // for loop that will scroll the screen IF the string is to long
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
     delay(400);
}
     delay(2000);

    for (int IGNORE = 0; IGNORE < lenght + 9 ; IGNORE++) {// PLEASE IGNORE this is the weird for loop i needed to be able to use lcd.clear(); could not get it to work any other way....  
 
    lcd.clear();
    
  }

      lcd.home();
   delay(500);
   
 lcd.print("City: ");
    lcd.print(CITY);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(celsiusTemp);
     
  }



delay(3000);
  switchNumber = 0;
  clientJson.stop();
  }


  
void wifiInit()
{
    // Initialize ESP module
    WiFi.init(&mySerial1);
    // Check for the presence of the shield
    if (WiFi.status() == WL_NO_SHIELD)
    {
        Serial.println(F("WiFi shield not present"));
        // Don't continue
        while (true)
        {
        
        }       
    }

    // Attempt to connect to WiFi network
    while (status != WL_CONNECTED)
    {
        Serial.print(F("Attempting to connect to SSID: "));
        Serial.println(ssid);
        // Connect to WPA/WPA2 network
        status = WiFi.begin(ssid, pass);
    }

    server.begin();
    IPAddress ip = WiFi.localIP();
    Serial.print(F("IP Address: "));
    Serial.println(ip);

    Serial.println(F("You're connected to the network"));
    printWifiStatus();
    Serial.println();

}




void printWifiStatus()
{
    // print the SSID of the network you're attached to
    Serial.print(F("SSID: "));
    Serial.println(WiFi.SSID());

    // print the received signal strength
    long rssi = WiFi.RSSI();
    Serial.print(F("Signal strength (RSSI):"));
    Serial.print(rssi);
    Serial.println(F(" dBm"));

}

void printByte(byte character []) //subfunktion used by all the weather functions to "print" on the matrix
{
  int i = 0;
  for(i=0;i<8;i++)
  {
    lc.setRow(0,i,character[i]);
  }
}
void cloud(String STAD,String VADER,String BESKRIVNING, float TEMP)
{
    byte clouds[8] = {0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,};
    byte clouds2[8] = {0x00,0x01,0x03,0x01,0x00,0x00,0x00,0x00,};
    byte clouds3[8] = {0x00,0x03,0x07,0x03,0x00,0x00,0x00,0x00,};
    byte clouds4[8] = {0x01,0x07,0x0F,0x07,0x00,0x00,0x00,0x00,};
    byte clouds5[8] = {0x03,0x0F,0x1F,0x0F,0x00,0x00,0x00,0x00,};
    byte clouds6[8] = {0x06,0x1F,0x3F,0x1F,0x00,0x00,0x00,0x00,};
    byte clouds7[8] = {0x0C,0x3F,0x7F,0x3F,0x00,0x00,0x00,0x00,};
    byte clouds8[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x00,0x01,0x00,};
    byte clouds9[8] = {0x30,0xFC,0xFE,0xFC,0x00,0x01,0x03,0x01,};
    byte clouds10[8] = {0x60,0xF8,0xFC,0xF8,0x00,0x03,0x07,0x03,};
    byte clouds11[8] = {0xC0,0xF0,0xF8,0xF0,0x01,0x07,0x0F,0x07,};
    byte clouds12[8] = {0x80,0xE0,0xF0,0xE0,0x03,0x0F,0x1F,0x0F,};
    byte clouds13[8] = {0x00,0xC0,0xE0,0xC0,0x06,0x1F,0x3F,0x1F,};
    byte clouds14[8] = {0x00,0x80,0xC0,0x80,0x0C,0x3F,0x7F,0x3F,};
    byte clouds15[8] = {0x00,0x00,0x80,0x00,0x18,0x7E,0xFF,0x7E,};
    byte clouds16[8] = {0x00,0x00,0x00,0x00,0x30,0xFC,0xFE,0xFC,};
    byte clouds17[8] = {0x00,0x00,0x00,0x00,0x60,0xF8,0xFC,0xF8,};
    byte clouds18[8] = {0x00,0x00,0x00,0x00,0xC0,0xF0,0xF8,0xF0,};
    byte clouds19[8] = {0x00,0x00,0x00,0x00,0x80,0xE0,0xF0,0xE0,};
    byte clouds20[8] = {0x00,0x00,0x00,0x00,0x00,0xC0,0xE0,0xC0,};
    byte clouds21[8] = {0x00,0x00,0x00,0x00,0x00,0x80,0xC0,0x80,};
    byte clouds22[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,};
    byte clouds23[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};
    int lenght = BESKRIVNING.length();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Weather: ");
    lcd.print(BESKRIVNING);
     delay(2000);
for (int positionCounter = 16; positionCounter < lenght + 9; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
     delay(400);
}
delay(2000);

    for (int IGNORE = 0; IGNORE < lenght + 9 ; IGNORE++) {
 
    lcd.clear();
    
  }

      lcd.home();
   delay(500);
   
 lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
          
      for(int k=0;k<3;k++){
    printByte(clouds);
    delay (200);
    printByte(clouds2);
    delay (200);
    printByte(clouds3);
    delay (200);
    printByte(clouds4);
    delay (200);
    printByte(clouds5);
    delay (200);
    printByte(clouds6);
    delay (200);
    printByte(clouds7);
    delay (200);
    printByte(clouds8);
    delay (200);
    printByte(clouds9);
    delay (200);
    printByte(clouds10);
    delay (200);
    printByte(clouds11);
    delay (200);
    printByte(clouds12);
    delay (200);
    printByte(clouds13);
    delay (200);
    printByte(clouds14);
    delay (200);
    printByte(clouds15);
    delay (200);
    printByte(clouds16);
    delay (200);
     printByte(clouds17);
    delay (200);
    printByte(clouds18);
    delay (200);
    printByte(clouds19);
    delay (200);
    printByte(clouds20);
    delay (200);
    printByte(clouds21);
    delay (200);
    printByte(clouds22);
    delay (200);
    printByte(clouds23);
    delay(200);
      }

}

void rain(String STAD,String VADER,String BESKRIVNING, float TEMP)
{
    byte rain1[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x00,0x00,0x00};
    byte rain2[8] = {0x18,0x7E,0xFF,0x7E,0x24,0x00,0x00,0x00};
    byte rain3[8] = {0x18,0x7E,0xFF,0x7E,0x24,0x24,0x00,0x00};
    byte rain4[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x24,0x24,0x00};
    byte rain5[8] = {0x18,0x7E,0xFF,0x7E,0x42,0x00,0x24,0x24};
    byte rain6[8] = {0x18,0x7E,0xFF,0x7E,0x42,0x42,0x00,0x24};
    byte rain7[8] = {0x18,0x7E,0xFF,0x7E,0x10,0x42,0x42,0x00};
    byte rain8[8] = {0x18,0x7E,0xFF,0x7E,0x10,0x10,0x42,0x42};
    byte rain9[8] = {0x18,0x7E,0xFF,0x7E,0x04,0x10,0x10,0x42};
    byte rain10[8] = {0x18,0x7E,0xFF,0x7E,0x24,0x04,0x10,0x10};
    byte rain11[8] = {0x18,0x7E,0xFF,0x7E,0x20,0x24,0x04,0x10};
    byte rain12[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x20,0x24,0x04};
    byte rain13[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x00,0x20,0x24};
    byte rain14[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x00,0x00,0x20};
    byte rain15[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};
        
int lenght = BESKRIVNING.length();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Weather: ");
    lcd.print(BESKRIVNING);
     delay(2000);
for (int positionCounter = 16; positionCounter < lenght + 9; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(400);
}
delay(2000);

    for (int IGNORE = 0; IGNORE < lenght + 9 ; IGNORE++) {
 
    lcd.clear();
    
  }

      lcd.home();
   delay(500);
   
 lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     
         for(int k=0;k<3;k++){
    printByte(rain1);
    delay (200);
    printByte(rain2);
    delay (200);
    printByte(rain3);
    delay (200);
    printByte(rain4);
    delay (200);
    printByte(rain5);
    delay (200);
    printByte(rain6);
    delay (200);
    printByte(rain7);
    delay (200);
    printByte(rain8);
    delay (200);
    printByte(rain9);
    delay (200);
    printByte(rain10);
    delay (200);
    printByte(rain11);
    delay (200);
    printByte(rain12);
    delay (200);
    printByte(rain13);
    delay (200);
    printByte(rain14);
    delay (200);
         }
         printByte(rain15);
}

void sun(String STAD,String VADER,String BESKRIVNING, float TEMP)
{
   byte sun1[8] = {0x00,0x00,0x18,0x3C,0x3C,0x18,0x00,0x00,};
    byte sun2[8] = {0x00,0x08,0x3C,0x7C,0x3E,0x3C,0x10,0x00,};
    byte sun3[8] = {0x08,0x4A,0x3C,0xFC,0x3F,0x3C,0x52,0x10,};
    byte sun4[8] = {0x89,0x4A,0x3C,0xFC,0x3F,0x3C,0x52,0x91,};
   byte sun5[8] = {0x00,0x4A,0x3C,0x7C,0x3E,0x3C,0x52,0x00,};

    byte sun6[8] = {0x00,0x00,0x3C,0x3C,0x3C,0x3C,0x00,0x00,};
    byte sun7[8] = {0x00,0x00,0x18,0x3C,0x3C,0x18,0x00,0x00,};
    byte sun8[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};


int lenght = BESKRIVNING.length();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Weather: ");
    lcd.print(BESKRIVNING);
     delay(2000);
for (int positionCounter = 16; positionCounter < lenght + 9; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(400);
}
delay(2000);

    for (int IGNORE = 0; IGNORE < lenght + 9 ; IGNORE++) {
 
    lcd.clear();
    
  }

      lcd.home();
   delay(500);
   
 lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     


      for(int k=0;k<6;k++){
    printByte(sun1);
    delay(200);
    printByte(sun2);
    delay(200);
    printByte(sun3);
    delay(200);
    printByte(sun4);
    delay(200);
    printByte(sun5);
    delay(200);
    printByte(sun1);
    delay(200);
    printByte(sun2);
    delay(200);
    printByte(sun3);
    delay(200);
    printByte(sun4);
    delay(200);
    printByte(sun5);
    delay(200);
      }
    printByte(sun8);
}

void thunderstorm(String STAD,String VADER,String BESKRIVNING, float TEMP)
{
  byte thunderstorm[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x00,0x00,0x00};
    byte thunderstorm2[8] = {0x18,0x7E,0xFF,0x7E,0x18,0x30,0x60,0x80};
    byte thunderstorm3[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};


int lenght = BESKRIVNING.length();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Weather: ");
    lcd.print(BESKRIVNING);
     delay(2000);
for (int positionCounter = 16; positionCounter < lenght + 9; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(400);
}
delay(2000);

    for (int IGNORE = 0; IGNORE < lenght + 9 ; IGNORE++) {
 
    lcd.clear();
    
  }

      lcd.home();
   delay(500);
   
 lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     

      for(int k=0;k<20;k++){

    printByte(thunderstorm);
    delay (200);
    printByte(thunderstorm2);
    delay (200);
      }
      printByte(thunderstorm3);
  }

  void snow(String STAD,String VADER,String BESKRIVNING, float TEMP)
  {
    byte snow[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x00,0x00,0x00};
    byte snow2[8] = {0x18,0x7E,0xFF,0x7E,0x20,0x00,0x00,0x00};
    byte snow3[8] = {0x18,0x7E,0xFF,0x7E,0x04,0x20,0x00,0x00};
    byte snow4[8] = {0x18,0x7E,0xFF,0x7E,0x10,0x04,0x20,0x00};
    byte snow5[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x10,0x04,0x20};
    byte snow6[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x00,0x10,0x04};
    byte snow7[8] = {0x18,0x7E,0xFF,0x7E,0x00,0x00,0x00,0x10};
    byte snow8[8] = {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};


int lenght = BESKRIVNING.length();
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     delay(3000);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Weather: ");
    lcd.print(BESKRIVNING);
     delay(2000);
for (int positionCounter = 16; positionCounter < lenght + 9; positionCounter++) {
    // scroll one position left:
    lcd.scrollDisplayLeft();
    // wait a bit:
    delay(400);
}
delay(2000);

    for (int IGNORE = 0; IGNORE < lenght + 9 ; IGNORE++) {
 
    lcd.clear();
    
  }

      lcd.home();
   delay(500);
   
 lcd.print("City: ");
    lcd.print(STAD);
    lcd.setCursor(0,1);
    lcd.print("Temp: ");
    lcd.print(TEMP);
     

      for(int k=0;k<8;k++){
    printByte(snow);
    delay(200);
    printByte(snow2);
    delay(200);
    printByte(snow3);
    delay(200);
    printByte(snow4);
    delay(200);
    printByte(snow5);
    delay(200);
    printByte(snow6);
    delay(200);
    printByte(snow7);
    delay(200);
      }
    printByte(snow8);
}


//done:)
  
