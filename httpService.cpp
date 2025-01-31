#include <WiFi.h>
#include <HTTPClient.h>
#include "httpService.h"

httpService::httpService(char* ssid, char* password, char* service){
  this->ssid=ssid;
  this->password=password;
  this->serviceURI=service;
}

void httpService::connectToWifi(char* ssid, char* password){
  WiFi.begin(ssid, password);
  Serial.println("Connecting");
  while(WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());
}

void httpService::on() { 
  connectToWifi(this->ssid, this->password);
}
//va aggiunto la luminosità
int httpService::sendData(String address, float temp,float light, String place){  
  
   HTTPClient http;    
   http.begin(address + "/api/data");      
   http.addHeader("Content-Type", "application/json");    
    
   String msg = 
    String("{ \"value\": ") + String(temp) + 
    ", \"place\": \"" + place +"\" }";
   
   int retCode = http.POST(msg);   
   String msg = String("{ \"value\": ") + String(light) + ", \"place\": \"" + place +"\" }";
   
   int retCode = http.POST(msg);
   http.end();  
      
   return retCode;
}

void httpService::send(int t, int l) {
  if (WiFi.status()== WL_CONNECTED){      

    int value = random(15,20);
    int code = sendData(serviceURI,t ,l ,"home");
    if (code == 200){
       Serial.println("Ok");   
     } else {
       Serial.println(String("Error: ") + code);
     }
    
    delay(5000);

  } else {
    Serial.println("WiFi Disconnected... Reconnect.");
    //connectToWifi(ssid, password);
  }
}
