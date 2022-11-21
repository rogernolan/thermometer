
#include "Firebase_Arduino_WiFiNINA.h"
#include "Arduino_JSON.h"
#include "arduino_secrets.h"

char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
char fb_key[] = FIREBASE_KEY;
char db_name[] = "orangevan-e8032-default-rtdb.europe-west1.firebasedatabase.app";

FirebaseData fbdo;

void setup() {

  Serial.begin(9600);
  while (!Serial);  // loop on serial waitingfor it to connect.

  // Connect to Firebase:
  Firebase.begin(db_name, fb_key, ssid, pass);
  Firebase.reconnectWiFi(true);

  Serial.println("----------------------------------------");

}

int time = 0;

void loop() {
  delay(1000);

  String contentType = "application/json";

  JSONVar myObject;

  time++;
  int day = time / 10;
  int hour = time % 10;
  float temp = random(-100,350) / 10.0;

  myObject["timestamp"] = hour;
  myObject["temperature"] = temp;

  String jsonString = JSON.stringify(myObject);

  Serial.print("Writing JSON: ");
  Serial.println(jsonString);

  //https://orangevan-e8032-default-rtdb.europe-west1.firebasedatabase.app/
  char pathString[24];
  sprintf(pathString,"temp/denby-daily-%d",day);
  
  Serial.print("to: ");
  Serial.println(pathString);

  if( Firebase.pushJSON(fbdo, pathString, jsonString)) 
  {

   //Database path to be appended
    Serial.println(fbdo.dataPath()); //Should be "/test/append"

    //New created key/name
    Serial.println(fbdo.pushName());

    //Absolute path of new appended data
    Serial.println(fbdo.dataPath() + "/"+ fbdo.pushName());

  } else {
    //Failed, then print out the error detail
    Serial.println(fbdo.errorReason());
  }

}
