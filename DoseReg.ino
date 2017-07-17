#include <Keypad.h>
#include <Wire.h>
#include <DS3231.h>
#include <string.h>//for the strings...jyess

// Paint example specifically for the TFTLCD breakout board.
// If using the Arduino shield, use the tftpaint_shield.pde sketch instead!
// DOES NOT CURRENTLY WORK ON ARDUINO LEONARDO

// Modified for SPFD5408 Library by Joao Lopes
// Version 0.9.2 - Rotation for Mega

// *** SPFD5408 change -- Begin
#include <DS3231.h>
#include <SPFD5408_Adafruit_GFX.h>    // Core graphics library
#include <SPFD5408_Adafruit_TFTLCD.h> // Hardware-specific library
#include <SPFD5408_TouchScreen.h>
// *** SPFD5408 change -- End

#if defined(__SAM3X8E__)
#undef __FlashStringHelper::F(string_literal)
#define F(string_literal) string_literal
#endif
void(* resetFunc) (void) = 0; //declare reset function @ address 0
int ldec = 0;
// When using the BREAKOUT BOARD only, use these 8 data lines to the LCD:
// For the Arduino Uno, Duemilanove, Diecimila, etc.:
//   D0 connects to digital pin 8  (Notice these are
//   D1 connects to digital pin 9   NOT in order!)
//   D2 connects to digital pin 2
//   D3 connects to digital pin 3
//   D4 connects to digital pin 4
//   D5 connects to digital pin 5
//   D6 connects to digital pin 6
//   D7 connects to digital pin 7

// For the Arduino Mega, use digital pins 22 through 29
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 22
//   D1 connects to digital pin 23
//   D2 connects to digital pin 24
//   D3 connects to digital pin 25
//   D4 connects to digital pin 26
//   D5 connects to digital pin 27
//   D6 connects to digital pin 28
//   D7 connects to digital pin 29

// For the Arduino Due, use digital pins 33 through 40
// (on the 2-row header at the end of the board).
//   D0 connects to digital pin 33
//   D1 connects to digital pin 34
//   D2 connects to digital pin 35
//   D3 connects to digital pin 36
//   D4 connects to digital pin 37
//   D5 connects to digital pin 38
//   D6 connects to digital pin 39
//   D7 connects to digital pin 40

DS3231 clock;
RTCDateTime dt;

#include <Keypad.h>
#define YP A1  // must be an analog pin, use "An" notation!
#define XM A2  // must be an analog pin, use "An" notation!
#define YM 7   // can be a digital pin
#define XP 6   // can be a digital pin

// Original values
//#define TS_MINX 150
//#define TS_MINY 120
//#define TS_MAXX 920
//#define TS_MAXY 940

// Calibrate values
#define TS_MINX 125
#define TS_MINY 85
#define TS_MAXX 965
#define TS_MAXY 905
const byte ROWS = 4;
const byte COLS = 4;
char buffer;
char keys[ROWS][COLS] =
{
  {'1', '2', '3', NO_KEY},
  {'4', '5', '6', NO_KEY},
  {'7', '8', '9', NO_KEY},
  {NO_KEY, '0', NO_KEY, '*'},

};

byte rowPins[ROWS] = {34, 36, 38, 40}; //connect to row pinouts
byte colPins[COLS] = {42, 44, 46, 48}; //connect to column pinouts

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );


// For better pressure precision, we need to know the resistance
// between X+ and X- Use any multimeter to read it
// For the one we're using, its 300 ohms across the X plate
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

#define LCD_CS A3
#define LCD_CD A2
#define LCD_WR A1
#define LCD_RD A0
// optional
#define LCD_RESET A4

// Assign human-readable names to some common 16-bit color values:
#define  BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF


Adafruit_TFTLCD tft(LCD_CS, LCD_CD, LCD_WR, LCD_RD, LCD_RESET);
int pizza = 0;
#define BOXSIZE 40
#define PENRADIUS 3
int oldcolor, currentcolor;






int keyret2 = 0;
int hourtwo;
int mintwo;
int sTime;
int keyret = 0;
int hour;
int minute;
int fTime;

