
/*
 * Jag har skapat hastighetsmätare som kan mäta hastigheten som ett 
 * objekt rör sig i. Hastighetsmätaren kan sätaspå och stengasav. Man 
 * kan också ändra på vilket "mode" som den är insteld på. De första 
 * läget skrivr ut hastigheten i realtid medans de andra läget skriver
 * ut de största värdet som den har fåtfram. Vär man stengerav seter på 
 * ändrar läge så spelas de också up olika djud som indikerar ens
 * handling. De skrivs även ut en text på skärmen som beskriver ens 
 * ändring. Detta är för att göra hastighetsmätaren mer lätt användlig.
 */

 
/*
 * Här includerar jag alla biblotek som används och raden underskriver
 * jag in info om vilken oledskärm de är jag använder till daton.
 */ 
#include <U8glib.h> 

U8GLIB_SSD1306_128X64 oled(U8G_I2C_OPT_NO_ACK); 

//här initsierar jag alla variabler som används

int trigPin = 11;    
int echoPin = 12;    

float cm1;
float cm2;
float hastighetms;
float maxhastighet = 0;

int toggleOnOff = 0;
int toggleMode = 0;

int speakerPin = 8;

//I void setup beskriver man för arduinot vilka pins som ska användas och åt
//vad, om den ska ta in datta(input) eller geut(output). I setup intruduserar
//jag också vlken font som oled skärmen ska visa.

void setup() {
  oled.setFont(u8g_font_helvB10);
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(2,INPUT);
  pinMode(4,INPUT);
  pinMode(speakerPin, OUTPUT);
}

void loop() {

  if ( OnOffSwitch() == 1) { // här sktiver jag att om onoffswitch ger värde 1 
                             // så ska pogramet sätas på annars hender ingenting.
    if ( ModeSwitch() == 1){ // här sktiver jag att om ModeSwitch är lika med 1 så ska pogramet för
                             // maxhastighet vara på men om värdet är något annat så ska pogramet för
                             //hastighet varapå.
/*
 * i denär delen använder jag funktionen "hastighet" för att tareda 
 * på hastigheten i stunden. Sedan jämför jag mitt nya värde med 
 * största värdet jag har fåt in. Om mitt nya värde är störe än 
 * mitt föra kommer den att ersäta och printa ut de nya värdet på 
 * skermen.
 */
      hastighetms = hastighet();

      if (abs(hastighetms) > abs(maxhastighet)) {
        maxhastighet = hastighetms;
      }
      
      Serial.print(maxhastighet);
      Serial.print("m/s");
      Serial.println();
 
      updateOled(String(String(maxhastighet) + " m/s"));
  
    }
    else {
/*
 * I denhär delen ber jag funktionen "hastighet" att ge mig des värde
 * nu i stnden och sedan printar den ut de på skärmen.
 */
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

/*
 *funktionen nedan (OnOffSwitch) börjar med att kolla om den har fått
 *en singnal. Om den har fått en singnal så så kollar den vilket värde
 *som variablen (toggleOnOff) har (antingen 1 eller 0). Om variablen 
 *har värdet 1 så kommer den slåöver värdet till 0 och samma sak tvärt
 *om. Om funktionen slåröver från 1 till 0 så updaterar den oledsärmen
 *till att det står off och ändrar även variablen toggleMode till 0
 *detta gör så att hastighetsmätaren alltid är på ett ovh samma läge 
 *när den säts på. Det spelar också en viss melodi när den går till
 *off. När toggleOnOff går från 0 till 1 spellas det samma melodi fast 
 *baklänges. 
 */
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
// funktionen ModeSwitch aggerar väldigt likt OnOffSwitch.
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
/*
 * funktionen hastighet tar in två stycken värden från fuktonen avstand
 * och gör om värdena till två variabler (cm1 , cm2). dett fins ett 
 * delay mellan cm 1 och cm2 på 200 millisekunder. Sän tar daton och 
 * delar skilnaden i streka på tiden och får dör med fram hastigheten.
 * göra systemet mer nogrant så att den kan mäta högre och lägre hastigheter 
 * bätre så kan man öka eller minska värdet på delay.
 */
float hastighet() {
  
  cm1 = avstand();
  delay(200);
  cm2 = avstand();

  return ((cm1 - cm2) / 100.0) / 0.2;
}
/*
 * funktionen avstand mäter avståndet mellan ultradjudsensorn och 
 * nermaste objekt. Detta gör den genom att den ber trigpin att skika
 * ut en singnal. Sedan mäter ecoPin hur lång tid det tar för 
 * singnalen att komma tilbaks. Sen delar man tiden med två i och med
 * att ljudet åker både fram och tilbaks. Slutligen delar man de med 
 * ljudets hastighet för att få avståndet.
 */
long avstand() {
  long duration;
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
 
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
 
  return (duration/2) * 0.0343;  
}
// Den här funktionen updaterar särmen och bestemer var texten ska skrivas.
void updateOled(String text){
  oled.firstPage();
  do{
    oled.drawStr(1, 30, text.c_str());
  } while ( oled.nextPage() );
}
