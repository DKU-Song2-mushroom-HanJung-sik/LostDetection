#include <ArduinoHttpClient.h>
#include <CytronWiFiShield.h>
#include <CytronWiFiClient.h>
#include <CytronWiFiServer.h>
#include <SoftwareSerial.h>

#define TRIG_r1 9 //TRIG 핀 설정 (초음파 보내는 핀)
#define ECHO_r1 8 //ECHO 핀 설정 (초음파 받는 핀)
#define TRIG_r2 7
#define ECHO_r2 6
//PIN for 

const char* ssid     = "Embedded_2G";
const char* password = "32473242";
const char* host = "220.149.236.71";
//HttpClient client = HttpClient(wifi, host, 80);

void setup() {
  Serial.begin(9600);
  delay(100);
  
  pinMode(TRIG_r1, OUTPUT);
  pinMode(ECHO_r1, INPUT);
  pinMode(TRIG_r2, OUTPUT);
  pinMode(ECHO_r2, INPUT);
  //ULT PIN setup


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
  long duration_r1, distance_r1;
  long duration_r2, distance_r2;
  ESP8266Client client;
  const int httpPort = 80;
  int sendmessage = 0;
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

  ///
  ///Here we Receving Losts by Ultra-Sonic Sensor
  ///
  //Receive Lost from Seat1
  digitalWrite(TRIG_r1, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_r1, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_r1, LOW);
  
    duration_r1 = pulseIn (ECHO_r1, HIGH); 
  distance_r1 = duration_r1 * 17 / 1000;
//Receive Lost from Seat 2

    digitalWrite(TRIG_r2, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_r2, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_r2, LOW);
  
  duration_r2 = pulseIn (ECHO_r2, HIGH); 
  distance_r2 = duration_r2 * 17 / 1000;

  if(distance_r1<10 && distance_r2<10){
    sendmessage=3; //11
    }else if(distance_r1<10){
      sendmessage =2; //10
      }
      else if(distance_r2<10){
        sendmessage=1;  //01
      }

  if(sendmessage){
      String data = "theaterId="; 
     //buffer.append("theaterId").append("=").append(0).append("&");                 // php 변수에 값 대입 
     data.concat(0);
     data.concat("&");
     //buffer.append("seatNo").append("=").append("1234).append("&");   // php 변수 앞에 '$' 붙이지 않는다 
     data.concat("seatNo=");
     
     if(sendmessage & 2){
      data.concat(String(3));
      sendmessage = sendmessage&1;
     }
     else if(sendmessage & 1) {
      data.concat(String(4));
      sendmessage = sendmessage&0;
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
  
     delay(500); // Can be changed
    if (client.connected()) { 
      client.stop();  // DISCONNECT FROM THE SERVER
      Serial.println("DISCONNECT FROM THE SERVER");
  
    }
  }
  Serial.println();
  Serial.println("closing connection");
  delay(5000);
}