void alarmget(int &hora, int &minu, int &tiem) {
  int hour1;
  int hour2;
  int min1;
  int min2;
  int tiem1;
    while (1 == 1) {
    if (keyret == 0) {
      char key = keypad.getKey();
      if (key != NO_KEY)
      {
        if (key == '*') {
          resetFunc();
        }
        else {
          Serial.println(key);
          hour1 = (int)key - 48;
          keyret = 1;
          Serial.println(hour1);
        }
      }
    }
    else if (keyret == 1) {
      char key2 = keypad.getKey();
      if (key2 != NO_KEY)
      {
        if (key2 == '*') {
          resetFunc();
        }
        else {
          Serial.println(key2);
          hour2 = (int)key2 - 48;
          Serial.println(hour2);
          keyret = 2;
        }
      }
    }
    else if (keyret == 2) {
      char key3 = keypad.getKey();
      if (key3 != NO_KEY)
      {
        if (key3 == '*') {
          resetFunc();
        }
        else {
          Serial.println(key3);
          min1 = (int)key3 - 48;
          Serial.println(min1);
          keyret = 3;
        }
      }
    }
    else if (keyret == 3) {
      char key4 = keypad.getKey();
      if (key4 != NO_KEY)
      {
        if (key4 == '*') {
          resetFunc();
        }
        else {
          Serial.println(key4);
          min2 = (int)key4 - 48;
          Serial.println(min2);
          keyret = 4;
        }
      }
    }
    else if (keyret == 4) {
      char key5 = keypad.getKey();
      if (key5 != NO_KEY)
      {
        if (key5 == '*') {
          resetFunc();
        }
        else {
          Serial.println(key5);
         tiem1 = (int)key5 - 48;
 
          keyret = 0;
          break;
        }
      }
    }
  }
  hora = (hour1 * 10) + hour2;
  minu = (min1 * 10) + min2;
  if (hora > 24 || hora < 0) {
    resetFunc();
  }
  if (minu > 59 || minu < 0) {
    resetFunc();
  }
  tiem = tiem1;
  Serial.println(hora);
  Serial.println(minu);
   Serial.println(tiem);
}




int stopper = 0;
int done = 0;
void hello() {
  // put your main code here, to run repeatedly:
  //digitalWrite(13, HIGH);
  //delay(2000);
  digitalWrite(49, 0);
  
  while (1==1) {
    Serial.print("spin on");
    digitalWrite(49, LOW);
    for ( int i =0; i < 2000; i++) {
      delay(1);
    if (digitalRead(47) == 0) {
      digitalWrite(49, HIGH);
      done = 1;
      break;
    }
   }
   if(done==1){
    Serial.print("breaking one"+(digitalRead(47)));
    break;
   }
    digitalWrite(49, HIGH);
     Serial.print("spin off");
   // delay(2000);
   for ( int i =0; i < 2000; i++) {
      delay(1);
    if (digitalRead(47) == 0) {
      digitalWrite(49, HIGH);
      done = 1;
      break;
    }
   }
   if(done==1){
    Serial.print("breaking two"+(digitalRead(47)));
    break;
   }

    
    Serial.print("sole on");
    digitalWrite(51, LOW);
    for ( int i =0; i < 2000; i++) {
      delay(1);
    if (digitalRead(47) == 0) {
      digitalWrite(49, HIGH);
      digitalWrite(51, HIGH);
      done = 1;
      break;
    }
   }
   if(done==1){
    Serial.print("breaking three"+(digitalRead(47)));
    break;
   }
    Serial.print("sole off");
    digitalWrite(51, HIGH);

    for ( int i =0; i < 2000; i++) {
      delay(1);
    if (digitalRead(47) == 0) {
      digitalWrite(49, HIGH);
      digitalWrite(51, HIGH);
      done = 1;
      break;
    }
   }
   if(done==1){
    Serial.print("breaking four"+(digitalRead(47)));
    break;
   }
   
    /*
    delay(2000);
    
    if (digitalRead(47) == 0) {
      pizza = 1;
      Serial.print(pizza);
      digitalWrite(49, HIGH);
      break;
    }
    
    Serial.print("sole off");
    digitalWrite(51, HIGH);
    delay(2000);
    if (digitalRead(47) == 0) {
      pizza = 1;
      Serial.print(pizza);
      digitalWrite(49, HIGH);
      break;
    }

    */
    
    stopper = stopper + 1;
    if (stopper >= 5) {
      tft.setCursor(75, 200);
      tft.setTextSize (2);
      tft.setTextColor(BLACK);
      digitalWrite(49, 1);
      digitalWrite(32, HIGH);
      delay(750);
      digitalWrite(32, LOW);
      drawBorder(RED);

      tft.println("Error, Check ");
      tft.println(" containment units.");
      tft.println("");
      tft.println(" Touch to Restart");
      waitOneTouch();
      resetFunc();
      break;
    }
  }
  Serial.println("ALARM 1 exiting loop!");
  digitalWrite(49, HIGH);
  digitalWrite(51, HIGH);
}


