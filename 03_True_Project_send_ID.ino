#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "Keypad.h"
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

const char* ssid     = "GNPC";
const char* password = "giveme1000";

const char* host = "192.168.137.243";   //host nick
//const char* host = "192.168.43.6";   //localhost
const int httpPort = 80;
String var = "";

const byte Rows= 4; //number of rows on the keypad i.e. 4
const byte Cols= 3; //number of columns on the keypad i,e, 3

char keymap[Rows][Cols]=
{
{'1', '2', '3'},
{'4', '5', '6'},
{'7', '8', '9'},
{'*', '0', '#'}
};

static String input = "";
static String res = "";

byte rPins[Rows]= {10,16,0,2}; //Rows 0 to 3
byte cPins[Cols]= {14,12,13}; //Columns 0 to 2

boolean check = false;

Keypad kpd = Keypad(makeKeymap(keymap), rPins, cPins, Rows, Cols);
LiquidCrystal_I2C lcd(0x27, 16, 4);

String encoder(String input){
  int code = input.toInt();
  int encode = ((code+25000)*8)+30000;
  Serial.println(code);
  Serial.println(encode);
  return String(encode);
}

void setResponse(String hostD,char res){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connect to ");
  lcd.print(ssid);
  lcd.setCursor(0,1);
  lcd.print("Connected.");
  lcd.setCursor(0,2);
  lcd.print("To ");
  lcd.print(hostD);
  lcd.setCursor(0,3);
//  int check = String(res).toInt();
//  Serial.print("Response is ");
//  Serial.println(check);
  if(res == '1'){
    lcd.print("Auth Success.");
    Serial.print("Authorizetion success.");
  }else if(res == '0'){
    lcd.print("Auth Failed!!");
    Serial.print("Authorizetion fail!!");
  }
}

void setConnect(String hostD){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connect to ");
  lcd.print(ssid);
  lcd.setCursor(0,1);
  lcd.print("Connected.");
  lcd.setCursor(0,2);
  lcd.print("To ");
  lcd.print(hostD);
  lcd.setCursor(0,3);
  lcd.print("Connection failed.");
}

void setSend(String hostD){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connect to ");
  lcd.print(ssid);
  lcd.setCursor(0,1);
  lcd.print("Connected.");
  lcd.setCursor(0,2);
  lcd.print("To ");
  lcd.print(hostD);
  lcd.setCursor(0,3);
  lcd.print("Sending....");
}

void setDisplay(String inputD){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Connect to ");
  lcd.print(ssid);
  lcd.setCursor(0,1);
  lcd.print("Connected.");
  lcd.setCursor(0,2);
  lcd.print("Enter your ID : ");
  lcd.setCursor(0,3);
  lcd.print(inputD);
}

void setup() {
  Serial.begin(115200);
  lcd.begin();
  delay(10);

  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Connect to ");
   lcd.print(ssid);
   lcd.setCursor(0,1);
   lcd.print("Disconnect....");
  }

   lcd.clear();
   lcd.setCursor(0,0);
   lcd.print("Connect to ");
   lcd.print(ssid);
   lcd.setCursor(0,1);
   lcd.print("Connected.");

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

int value = 0;

void loop() {
  
         char keypressed = kpd.getKey();
         if((keypressed == '#') && (input.length() == 18))
         {
              Serial.println("Input is : "+input);
              var = input;
              input = "";
              check = true;
         }
         else if (keypressed == '*'){
              input = "";
              Serial.println("Clear.");
              setDisplay(input);
         }
         else if (keypressed != NO_KEY)
         { 
              if(input.length() < 18 && keypressed != '#')
              {
                input += keypressed;
                Serial.println(input);
              }
              setDisplay(input);
         }
 
 if(check){
     ++value;

     Serial.print("connecting to ");
     Serial.println(host);
    
     // Use WiFiClient class to create TCP connections
     // WiFiServer server(httpPort);
     setSend(host);
     WiFiClient client;
     if (!client.connect(host, httpPort)) {
      Serial.println("connection failed");
      setConnect(host);
      return;
     }
  
     // create a URI for the request
     String url = "/uss/Auth.php?data="+encoder(var);
     //String url = "/jok/Auth.php?data="+encoder(var);
    
     Serial.print("Requesting URL: ");
     Serial.println(url);
    
     // This will send the request to the server
     client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
     unsigned long timeout = millis();
     while (client.available() == 0) {
     if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
     }
    }
    
     // Read all the lines of the reply from server and print them to Serial
     res = "";
     while(client.available()){
        String line = client.readStringUntil('\r');
//        Serial.print(line);
//        Serial.print("  ");
//        Serial.println(line.length());
        res += line;
     }
     setResponse(host,res[res.length()-2]);
     
     Serial.println();
     Serial.println("closing connection"); 
     check = false;
  }
}
