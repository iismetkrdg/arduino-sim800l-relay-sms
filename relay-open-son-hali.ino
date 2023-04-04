#include <SoftwareSerial.h>

//Create software serial object to communicate with SIM800L
SoftwareSerial mySerial(2,3); //SIM800L Tx & Rx is connected to Arduino #3 & #2

int durum1 = 0;
int durum2 = 0;
int tekrar = 0;
void xyz()
{
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  digitalWrite(6,HIGH);
  digitalWrite(5,HIGH);
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
  //Begin serial communication with Arduino and SIM800L
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(4000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  mySerial.println("AT+CSQ"); //Signal quality test, value range is 0-31 , 31 is the best
  updateSerial();
  mySerial.println("AT+CCID"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CREG?"); //Check whether it has registered in the network
  updateSerial();
  mySerial.println("AT+CLIP=1"); //Read SIM information to confirm whether the SIM is plugged
  updateSerial();
  mySerial.println("AT+CMGF=1"); // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CNMI=1,2,0,0,0"); // Decides how newly arrived SMS messages should be handled
  updateSerial();
  tekrar = 1;
}
void setup()
{     
  xyz();
  
}

void loop()
{
  if(tekrar != 1)
  {
    xyz();
  }
  
  updateSerial();
}

void updateSerial()
{
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    String gelen = mySerial.readString();
    Serial.println(gelen);
    if(gelen.indexOf("kayitli numara")!=-1)
    {
      if(gelen.indexOf("role1ON")!=-1){
        if (durum1==0)
        {
          durum1=1;
          digitalWrite(6,LOW);
          delay(1000);
        };
      }
      if(gelen.indexOf("role1OFF")!=-1){
        if (durum1==1)
        {
          durum1=0;
          digitalWrite(6,HIGH);
          delay(1000);
        };
      }
      if(gelen.indexOf("kapi")!=-1){
          digitalWrite(5,LOW);
          delay(1000);
          digitalWrite(5,HIGH);
      }
      
    };
    if(gelen.indexOf("yenino")!=-1)
    {
      String numara = gelen.substring(gelen.indexOf("yenino")+12,gelen.indexOf("."));
      mySerial.println("AT+CPBW=,\""+numara+"\",129,\"kayitli numara\"");
    }
    
    
  }
}