//COOL THING MADE BY GUS TO MAKE DRAWING TO THE LCD MUCH EASIER... BASICARRY JUST A FUNCTION, NO HARDCIDIGO
void LCDraw(float posX, float posY, float txtSize, float txtColour, String whatToSay){
  tft.setCursor (posX, posY);//places the text at the specific position. 
  tft.setTextSize (txtSize);//uses a simple integer for the text size, usually 2
  tft.setTextColor(txtColour);//uses the string for the colour, like GREEN, BLACK< BLUE, etc. 
  tft.println(whatToSay);//outputs what the LCd is going to say
}

void setup(void) {

  pinMode(32, OUTPUT);
  digitalWrite(49, 1);
  digitalWrite(45, 1);
  Serial.begin(9600);
  Serial.println(F("Paint!"));
  Serial.println(F("Initialize DS3231"));
  clock.begin();
  tft.reset();
  pinMode(45, OUTPUT);
  pinMode(49, OUTPUT); //spin
  pinMode(51, OUTPUT); //pow
  digitalWrite(51,HIGH);
  pinMode (47, INPUT); //Sensor output
  // *** SPFD5408 change -- Begin
  //  uint16_t identifier = tft.readID();
  //
  //  if(identifier == 0x9325) {
  //    Serial.println(F("Found ILI9325 LCD driver"));
  //  } else if(identifier == 0x9328) {
  //    Serial.println(F("Found ILI9328 LCD driver"));
  //  } else if(identifier == 0x7575) {
  //    Serial.println(F("Found HX8347G LCD driver"));
  //  } else if(identifier == 0x9341) {
  //    Serial.println(F("Found ILI9341 LCD driver"));
  //  } else if(identifier == 0x8357) {
  //    Serial.println(F("Found HX8357D LCD driver"));
  //  } else {
  //    Serial.print(F("Unknown LCD driver chip: "));
  //    Serial.println(identifier, HEX);
  //    Serial.println(F("If using the Adafruit 2.8\" TFT Arduino shield, the line:"));
  //    Serial.println(F("  #define USE_ADAFRUIT_SHIELD_PINOUT"));
  //    Serial.println(F("should appear in the library header (Adafruit_TFT.h)."));
  //    Serial.println(F("If using the breakout board, it should NOT be #defined!"));
  //    Serial.println(F("Also if using the breakout, double-check that all wiring"));
  //    Serial.println(F("matches the tutorial."));
  //    return;
  //  }
  //
  //  tft.begin(identifier);
  //clock.setDateTime(2017, 5, 17, 17, 20, 0);
  clock.armAlarm1(false);
  clock.clearAlarm1();

  clock.armAlarm2(false);
  clock.clearAlarm2();

  tft.begin(0x9325); // SDFP5408
  pinMode(49, OUTPUT);
  tft.setRotation(0); // Need for the Mega, please changed for your choice or rotation initial


  // Border

  drawBorder(RED);

  // Initial screen

  tft.setCursor (55, 50);
  tft.setTextSize (3);
  tft.setTextColor(RED);
  tft.println("DOSEREG");
  tft.setCursor (90, 85);
  tft.println("GUN");
  tft.setCursor (20, 120);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println("Powered by DoseOS");
  tft.setCursor (80, 250);
  tft.setTextSize (1);
  tft.setTextColor(BLACK);
  tft.println("Touch to proceed");

  // Wait touch

  waitOneTouch();

  // *** SPFD5408 change -- End

  // -- End
  dt = clock.getDateTime();
  // Paint
  drawBorder(BLUE);

  // tft.text()
  LCDraw(10, 10, 2, BLACK, "What time do you     want your dose?");//should be identical to whats below. 
  /*tft.setCursor (10, 10);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println("What time do you     want your dose?");
  */
  LCDraw(70, 45, 2, BLACK, "Alarm 1");
  LCDraw(40, 80, 2, BLACK, "Hour");
  LCDraw(120, 80, 2, BLACK, "Min");
  LCDraw(175, 80, 2, BLACK, "#");
  LCDraw(70, 125, 2, BLACK, "Alarm 2");
  LCDraw(40, 160, 2, BLACK, "Hour 2");
  LCDraw(160, 160, 2, BLACK, "Min 2");
 LCDraw(5, 220, 1.5, BLACK, " Enter Your Second Time Then Touch \n             Once.");
  //full refresh of LCD


  tft.setCursor (40, 245);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.print("Current time: ");
  tft.setCursor (80, 275);
  tft.print(dt.hour);
  tft.print(":");
  tft.print(dt.minute);

  tft.setCursor (10, 70);
  tft.setTextSize (2);
  tft.setTextColor(WHITE);
  alarmget(hour, minute, fTime);
if(hour == 24){
  hello();//immediately manual start the cool thing
  }
  tft.setCursor (45, 110);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(hour);

  tft.setCursor (165, 110);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(minute);
  waitOneTouch();
  drawBorder(BLUE);

  // tft.text() AFTER FULL REFRESH
  LCDraw(10, 10, 2, BLACK, "What time do you     want your dose?");
  LCDraw(70, 45, 2, BLACK, "Alarm 1");
  LCDraw(40, 80, 2, BLACK, "Hour");
  LCDraw(120, 80, 2, BLACK, "Min");
LCDraw(175, 80, 2, BLACK, "#");
  LCDraw(70, 125, 2, BLACK, "Alarm 2");
  LCDraw(40, 160, 2, BLACK, "Hour 2");
  LCDraw(160, 160, 2, BLACK, "Min 2");
  LCDraw(5, 220, 1.5, BLACK, " Enter Your Second Time Then Touch \n             Once.");


  tft.setCursor (40, 265);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.print("Current time: ");
  tft.setCursor (80, 295);
  tft.print(dt.hour);
  tft.print(":");
  tft.print(dt.minute);

  tft.setCursor (10, 70);
  tft.setTextSize (2);
  tft.setTextColor(WHITE);


  tft.setCursor (45, 110);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(hour);

  tft.setCursor (165, 110);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(minute);
  tft.setCursor (170, 110);
  tft.setTextSize (1.5);
  tft.setTextColor(BLACK);
  tft.setCursor (195, 110);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(fTime);
  alarmget(hourtwo, mintwo, sTime);
  tft.setCursor (45, 190);

  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(hourtwo);

  tft.setCursor (165, 190);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(mintwo);
  tft.setCursor (195, 190);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println(sTime);
  Serial.println(F("bout to call loop"));
  Serial.println(hour);
  Serial.println(minute);
  clock.setAlarm1(0, hour, minute, 0, DS3231_MATCH_H_M_S);
  clock.setAlarm2(0, hourtwo, mintwo, DS3231_MATCH_H_M);
  waitOneTouch();
  tft.setCursor (35, 250);
  tft.setTextSize (2);
  tft.setTextColor(BLACK);
  tft.println("Alarms Armed!.");

  waitOneTouch();
  checkAlarms();
  yo();
  delay(1000);
  waitOneTouch();
  resetFunc();
  pinMode(13, OUTPUT);

}
void loop() {
}

