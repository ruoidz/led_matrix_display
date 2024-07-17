#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include "DHT.h"
#include "RTClib.h"

#include <stdio.h>
#include <stdint.h>
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
#define MAX_DEVICES 4 // 4 blocks
#define CS_PIN 17
#define DHTPIN 4     // Digital pin connected to the DHT sensor
// Feather HUZZAH ESP8266 note: use pins 3, 4, 5, 12, 13 or 14 --
// Pin 15 can work but DHT must be disconnected during program upload.
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11   // DHT 11
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
// create an instance of the MD_Parola class

MD_Parola ledMatrix = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);
DHT dht(DHTPIN, DHTTYPE);
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

void setup() {
  Serial.begin(9600);
  pinMode(15, OUTPUT);
  Serial.println(F("DHTxx test!"));
  dht.begin();
  //RTC setup
  #ifndef ESP8266
  while (!Serial); // wait for serial port to connect. Needed for native USB
#endif

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    // When time needs to be set on a new device, or after a power loss, the
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
     //rtc.adjust(DateTime(2014, 5, 29, 15, 20, 0));
  }

  // When time needs to be re-set on a previously configured device, the
  // following line sets the RTC to the date & time this sketch was compiled
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  // This line sets the RTC with an explicit date & time, for example to set
  // January 21, 2014 at 3am you would call:
  //rtc.adjust(DateTime(2024, 5, 29, 15, 20, 0));

  ledMatrix.begin();         // initialize the LED Matrix
  ledMatrix.setIntensity(0); // set the brightness of the LED matrix display (from 0 to 15)
  ledMatrix.displayClear();  // clear LED matrix display
  attachInterrupt(13, count, FALLING);
}
int n=0;
float t=0;
long long buttonPressedTime = 0;
int buttonFlag = 0;
char curMessage[30] = { "" };
void  count()
{
  buttonFlag = 1;
  buttonPressedTime = millis();
}

void buttonProcess(){
  if(buttonFlag == 1){
    while(digitalRead(13) == LOW){
      Serial.println();
      //break;
    }
    if(millis() - buttonPressedTime > 20){
      n++;
      if(n>3)n = 0;
    }
  }
  buttonFlag = 0;
}

void loop() {
  DateTime now = rtc.now();
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }
  buttonProcess();
  // ledMatrix.setTextAlignment(PA_LEFT);
  // ledMatrix.print("Left"); // display text
  // delay(2000);
  Serial.println(n);
  if(n==0)
  {
    displayDate();
  }
  if(n==1)
  {
    displayTime();
  }
  if(n==2)
  {
    displayTemp();
  }
  if(n==3)
  {
    
    displayName();
  }
  led();
  // ledMatrix.setTextAlignment(PA_RIGHT);
  // ledMatrix.print("Right"); // display text
  // delay(2000);

  // ledMatrix.setTextAlignment(PA_CENTER);
  // ledMatrix.setInvert(true);
  // ledMatrix.print("Invert"); // display text inverted
  // delay(2000);

  // ledMatrix.setInvert(false);
  // ledMatrix.print(1234); // display number
}
void displayName()
{
  //ledMatrix.displayClear();
  if (ledMatrix.displayAnimate()){
    ledMatrix.displayText("Linh va Minh", PA_LEFT, 25, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
}
void displayDate()
{
  //ledMatrix.displayClear();
  char char_array[20];
  DateTime now = rtc.now();
  String date = String(now.year()) +'.'+ String(now.month()) +'.'+ String(now.day())+ '.' + String(daysOfTheWeek[now.dayOfTheWeek()]);
  date.toCharArray(char_array, 20);
  strcpy(curMessage, char_array);
  if (ledMatrix.displayAnimate()){
    ledMatrix.displayText(curMessage, PA_LEFT, 50, 0, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  //ledMatrix.setTextAlignment(PA_CENTER);
  //ledMatrix.print(date); // display text
}

void displayTime()
{
  DateTime now = rtc.now();
  String time = String(now.hour()) +'.'+ String(now.minute()) +'.'+String(now.second());
  ledMatrix.setTextAlignment(PA_LEFT);
  ledMatrix.print(time); // display text
}
void displayTemp()
{
  String a=String(t);
  ledMatrix.setTextAlignment(PA_CENTER);
  ledMatrix.print(a); // display text
  Serial.println(t);
}
void led()
{
  if(digitalRead(14)==0)
  {
    digitalWrite(15, HIGH);
  }
  else
  {
    digitalWrite(15,LOW);
  }
}