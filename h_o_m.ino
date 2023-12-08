/*
   created by Rui Santos, https://randomnerdtutorials.com

   Complete Guide for Ultrasonic Sensor HC-SR04

    Ultrasonic sensor Pins:
        VCC: +5VDC
        Trig : Trigger (INPUT) - Pin11
        Echo: Echo (OUTPUT) - Pin 12
        GND: GND
*/
#include <U8glib.h>

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK);

int trigPin = 11;    // Trigger
int echoPin = 12;    // Echo

float cm1;
float cm2;
float hastighetms;
float maxhastighet = 0;

int toggleOnOff = 0;
int toggleMode = 0;

int speakerPin = 8;

void setup() {
  oled.setFont(u8g_font_helvB10);
  //Serial Port begin
  Serial.begin (9600);
  //Define inputs and outputs
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(2,INPUT);
  pinMode(4,INPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop() {

  if ( OnOffSwitch() == 1) {
    if ( ModeSwitch() == 1){
      
      hastighetms = hastighet();

      if (abs(hastighetms) > abs(maxhastighet)) {
        maxhastighet = hastighetms;
      }
      
      Serial.print(maxhastighet);
      Serial.print("m/s");
      Serial.println();
 
      updateOled(String(String(maxhastighet) + " s/m"));
  
    }
    else {
      
      hastighetms = hastighet();
      
      Serial.print(hastighetms);
      Serial.print("m/s");
      Serial.println();
 
      updateOled(String(hastighetms) + " m/s ");
    }
  }
  else {
    updateOled("off");
  }
}

int OnOffSwitch() {
  if (digitalRead(2)== 1){
    if (toggleOnOff == 1){
      toggleOnOff = 0;
      updateOled("Off");
      toggleMode = 0;

      tone(speakerPin, 1300);
      delay(200);
      tone(speakerPin, 1100);
      delay(200);
      tone(speakerPin, 900);
      delay(200);
      tone(speakerPin, 700);
      delay(200);
      tone(speakerPin, 500);
    }
    else{
      toggleOnOff = 1;
      updateOled("On");
      
      tone(speakerPin, 500);
      delay(200);
      tone(speakerPin, 700);
      delay(200);
      tone(speakerPin, 900);
      delay(200);
      tone(speakerPin, 1100);
      delay(200);
      tone(speakerPin, 1300);
    }
    delay(200);
    noTone(speakerPin);
  }
  return toggleOnOff;
}

int ModeSwitch(){
  if (digitalRead(4)== 1) {
    if (toggleMode == 1) {
      toggleMode = 0;
      updateOled("Mode: Hastighet");
      tone(speakerPin, 1500);
    }
    else{
      toggleMode = 1;
      updateOled("Mode: Max-Hastighet");
      maxhastighet = 0;
      tone(speakerPin, 300);
    }
    delay(200);
    noTone(speakerPin);
    delay(800);
  }
  return toggleMode;
}

float hastighet() {
  
  cm1 = avstand();
  delay(200);
  cm2 = avstand();

  return ((cm1 - cm2) / 100.0) / 0.2;
}

long avstand() {
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  return (duration/2) * 0.0343;     // Divide by 29.1 or multiply by 0.0343
}

void updateOled(String text){
  oled.firstPage();
  do{
    oled.drawStr(1, 30, text.c_str());
  } while ( oled.nextPage() );
}