#define MINPRESSURE 10
#define MAXPRESSURE 1000
void yo() {
  boolean run1 = false;
  boolean run2 = false;
  while ((run1 == false) || (run2 == false)) {

    dt = clock.getDateTime();
    Serial.println(clock.dateFormat("d-m-Y H:i:s - l", dt));
    if (clock.isAlarm1())
    {
      Serial.println("ALARM 1 TRIGGERED!");
      digitalWrite(32, HIGH);
      delay(500);
      digitalWrite(32, LOW);
      for(int i = 1; i<=fTime; i++){
        done = 0;
      hello();
      delay(1000);
      }
      //break;
      run1 = true;
    }
    if (clock.isAlarm2() )
    {
      Serial.println("ALARM 2 TRIGGERED!");
      digitalWrite(32, HIGH);
      delay(500);
      digitalWrite(32, LOW);
      for(int j = 1; j<=sTime; j++){
        stopper = 0;
      hello2();
      delay(500);
      }
      //break;
      run2 = true;
    }

    delay(1000);

  }
}


void hello2() {
  // put your main code here, to run repeatedly:
  //digitalWrite(13, HIGH);
  //delay(2000);
  digitalWrite(45, 0);
  pizza = 0;
  while (1 == 1) {
    Serial.print("in loop");
    digitalWrite(45, pizza);
    delay(10);
    if (digitalRead(47) == 0) {
      pizza = 1;
      Serial.print(pizza);
      digitalWrite(45, pizza);
      break;
    }
    stopper = stopper + 1;
    Serial.println(stopper);
    if (stopper >= 1000) {
      tft.setCursor(75, 200);
      tft.setTextSize (2);
      tft.setTextColor(BLACK);
      digitalWrite(45, 1);
      digitalWrite(32, HIGH);
      delay(750);
      digitalWrite(32, LOW);
      drawBorder(RED);
      tft.println("Error, Check ");
      tft.println(" containment units.");
      tft.println("");
      tft.println(" Touch to Restart");
      waitOneTouch();
      resetFunc();
      break;
    }
  }
}




