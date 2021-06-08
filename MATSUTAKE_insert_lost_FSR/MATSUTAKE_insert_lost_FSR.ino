#include <ArduinoHttpClient.h>
#include <CytronWiFiShield.h>
#include <CytronWiFiClient.h>
#include <CytronWiFiServer.h>
#include <SoftwareSerial.h>

    
const char* ssid     = "Embedded_2G";
const char* password = "32473242";
const char* host = "220.149.236.71";
//for WiFi Connection
int led = 13;                       
int r = A0;  
int l = A1; 
int seatNo1 = 0; //A0
int seatNo2 = 1; //A1

//for FSR sensor

void setup() {
  Serial.begin(9600);
  delay(100);
  pinMode(led, OUTPUT); 
  
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);


 while (!wifi.begin(2, 3)) {
    delay(500);
    Serial.print(".");
  }  
  while(!wifi.connectAP(ssid, password))
  {
    Serial.println(F("Error connecting to WiFi"));
    Serial.print(".");
  } 

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(wifi.localIP()); 
}

void loop() {

  ESP8266Client client;
  const int httpPort = 80;
  int rv = analogRead(r);
  int lv = analogRead(l);
  int sendmessage=0;
  Serial.println("r = " + String(rv) + " l = " + String(lv));
  
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  int i=5000;
  while (client.available()<=0&&i--)
  {
    delay(1);
    if(i==1) {
      Serial.println(F("Timeout"));
      //client.stop();
      //return;
      }
  }
  if(rv > 10 and lv >10){
    sendmessage=3; //11
  }else if (rv > 10){
    sendmessage=2; //10
    }
    else if (lv >10){
      sendmessage=1; //01
      }

  if(sendmessage){
      String data = "theaterId="; 
     //buffer.append("theaterId").append("=").append(0).append("&");                 // php 변수에 값 대입 
     data.concat(0);
     data.concat("&");
     //buffer.append("seatNo").append("=").append("1234).append("&");   // php 변수 앞에 '$' 붙이지 않는다 
    data.concat("seatNo=");
    if(sendmessage & 2){
       data.concat(String(0));
       sendmessage = sendmessage & 1;
    }else{
      data.concat(String(1));
      sendmessage = sendmessage & 0;
      }
    data.concat("&");
     //buffer.append("isLost").append("=").append(1).append("&");
     data.concat("isLost=");
     data.concat(String(1));
     Serial.print("Requesting POST: "+data);
     // Send request to the server:
     if(client.connect(host, 80)){
       client.println("POST http://220.149.236.71/insert_lost_post.php HTTP/1.1");
       client.println("Host: 220.149.236.71");
       client.println("Content-Type: application/x-www-form-urlencoded");
       client.print("Content-Length: ");
       client.println(data.length());
       client.println();
       client.print(data);
       client.flush();
     
        char c = client.read();
        Serial.print(c);
        Serial.println("Client Avaliable");
     }else{
        Serial.println("Unable to Connect");
  
      }
  }
  
     delay(500); // Can be changed
    if (client.connected()) { 
      client.stop();  // DISCONNECT FROM THE SERVER
      Serial.println("DISCONNECT FROM THE SERVER");
  
    }
  Serial.println();
  Serial.println("closing connection");
  delay(50000);
}