// Wait one touch

TSPoint waitOneTouch() {

  // wait 1 touch to exit function

  TSPoint p;

  do {
    p = ts.getPoint();

    pinMode(XM, OUTPUT); //Pins configures again for TFT control
    pinMode(YP, OUTPUT);

  } while ((p.z < MINPRESSURE ) || (p.z > MAXPRESSURE));

  return p;
}


void drawBorder( int colour) {

  // Draw a border

  uint16_t width = tft.width() - 1;
  uint16_t height = tft.height() - 1;
  uint8_t border = 10;

  tft.fillScreen(colour);
  tft.fillRect(border, border, (width - border * 2), (height - border * 2), WHITE);

}
void checkAlarms()
{
  RTCAlarmTime a1;
  RTCAlarmTime a2;

  if (clock.isArmed1())
  {
    a1 = clock.getAlarm1();

    Serial.print("Alarm1 is triggered ");
    switch (clock.getAlarmType1())
    {
      case DS3231_EVERY_SECOND:
        Serial.println("every second");
        break;
      case DS3231_MATCH_S:
        Serial.print("when seconds match: ");
        Serial.println(clock.dateFormat("__ __:__:s", a1));
        break;
      case DS3231_MATCH_M_S:
        Serial.print("when minutes and sencods match: ");
        Serial.println(clock.dateFormat("__ __:i:s", a1));
        break;
      case DS3231_MATCH_H_M_S:
        Serial.print("when hours, minutes and seconds match: ");
        Serial.println(clock.dateFormat("__ H:i:s", a1));
        break;
      case DS3231_MATCH_DT_H_M_S:
        Serial.print("when date, hours, minutes and seconds match: ");
        Serial.println(clock.dateFormat("d H:i:s", a1));
        break;
      case DS3231_MATCH_DY_H_M_S:
        Serial.print("when day of week, hours, minutes and seconds match: ");
        Serial.println(clock.dateFormat("l H:i:s", a1));
        break;
      default:
        Serial.println("UNKNOWN RULE");
        break;
    }
  } else
  {
    Serial.println("Alarm1 is disarmed.");
  }

  if (clock.isArmed2())
  {
    a2 = clock.getAlarm1();

    Serial.print("Alarm1 is triggered ");
    switch (clock.getAlarmType2())
    {
      case DS3231_EVERY_MINUTE:
        Serial.println("every minute");
        break;
      case DS3231_MATCH_M:
        Serial.print("when minutes match: ");
        Serial.println(clock.dateFormat("__ __:i:s", a2));
        break;
      case DS3231_MATCH_H_M:
        Serial.print("when hours and minutes match:");
        Serial.println(clock.dateFormat("__ H:i:s", a2));
        break;
      case DS3231_MATCH_DT_H_M:
        Serial.print("when date, hours and minutes match: ");
        Serial.println(clock.dateFormat("d H:i:s", a2));
        break;
      case DS3231_MATCH_DY_H_M:
        Serial.println("when day of week, hours and minutes match: ");
        Serial.print(clock.dateFormat("l H:i:s", a2));
        break;
      default:
        Serial.println("UNKNOWN RULE");
        break;
    }
  } else
  {
    Serial.println("Alarm1 is disarmed.");
  }
}



//call reset

